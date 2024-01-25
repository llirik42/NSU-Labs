#pragma once

#include "socket_info.hpp"
#include "abstract_error.hpp"

namespace Errors {
    class NetworkError : public AbstractError {
    public:
        explicit NetworkError(const char* string) : AbstractError(string) {}

        explicit NetworkError(const std::string& string) : AbstractError(string) {}

        explicit NetworkError(const std::runtime_error& error) : AbstractError(error) {}

        explicit NetworkError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit NetworkError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };

    class SocketClosedError : public NetworkError {
    public:
        explicit SocketClosedError() : NetworkError("socket closed") {}
    };
}

namespace Utils {
    SocketInfo openServerSocket(uint16_t port, const char* address, int backlog);

    SocketInfo openDestinationSocket(uint16_t destination_port, const char* destination_address);

    SocketInfo acceptNonBlockingSocket(int listening_socket);

    void closeSocket(int s) noexcept;

    size_t sendMessage(int socket, const void* message, size_t length);

    size_t recvMessage(int socket, void* message, size_t length);
}
