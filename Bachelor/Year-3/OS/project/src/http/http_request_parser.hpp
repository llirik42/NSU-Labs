#pragma once

#include "dto.hpp"

namespace HTTP {
    class RequestParser {
    public:
        RequestParser();

        Request parse(const std::string& string);

        ~RequestParser();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
