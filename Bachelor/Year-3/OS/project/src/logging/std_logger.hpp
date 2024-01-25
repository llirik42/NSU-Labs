#include "logger.hpp"

namespace Logging {
    class StdLogger : public Logger {
    public:
        void logInfo(const std::string& message) override;

        void logInfo(const std::string& message, const Errors::AbstractError& error) override;

        void logError(const std::string& message) override;

        void logError(const std::string& message, const Errors::AbstractError& error) override;
    };
}
