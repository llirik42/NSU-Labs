#include "cerror.hpp"
#include <cstring>

std::string Utils::cerror(const char* message, const std::string& error) {
    return std::string(message) + ": " + error;
}

std::string Utils::cerror(const char* message, int errno_code) {
    const unsigned int buffer_size = 256;

    char buffer[buffer_size];
    int old_errno = errno;
    char* res = strerror_r(errno_code, buffer, buffer_size);
    errno = old_errno;
    const std::string ret = cerror(message, res);

    return ret;
}

std::string Utils::cerror(const std::string& message, const std::string& error) {
    return cerror(message.c_str(), error);
}

std::string Utils::cerror(const std::string& message, int errno_code) {
    return cerror(message.c_str(), errno_code);
}
