#include "errors.hpp"
#include "../utils/cerror.hpp"
#include <netdb.h>

Linux::LinuxError Linux::createSuccess() {
    return {.code = 0, .type = NO_TYPE};
}

Linux::LinuxError Linux::createErrnoError(int code, const char* message) {
    return {.message = message, .code = code, .type = ERRNO_TYPE};
}

Linux::LinuxError Linux::createGAIError(int code, const char* message) {
    return {.message = message, .code = code, .type = GAI_TYPE};
}

std::string Linux::linuxErrorToString(const LinuxError& error) {
    switch (error.type) {
        case NO_TYPE: return error.message + std::string(": Success");
        case GAI_TYPE: return error.message + std::string(": ") + std::string(gai_strerror(error.code));
        default: return {};
    }
}

std::string Linux::cerror(const char* message, LinuxError error) {
    if (error.type == ERRNO_TYPE) {
        return Utils::cerror(message, error.code);
    } else {
        return Utils::cerror(message, Linux::linuxErrorToString(error));
    }
}
