#pragma once

#include <string>

namespace HTTP {
    class ResponseCreator {
    public:
        std::string createResponseToIncorrectRequest();

        std::string createResponseToUnknownDestination();

        std::string createResponseToUnsupportedMethod();
    };
}
