#pragma once

#include "triplet.hpp"
#include "abstract_error.hpp"
#include <vector>

namespace Errors {
    class SocketSelectorError : public AbstractError {
    public:
        explicit SocketSelectorError(const char* string) : AbstractError(string) {}

        explicit SocketSelectorError(const std::string& string) : AbstractError(string) {}

        explicit SocketSelectorError(const std::runtime_error& error) : AbstractError(error) {}

        explicit SocketSelectorError(const std::string& message, const AbstractError& error) : AbstractError(message,
                                                                                                             error) {}

        explicit SocketSelectorError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Utils {
    class SocketSelector {
    public:
        SocketSelector();

        void addForEverything(int socket_fd);

        void addForReading(int socket_fd);

        void addForWriting(int socket_fd);

        void addForErrors(int socket_fd);

        void deleteForEverything(int socket_fd);

        void deleteForReading(int socket_fd);

        void deleteForWriting(int socket_fd);

        void deleteForErrors(int socket_fd);

        [[nodiscard]] Triplet<std::vector<int>> select() const; // Returns {readable, writeable, exceptional}

        ~SocketSelector() noexcept;

    private:
        class Impl;

        Impl* _pimpl;
    };
}
