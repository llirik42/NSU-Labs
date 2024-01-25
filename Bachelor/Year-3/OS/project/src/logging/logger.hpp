#pragma once

#include "../utils/abstract_error.hpp"

namespace Errors {
    class LoggingError : public AbstractError {
    public:
        explicit LoggingError(const char* string) : AbstractError(string) {}

        explicit LoggingError(const std::string& string) : AbstractError(string) {}

        explicit LoggingError(const std::runtime_error& error) : AbstractError(error) {}

        explicit LoggingError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit LoggingError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Logging {
    class Logger {
    public:
        virtual void logInfo(const std::string& message) = 0;

        virtual void logInfo(const std::string& message, const Errors::AbstractError& error) = 0;

        virtual void logError(const std::string& message) = 0;

        virtual void logError(const std::string& message, const Errors::AbstractError& error) = 0;

        virtual ~Logger() = default;
    };
}
