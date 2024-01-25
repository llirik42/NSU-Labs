#pragma once

#include <string>
#include <vector>

namespace HTTP {
    struct Header {
        std::string name;
        std::string value;
    };

    struct Request {
        std::string method;
        std::string uri;
        int major_version;
        int minor_version;
        std::vector<Header> headers;
    };

    struct Response {
        bool ok;
    };

    struct Host {
        std::string host_name;
        uint16_t port;
    };
}
