#pragma once

#include "abstract_error.hpp"

namespace Errors {
    class RWLockError : public AbstractError {
    public:
        explicit RWLockError(const char* string) : AbstractError(string) {}

        explicit RWLockError(const std::string& string) : AbstractError(string) {}

        explicit RWLockError(const std::runtime_error& error) : AbstractError(error) {}

        explicit RWLockError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit RWLockError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Utils {
    class RWLock {
    public:
        RWLock();

        void readLock();

        void writeLock();

        void unlock();

        ~RWLock();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
