#pragma once

#include "dto.hpp"

namespace HTTP {
    class ResponseParser {
    public:
        ResponseParser();

        Response parseResponse(const std::string& data);

        ~ResponseParser();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
