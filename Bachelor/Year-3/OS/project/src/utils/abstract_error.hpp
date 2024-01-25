#pragma once

#include <stdexcept>

namespace Errors {
    class AbstractError : public std::runtime_error {
    public:
        explicit AbstractError(const char* string) : std::runtime_error(string) {}

        explicit AbstractError(const std::string& string) : AbstractError(string.c_str()) {}

        explicit AbstractError(const std::runtime_error& error) : std::runtime_error(error) {}

        explicit AbstractError(const std::string& message, const AbstractError& error) : AbstractError(
                message + ": " + error.what()) {}

        explicit AbstractError(const char* message, const AbstractError& error) : AbstractError(std::string(message),
                                                                                                error) {}
    };
}
