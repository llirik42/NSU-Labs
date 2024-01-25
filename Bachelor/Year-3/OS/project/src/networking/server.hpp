#pragma once

#include "connection.hpp"
#include "../utils/abstract_error.hpp"
#include <memory>

namespace Errors {
    class ServerError : public AbstractError {
    public:
        explicit ServerError(const char* string) : AbstractError(string) {}

        explicit ServerError(const std::string& string) : AbstractError(string) {}

        explicit ServerError(const std::runtime_error& error) : AbstractError(error) {}

        explicit ServerError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit ServerError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Networking {
    class Server {
    public:
        Server(uint16_t port, int backlog);

        Server(uint16_t port, const char* address, int backlog);

        [[nodiscard]] Connection* acceptConnection() const;

        [[nodiscard]] int getBacklog() const;

        [[nodiscard]] const std::string& repr() const;

        ~Server();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
