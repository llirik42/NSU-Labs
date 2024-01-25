#pragma once

#include <string>

namespace Linux {
    enum NetworkErrorType {
        NO_TYPE,
        ERRNO_TYPE,
        GAI_TYPE
    };

    struct LinuxError {
        const char* message;
        int code;
        NetworkErrorType type;
    };

    LinuxError createSuccess();

    LinuxError createErrnoError(int code, const char* message);

    LinuxError createGAIError(int code, const char* message);

    std::string linuxErrorToString(const LinuxError& error);

    std::string cerror(const char* message, LinuxError error);
}
