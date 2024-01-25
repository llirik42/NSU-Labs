#include "logger.hpp"

namespace Logging {
    class FileLogger : public Logger {
    public:
        explicit FileLogger(const std::string& file_path);

        void logInfo(const std::string& message) override;

        void logInfo(const std::string& message, const Errors::AbstractError& error) override;

        void logError(const std::string& message) override;

        void logError(const std::string& message, const Errors::AbstractError& error) override;

        ~FileLogger() override;

    private:
        class Impl;

        Impl* _pimpl;
    };
}
