#pragma once

#include "abstract_error.hpp"

namespace Errors {
    class ThreadError : public AbstractError {
    public:
        explicit ThreadError(const char* string) : AbstractError(string) {}

        explicit ThreadError(const std::string& string) : AbstractError(string) {}

        explicit ThreadError(const std::runtime_error& error) : AbstractError(error) {}

        explicit ThreadError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit ThreadError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Utils {
    class Thread {
    public:
        Thread(void* (* start_routine)(void*), void* arg);

        void cancel();

        ~Thread();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
