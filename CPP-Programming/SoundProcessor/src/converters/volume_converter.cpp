#include "../custom_exceptions.h"
#include "converters_utils.h"
#include "volume_converter.h"

void convert_volume(Streams::OutputStream& output_stream,
                    Streams::InputStream& input_stream,
                    float coefficient,
                    size_t start_sample,
                    size_t end_sample){

    for (size_t i = 0; i < start_sample; i++){
        output_stream.write(input_stream.read_element());
    }

    for (size_t i = start_sample; i < end_sample; i++){
        auto tmp = static_cast<float>(input_stream.read_element());
        auto new_value = static_cast<WAVFormatInfo::Sample>(tmp * coefficient);
        output_stream.write(new_value);
    }

    while (input_stream.available()){
        output_stream.write(input_stream.read_element());
    }
}

void RawVolumeConverter::convert(std::string_view command,
                                 Streams::OutputStream& output_stream,
                                 Streams::InputStream& input_stream,
                                 const Converters::ConverterParams& params) const{

    if ((command == "mute" && params.size() != 2) || (command == "volume" && params.size() != 3)){
        throw Exceptions::IncorrectCommandParams();
    }

    size_t start_time_index = command == "mute" ? 0 : 1;
    size_t end_time_index = command == "mute" ? 1 : 2;

    float coefficient = command == "volume" ? convert_any<float>(params[0]) : 0;

    auto start_sample = calculate_sample_index_by_time(
            convert_any<float>(params[start_time_index]),
            input_stream
            );

    auto end_sample = calculate_sample_index_by_time(
            convert_any<float>(params[end_time_index]),
            input_stream
    );

    check_time_fragment(start_sample, end_sample, input_stream);

    convert_volume(output_stream, input_stream, coefficient, start_sample, end_sample);
}
