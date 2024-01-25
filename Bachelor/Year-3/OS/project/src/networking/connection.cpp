#include "connection.hpp"
#include <vector>

class Networking::Connection::Impl {
public:
    explicit Impl(const Utils::SocketInfo& info) {
        _socket_fd = info.socket_fd;
        _repr = std::string(info.address);
    }

    size_t send(const char* buffer, size_t length) const {
        return Utils::sendMessage(_socket_fd, buffer, length);
    }

    size_t recv(char* buffer, size_t max_length) const {
        return Utils::recvMessage(_socket_fd, buffer, max_length);
    }

    [[nodiscard]] const std::string& repr() const {
        return _repr;
    }

private:
    int _socket_fd;
    std::string _repr;
};

Networking::Connection::Connection(const Utils::SocketInfo& info) : SocketLike(info.socket_fd) {
    _pimpl = new Impl(info);
}

size_t Networking::Connection::send(const char* buffer, size_t length) const {
    try {
        return _pimpl->send(buffer, length);
    } catch (const Errors::NetworkError& error) {
        throw Errors::ConnectionError("Connection failed to send data", error);
    }
}

size_t Networking::Connection::recv(char* buffer, size_t max_length) const {
    try {
        return _pimpl->recv(buffer, max_length);
    } catch (const Errors::SocketClosedError& error) {
        throw Errors::ConnectionClosedError(error);
    } catch (const Errors::NetworkError& error) {
        throw Errors::ConnectionError("Connection failed to receive data", error);
    }
}

const std::string& Networking::Connection::repr() const {
    return _pimpl->repr();
}

Networking::Connection::~Connection() {
    delete _pimpl;
}
