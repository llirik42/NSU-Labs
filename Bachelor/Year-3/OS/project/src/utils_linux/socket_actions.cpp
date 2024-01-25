#include "../utils/socket_actions.hpp"
#include "errors.hpp"
#include "../utils/cerror.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

constexpr int DEFAULT_PROTOCOL = 0;
constexpr socklen_t SOCKADDR_IN_LENGTH = sizeof(struct sockaddr_in);

Utils::SocketInfo create_socket_info(int socket_fd, uint16_t port, const char* address) {
    Utils::SocketInfo result = {
            .socket_fd = socket_fd,
            .address = {'\0'}
    };
    sprintf(result.address, "%s:%d", address, port);

    return result;
}

sockaddr_in create_zero_sockaddr() {
    struct in_addr in_addr = {
            .s_addr = 0
    };

    return {
            .sin_family = 0,
            .sin_port = 0,
            .sin_addr = in_addr,
            .sin_zero = {0}
    };
}

Linux::LinuxError openSS(uint16_t port, const char* address, int backlog, int* server_socket) {
    *server_socket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);

    if (*server_socket == -1) {
        return Linux::createErrnoError(errno, "socket() failed");
    }

    const struct in_addr server_in_addr = {
            .s_addr = inet_addr(address),
    };

    const struct sockaddr_in server_sockaddr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = server_in_addr,
            .sin_zero = {0}
    };

    if (bind(*server_socket, reinterpret_cast<const struct sockaddr*>(&server_sockaddr), sizeof(server_sockaddr)) ==
        -1) {
        return Linux::createErrnoError(errno, "bind() failed");
    }

    if (listen(*server_socket, backlog) == -1) {
        return Linux::createErrnoError(errno, "listen() failed");
    }

    return Linux::createSuccess();
}

Utils::SocketInfo Utils::openServerSocket(uint16_t port, const char* address, int backlog) {
    const int old_errno = errno;
    int server_socket;
    const Linux::LinuxError error = openSS(port, address, backlog, &server_socket);
    errno = old_errno;

    if (error.code) {
        throw Errors::NetworkError(Linux::cerror("Cannot open server socket", error));
    }

    return create_socket_info(
            server_socket,
            port,
            address
    );
}

Linux::LinuxError openDS(uint16_t port, const char* address, int* destination_socket, char** ip_address) {
    // Trying to open new socket
    *destination_socket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (*destination_socket == -1) {
        return Linux::createErrnoError(errno, "socket() failed");
    }

    // Getting information about destination
    struct addrinfo* addr_info_res;

    const int addr_info_code = getaddrinfo(address, nullptr, nullptr, &addr_info_res);
    if (addr_info_code || addr_info_res == nullptr) {
        if (addr_info_code == EAI_SYSTEM) {
            return Linux::createErrnoError(errno, "getaddrinfo() failed");
        } else {
            return Linux::createGAIError(addr_info_code, "getaddrinfo() failed");
        }
    }

    // Trying to connect to any of addresses
    bool connected = false;

    for (auto it = addr_info_res; it != nullptr; it = it->ai_next) {
        auto sockaddr_in_ptr = reinterpret_cast<struct sockaddr_in*>(it->ai_addr);
        sockaddr_in_ptr->sin_port = htons(port);

        // Connecting until connect() isn't interrupted by signal
        int connect_code;
        while (true) {
            connect_code = connect(*destination_socket, reinterpret_cast<const struct sockaddr*>(sockaddr_in_ptr),
                                   sizeof(*sockaddr_in_ptr));

            if (connect_code == 0 || errno != EINTR) {
                break;
            }
        }

        // Connection succeeded
        if (connect_code != -1) {
            connected = true;
            auto tmp = reinterpret_cast<struct sockaddr_in*>(it->ai_addr);
            if (ip_address != nullptr) {
                char* ip_addr = inet_ntoa(tmp->sin_addr);
                strcpy(*ip_address, ip_addr);
            }
            break;
        }
    }

    freeaddrinfo(addr_info_res);

    if (connected) {
        return Linux::createSuccess();
    }

    return Linux::createErrnoError(errno, "connect() failed");
}

Utils::SocketInfo Utils::openDestinationSocket(uint16_t destination_port, const char* destination_address) {
    const int old_errno = errno;
    int destination_socket;
    char* ip_address = new char[40];
    const Linux::LinuxError error = openDS(destination_port, destination_address, &destination_socket, &ip_address);
    errno = old_errno;

    if (error.code) {
        throw Errors::NetworkError(Linux::cerror("Cannot open destination socket", error));
    }

    const SocketInfo info = create_socket_info(
            destination_socket,
            destination_port,
            ip_address
    );

    delete[] ip_address;

    return info;
}

Linux::LinuxError
acceptNBS(int listening_socket, int* client_socket, sockaddr_in* client_sockaddrr, socklen_t* client_socklen) {
    *client_sockaddrr = create_zero_sockaddr();
    *client_socklen = SOCKADDR_IN_LENGTH;

    int code;
    while (true) {
        code = accept4(listening_socket, reinterpret_cast<struct sockaddr*>(client_sockaddrr), client_socklen,
                       SOCK_NONBLOCK);

        if (code != -1 || errno != EINTR) {
            break;
        }
    }

    if (code == -1) {
        return Linux::createErrnoError(errno, "accept4() failed");
    }

    *client_socket = code;

    return Linux::createSuccess();
}

Utils::SocketInfo Utils::acceptNonBlockingSocket(int listening_socket) {
    const int old_errno = errno;
    int client_socket;
    sockaddr_in client_sockaddr = create_zero_sockaddr();
    socklen_t client_socklen;
    const Linux::LinuxError error = acceptNBS(listening_socket, &client_socket, &client_sockaddr, &client_socklen);
    errno = old_errno;

    if (error.code) {
        throw Errors::NetworkError(Linux::cerror("Cannot accept socket", error));
    }

    return create_socket_info(
            client_socket,
            ntohs(client_sockaddr.sin_port),
            inet_ntoa(client_sockaddr.sin_addr)
    );
}

void Utils::closeSocket(int s) noexcept {
    const int old_errno = errno;

    while (true) {
        const int code = close(s);
        const int current_errno = errno;

        if (code == 0 || current_errno != EINTR) {
            break;
        }
    }

    errno = old_errno;
}

ssize_t sendM(int socket, const void* message, size_t length) {
    ssize_t ret;
    while (true) {
        ret = send(socket, message, length, MSG_NOSIGNAL);

        if (ret != -1 || errno != EINTR) {
            break;
        }
    }

    return ret;
}

size_t Utils::sendMessage(int socket, const void* message, size_t length) {
    const int old_errno = errno;
    const ssize_t sent_count = sendM(socket, message, length);
    const int current_errno = errno;
    errno = old_errno;

    if (sent_count == -1) {
        throw Errors::NetworkError(Utils::cerror("Cannot send message", current_errno));
    }

    return static_cast<size_t>(sent_count);
}

ssize_t recvM(int socket, void* message, size_t length) {
    ssize_t ret;
    while (true) {
        ret = read(socket, message, length);

        if (ret >= 0 || errno != EINTR) {
            break;
        }
    }

    return ret;
}

size_t Utils::recvMessage(int socket, void* message, size_t length) {
    const int old_errno = errno;
    const ssize_t recv_count = recvM(socket, message, length);
    const int current_errno = errno;
    errno = old_errno;

    if (recv_count == 0) {
        throw Errors::SocketClosedError();
    }

    if (recv_count == -1) {
        throw Errors::NetworkError(Utils::cerror("Cannot receive message", current_errno));
    }

    return static_cast<size_t>(recv_count);
}
