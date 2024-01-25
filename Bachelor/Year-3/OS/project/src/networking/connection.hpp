#pragma once

#include "../utils/abstract_error.hpp"
#include "../utils/socket_info.hpp"
#include "../utils/socket_like.hpp"
#include <vector>

namespace Errors {
    class ConnectionError : public AbstractError {
    public:
        explicit ConnectionError(const char* string) : AbstractError(string) {}

        explicit ConnectionError(const std::string& string) : AbstractError(string) {}

        explicit ConnectionError(const std::runtime_error& error) : AbstractError(error) {}

        explicit ConnectionError(const std::string& message, const AbstractError& error) : AbstractError(message,
                                                                                                         error) {}

        explicit ConnectionError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };

    class ConnectionClosedError : public ConnectionError {
    public:
        explicit ConnectionClosedError(const char* string) : ConnectionError(string) {}

        explicit ConnectionClosedError(const std::string& string) : ConnectionError(string) {}

        explicit ConnectionClosedError(const std::runtime_error& error) : ConnectionError(error) {}

        explicit ConnectionClosedError(const std::string& message, const ConnectionError& error) : ConnectionError(
                message, error) {}

        explicit ConnectionClosedError(const char* message, const Errors::AbstractError& error) : ConnectionError(
                message, error) {}
    };
}

namespace Networking {
    class Connection : public Utils::SocketLike {
    public:
        explicit Connection(const Utils::SocketInfo& info);

        size_t send(const char* buffer, size_t length) const;

        size_t recv(char* buffer, size_t max_length) const;

        [[nodiscard]] const std::string& repr() const;

        ~Connection() override;

    private:
        class Impl;

        Impl* _pimpl;
    };
}
