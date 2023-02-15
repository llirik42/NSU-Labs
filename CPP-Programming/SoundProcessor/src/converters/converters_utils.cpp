#include <functional>
#include "../processor.h"
#include "converters_utils.h"

size_t calculate_sample_index_by_time(float time, Streams::InputStream& input_stream){

    // I know what I'm doing with comparison floating numbers by ==
    if (time == Processing::LEFT_TIME_ARROW_ALIAS){
        return 0;
    }

    float tmp = static_cast<float>(input_stream.get_size()) /
                static_cast<float>(input_stream.get_duration_s());

    float new_time = time == Processing::RIGHT_TIME_ARROW_ALIAS ? input_stream.get_duration_s() : time;

    return static_cast<size_t>(new_time * tmp);
}

void check_time_fragment(size_t start_sample, size_t end_sample, Streams::InputStream& input_stream){
    if (end_sample < start_sample || end_sample > input_stream.get_size()){
        throw Exceptions::IncorrectCommandParams();
    }
}
