#pragma once

#include "../utils/abstract_error.hpp"

namespace Errors {
    class InvalidHTTPRequestError : public AbstractError {
    public:
        explicit InvalidHTTPRequestError() : AbstractError("Invalid HTTP-request") {}

        explicit InvalidHTTPRequestError(const char* string) : AbstractError(string) {}

        explicit InvalidHTTPRequestError(const std::string& string) : AbstractError(string) {}

        explicit InvalidHTTPRequestError(const std::runtime_error& error) : AbstractError(error) {}

        explicit InvalidHTTPRequestError(const std::string& message, const AbstractError& error) : AbstractError(
                message, error) {}

        explicit InvalidHTTPRequestError(const char* message, const AbstractError& error) : AbstractError(message,
                                                                                                          error) {}
    };

    class InvalidHTTPResponseError : public AbstractError {
    public:
        explicit InvalidHTTPResponseError() : AbstractError("Invalid HTTP-response") {}

        explicit InvalidHTTPResponseError(const char* string) : AbstractError(string) {}

        explicit InvalidHTTPResponseError(const std::string& string) : AbstractError(string) {}

        explicit InvalidHTTPResponseError(const std::runtime_error& error) : AbstractError(error) {}

        explicit InvalidHTTPResponseError(const std::string& message, const AbstractError& error) : AbstractError(
                message, error) {}

        explicit InvalidHTTPResponseError(const char* message, const AbstractError& error) : AbstractError(message,
                                                                                                           error) {}
    };
}
