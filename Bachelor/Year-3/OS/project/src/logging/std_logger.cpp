#include "std_logger.hpp"
#include <iostream>

void log(const std::string& message, std::basic_ostream<char>& ostream) {
    ostream << message << "\n";
    ostream.flush();
}

void log(const std::string& message, const Errors::AbstractError& error, std::basic_ostream<char>& ostream) {
    log(message + ": " + error.what(), ostream);
}

void Logging::StdLogger::logInfo(const std::string& message) {
    log("INFO    " + message, std::cout);
}

void Logging::StdLogger::logInfo(const std::string& message, const Errors::AbstractError& error) {
    log("INFO    " + message, error, std::cout);
}

void Logging::StdLogger::logError(const std::string& message) {
    log("ERROR   " + message, std::cerr);
}

void Logging::StdLogger::logError(const std::string& message, const Errors::AbstractError& error) {
    log("ERROR   " + message, error, std::cerr);
}
