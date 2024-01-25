#pragma once

#include "dto.hpp"
#include <string>

namespace HTTP {
    long findIndexOfFirstCharAfterHeaders(const std::string& old, const std::string& cur);

    Request transformToHTTP1Dot0Request(const Request& request);

    std::string getShortReprOfHTTPData(const char* buffer, size_t length);

    std::string requestToString(const Request& request);
}
