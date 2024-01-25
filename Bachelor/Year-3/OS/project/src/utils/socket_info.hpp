#pragma once

namespace Utils {
    struct SocketInfo {
        int socket_fd;
        char address[40]; // max length of ipv6-host_name + '\0'
    };
}
