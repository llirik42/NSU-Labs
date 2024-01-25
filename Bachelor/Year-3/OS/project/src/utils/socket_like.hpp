#pragma once

#include "socket_actions.hpp"

namespace Utils {
    class SocketLike {
    public:
        SocketLike(): _socket_fd(-1) {}

        explicit SocketLike(int socket_fd) : _socket_fd(socket_fd) {}

        [[nodiscard]] int getSocketFd() const {
            return _socket_fd;
        }

        virtual ~SocketLike() {
            Utils::closeSocket(_socket_fd);
        }

    protected:
        void setSocketFd(int socket_fd) {
            _socket_fd = socket_fd;
        }

    private:
        int _socket_fd;
    };
}
