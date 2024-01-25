#pragma once

#include "socket_like.hpp"
#include "triplet.hpp"
#include "abstract_error.hpp"
#include <memory>

namespace Errors {
    class SocketLikeSelectorError : public AbstractError {
    public:
        explicit SocketLikeSelectorError(const char* string) : AbstractError(string) {}

        explicit SocketLikeSelectorError(const std::string& string) : AbstractError(string) {}

        explicit SocketLikeSelectorError(const std::runtime_error& error) : AbstractError(error) {}

        explicit SocketLikeSelectorError(const std::string& message, const AbstractError& error) : AbstractError(
                message, error) {}

        explicit SocketLikeSelectorError(const char* message, const AbstractError& error) : AbstractError(message,
                                                                                                          error) {}
    };
}

namespace Utils {
    class SocketLikeSelector {
    public:
        SocketLikeSelector();

        void addForEverything(SocketLike* socket_like);

        void addForReading(SocketLike* socket_like);

        void addForWriting(SocketLike* socket_like);

        void addForErrors(SocketLike* socket_like);

        void deleteForEverything(SocketLike* socket_like);

        void deleteForReading(SocketLike* socket_like);

        void deleteForWriting(SocketLike* socket_like);

        void deleteForErrors(SocketLike* socket_like);

        [[nodiscard]] Utils::Triplet<std::vector<SocketLike*>> select() const;

        ~SocketLikeSelector() noexcept;

    private:
        class Impl;

        Impl* _pimpl;
    };
}
