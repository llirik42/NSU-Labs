#pragma once

#include <any>
#include "../custom_exceptions.h"
#include "../streams.h"

template<typename T>
T convert_any(std::any any){
    try{
        return std::any_cast<T>(any);
    }
    catch(...){
        throw Exceptions::IncorrectCommandParams();
    }
}

size_t calculate_sample_index_by_time(float time, Streams::InputStream& input_stream);

void check_time_fragment(size_t start_sample, size_t end_sample, Streams::InputStream& input_stream);
