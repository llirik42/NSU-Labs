#include "../custom_exceptions.h"
#include "converters_utils.h"
#include "insert_converter.h"

void insert(Streams::OutputStream& output_stream,
            Streams::InputStream& input_stream,
            Streams::InputStream& additional_stream,
            size_t insert_place,
            size_t start_place,
            size_t end_place){

    additional_stream.skip(0, start_place);

    for (size_t i = 0; i < insert_place; i++){
        output_stream.write(input_stream.read_element());
    }

    for (size_t i = start_place; i < end_place; i++){
        output_stream.write(additional_stream.read_element());
    }

    while (input_stream.available()){
        output_stream.write(input_stream.read_element());
    }
}

void RawInsertConverter::convert(std::string_view command,
                                 Streams::OutputStream& output_stream,
                                 Streams::InputStream& input_stream,
                                 const Converters::ConverterParams& params) const{

    if ((command == "insert" && params.size() != 4) || ((command == "front" || command == "back") && params.size() != 3)){
        throw Exceptions::IncorrectCommandParams();
    }

    auto& additional_stream = *convert_any<Streams::InputStream*>(params[0]);

    size_t insert_place;

    if (command == "insert"){
        insert_place = calculate_sample_index_by_time(convert_any<float>(params[1]),
                input_stream);
    }

    if (command == "front"){
        insert_place = 0;
    }

    if (command == "back"){
        insert_place = calculate_sample_index_by_time(input_stream.get_duration_s(),
                                                      input_stream);
    }

    size_t index1 = command == "insert" ? 2 : 1;
    size_t index2 = command == "insert" ? 3 : 2;

    auto start_sample = calculate_sample_index_by_time(convert_any<float>(params[index1]),
                                                       input_stream);

    auto end_sample = calculate_sample_index_by_time(convert_any<float>(params[index2]),
                                                     input_stream);

    check_time_fragment(start_sample, end_sample, additional_stream);
    size_t end_in_input_file = insert_place + end_sample - start_sample;
    check_time_fragment(insert_place, end_in_input_file, input_stream);

    insert(output_stream,
           input_stream,
           additional_stream,
           insert_place,
           start_sample,
           end_sample);
}
