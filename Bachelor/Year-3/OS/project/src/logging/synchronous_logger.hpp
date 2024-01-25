#include "logger.hpp"
#include <memory>

namespace Logging {
    class SynchronousLogger : public Logger {
    public:
        explicit SynchronousLogger(const std::shared_ptr<Logger>& logger);

        void logInfo(const std::string& message) override;

        void logInfo(const std::string& message, const Errors::AbstractError& error) override;

        void logError(const std::string& message) override;

        void logError(const std::string& message, const Errors::AbstractError& error) override;

        ~SynchronousLogger() override;

    private:
        class Impl;

        Impl* _pimpl;
    };
}
