#pragma once

#include "abstract_error.hpp"

namespace Errors {
    class MutexError : public AbstractError {
    public:
        explicit MutexError(const char* string) : AbstractError(string) {}

        explicit MutexError(const std::string& string) : AbstractError(string) {}

        explicit MutexError(const std::runtime_error& error) : AbstractError(error) {}

        explicit MutexError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit MutexError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Utils {
    enum MutexKind {
        FAST,
        ERROR_CHECKING,
        RECURSIVE
    };

    class Mutex {
    public:
        explicit Mutex(MutexKind kind);

        void lock();

        void unlock();

        ~Mutex();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
