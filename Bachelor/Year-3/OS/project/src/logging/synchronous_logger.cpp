#include "synchronous_logger.hpp"
#include "file_logger.hpp"
#include "../utils/mutex.hpp"

class Logging::SynchronousLogger::Impl {
public:
    explicit Impl(const std::shared_ptr<Logger>& logger) {
        try {
            _logger = logger;
            _mutex = new Utils::Mutex(Utils::FAST);
        } catch (const Errors::MutexError& error) {
            throw Errors::LoggingError("Cannot init logger", error);
        }
    }

    void logInfo(const std::string& message) {
        try {
            _mutex->lock();
            _logger->logInfo(message);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            throw Errors::LoggingError("Cannot log", error);
        }
    }

    void logInfo(const std::string& message, const Errors::AbstractError& error) {
        try {
            _mutex->lock();
            _logger->logInfo(message, error);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            throw Errors::LoggingError("Cannot log", error);
        }
    }

    void logError(const std::string& message) {
        try {
            _mutex->lock();
            _logger->logError(message);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            throw Errors::LoggingError("Cannot log", error);
        }
    }

    void logError(const std::string& message, const Errors::AbstractError& error) {
        try {
            _mutex->lock();
            _logger->logError(message, error);
            _mutex->unlock();
        } catch (const Errors::AbstractError& e) {
            throw Errors::LoggingError("Cannot log", e);
        }
    }

    ~Impl() {
        delete _mutex;
    }

private:
    std::shared_ptr<Logger> _logger;
    Utils::Mutex* _mutex;
};

Logging::SynchronousLogger::SynchronousLogger(const std::shared_ptr<Logger>& logger) {
    _pimpl = new Impl(logger);
}

void Logging::SynchronousLogger::logInfo(const std::string& message) {
    _pimpl->logInfo(message);
}

void Logging::SynchronousLogger::logInfo(const std::string& message, const Errors::AbstractError& error) {
    _pimpl->logInfo(message, error);
}

void Logging::SynchronousLogger::logError(const std::string& message) {
    _pimpl->logError(message);
}

void Logging::SynchronousLogger::logError(const std::string& message, const Errors::AbstractError& error) {
    _pimpl->logError(message, error);
}

Logging::SynchronousLogger::~SynchronousLogger() {
    delete _pimpl;
}
