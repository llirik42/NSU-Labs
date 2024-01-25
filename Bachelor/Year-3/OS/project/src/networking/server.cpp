#include "server.hpp"

class Networking::Server::Impl {
public:
    explicit Impl(const Utils::SocketInfo& info, int backlog) {
        _socket_fd = info.socket_fd;
        _repr = std::string(info.address);
        _backlog = backlog;
    }

    [[nodiscard]] Connection* acceptConnection() const {
        return new Connection(Utils::acceptNonBlockingSocket(_socket_fd));
    }

    [[nodiscard]] int getBacklog() const {
        return _backlog;
    }

    [[nodiscard]] const std::string& repr() const {
        return _repr;
    }

private:
    int _socket_fd;
    int _backlog;
    std::string _repr;
};

Networking::Server::Server(uint16_t port, int backlog) : Server(port, "0.0.0.0", backlog) {}

Networking::Server::Server(uint16_t port, const char* address, int backlog) {
    try {
        const Utils::SocketInfo info = Utils::openServerSocket(port, address, backlog);
        _pimpl = new Impl(info, backlog);
    } catch (const Errors::NetworkError& error) {
        throw Errors::ServerError("Cannot init server", error);
    }
}

Networking::Connection* Networking::Server::acceptConnection() const {
    try {
        return _pimpl->acceptConnection();
    } catch (const Errors::NetworkError& error) {
        throw Errors::ServerError("Server failed to accept connection", error);
    }
}

int Networking::Server::getBacklog() const {
    return _pimpl->getBacklog();
}

const std::string& Networking::Server::repr() const {
    return _pimpl->repr();
}

Networking::Server::~Server() {
    delete _pimpl;
}
