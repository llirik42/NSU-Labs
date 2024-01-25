#include "file_logger.hpp"
#include <fstream>

class Logging::FileLogger::Impl {
public:
    explicit Impl(const std::string& file_path) {
        _stream = std::ofstream(file_path, std::ofstream::out | std::ofstream::trunc);

        if (!_stream.is_open()) {
            throw Errors::LoggingError("File + \"" + file_path + "\" cannot be open for logging");
        }
    }

    void logInfo(const std::string& message) {
        log("INFO    " + message);
    }

    void logInfo(const std::string& message, const Errors::AbstractError& error) {
        log("INFO    " + message, error);
    }

    void logError(const std::string& message) {
        log("ERROR   " + message);
    }

    void logError(const std::string& message, const Errors::AbstractError& error) {
        log("ERROR   " + message, error);
    }

private:
    std::ofstream _stream;

    void log(const std::string& message) {
        _stream << message << "\n";
        _stream.flush();
    }

    void log(const std::string& message, const Errors::AbstractError& error) {
        log(message + ": " + error.what());
    }
};

Logging::FileLogger::FileLogger(const std::string& file_path) {
    _pimpl = new Impl(file_path);
}

void Logging::FileLogger::logInfo(const std::string& message) {
    _pimpl->logInfo(message);
}

void Logging::FileLogger::logInfo(const std::string& message, const Errors::AbstractError& error) {
    _pimpl->logInfo(message, error);
}

void Logging::FileLogger::logError(const std::string& message) {
    _pimpl->logError(message);
}

void Logging::FileLogger::logError(const std::string& message, const Errors::AbstractError& error) {
    _pimpl->logError(message, error);
}

Logging::FileLogger::~FileLogger() {
    delete _pimpl;
}
