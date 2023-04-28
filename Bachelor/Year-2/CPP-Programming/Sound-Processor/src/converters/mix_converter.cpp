#include "../custom_exceptions.h"
#include "converters_utils.h"
#include "mix_converter.h"

void mix(Streams::OutputStream& output_stream,
         Streams::InputStream& input_stream,
         Streams::InputStream& additional_input_stream,
         size_t place_in_input_file,
         size_t start_sample_in_additional_file,
         size_t end_sample_in_additional_file,
         float coefficient){

    for (size_t i = 0; i < place_in_input_file; i++){
        output_stream.write(input_stream.read_element());
    }

    additional_input_stream.skip(start_sample_in_additional_file);

    for (size_t i = start_sample_in_additional_file; i < end_sample_in_additional_file; i++){
        auto sample1 = input_stream.read_element();
        auto sample2 = additional_input_stream.read_element();

        auto sample1_float = static_cast<float>(sample1);
        auto sample2_float = static_cast<float>(sample2);

        auto result_sample = static_cast<WAVFormatInfo::Sample>(sample1_float * (1 - coefficient) + sample2_float * coefficient);

        output_stream.write(result_sample);
    }

    while (input_stream.available()){
        output_stream.write(input_stream.read_element());
    }
}

void RawMixConverter::convert([[maybe_unused]] std::string_view command,
                              Streams::OutputStream& output_stream,
                              Streams::InputStream& input_stream,
                              const Converters::ConverterParams& params) const{

    if (params.size() != 4 && params.size() != 5){
        throw Exceptions::IncorrectCommandParams();
    }

    auto& additional_stream = *convert_any<Streams::InputStream*>(params[0]);

    float coefficient = params.size() == 5 ? convert_any<float>(params[1]) : 0.5f;

    const size_t index1 = params.size() == 5 ? 2 : 1;
    const size_t index2 = params.size() == 5 ? 3 : 2;
    const size_t index3 = params.size() == 5 ? 4 : 3;

    auto place_in_input_file = calculate_sample_index_by_time(
            convert_any<float>(params[index1]),
            input_stream
    );

    auto start_sample = calculate_sample_index_by_time(
            convert_any<float>(params[index2]),
            input_stream
    );

    auto end_sample = calculate_sample_index_by_time(
            convert_any<float>(params[index3]),
            input_stream
    );

    check_time_fragment(start_sample, end_sample, additional_stream);
    size_t end_in_input_file = place_in_input_file + end_sample - start_sample;
    check_time_fragment(place_in_input_file, end_in_input_file, input_stream);

    mix(output_stream,
        input_stream,
        additional_stream,
        place_in_input_file,
        start_sample,
        end_sample,
        coefficient);
}
