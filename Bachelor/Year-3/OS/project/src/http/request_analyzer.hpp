#pragma once

#include "dto.hpp"

namespace HTTP {
    class RequestAnalyzer {
    public:
        RequestAnalyzer();

        Host parseHost(const Request& request);

        std::string parseURL(const Request& request);

        bool isGetRequest(const Request& request);

        ~RequestAnalyzer();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
