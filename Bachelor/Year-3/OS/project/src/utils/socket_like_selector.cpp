#include "socket_selector.hpp"
#include "mutex.hpp"
#include "socket_like_selector.hpp"
#include <vector>
#include <set>

class Utils::SocketLikeSelector::Impl {
public:
    Impl() {
        try {
            _mutex = new Utils::Mutex(Utils::FAST);
            _socket_selector = new Utils::SocketSelector();
        } catch (const Errors::AbstractError& error) {
            throw Errors::SocketLikeSelectorError("Cannot init connection-selector", error);
        }
    }

    void addForEverything(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->addForEverything(socket_like->getSocketFd());
            _connections_for_read.insert(socket_like);
            _connections_for_write.insert(socket_like);
            _connections_for_errors.insert(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Adding for everything failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    void addForReading(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->addForReading(socket_like->getSocketFd());
            _connections_for_read.insert(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Adding for reading failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    void addForWriting(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->addForWriting(socket_like->getSocketFd());
            _connections_for_write.insert(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Adding for writing failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    void addForErrors(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->addForErrors(socket_like->getSocketFd());
            _connections_for_errors.insert(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Adding for errors failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    void deleteForEverything(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->deleteForEverything(socket_like->getSocketFd());
            _connections_for_read.erase(socket_like);
            _connections_for_write.erase(socket_like);
            _connections_for_errors.erase(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Deleting for everything failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    void deleteForReading(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->deleteForReading(socket_like->getSocketFd());
            _connections_for_read.erase(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Deleting for reading failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    void deleteForWriting(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->deleteForWriting(socket_like->getSocketFd());
            _connections_for_write.erase(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Deleting for writing failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    void deleteForErrors(SocketLike* socket_like) {
        try {
            _mutex->lock();
            _socket_selector->deleteForErrors(socket_like->getSocketFd());
            _connections_for_errors.erase(socket_like);
            _mutex->unlock();
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Deleting for errors failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

    [[nodiscard]] Utils::Triplet<std::vector<SocketLike*>> select() const {
        try {
            const auto [readable_sockets, writeable_sockets, exceptional_sockets] = _socket_selector->select();

            std::vector<SocketLike*> readable_connections;
            std::vector<SocketLike*> writeable_connections;
            std::vector<SocketLike*> exceptional_connections;

            readable_connections.reserve(readable_sockets.size());
            writeable_connections.reserve(writeable_sockets.size());
            exceptional_connections.reserve(exceptional_sockets.size());

            for (const auto& it: readable_sockets) {
                readable_connections.push_back(findSocketLike(it));
            }

            for (const auto& it: writeable_sockets) {
                writeable_connections.push_back(findSocketLike(it));
            }

            for (const auto& it: exceptional_sockets) {
                exceptional_connections.push_back(findSocketLike(it));
            }

            return {readable_connections, writeable_connections, exceptional_connections};
        } catch (const Errors::SocketSelectorError& error) {
            const std::string message = "Select of connection-selector failed";
            throw Errors::SocketLikeSelectorError(message, error);
        }
    }

private:
    Utils::Mutex* _mutex;
    Utils::SocketSelector* _socket_selector;
    std::set<SocketLike*> _connections_for_read;
    std::set<SocketLike*> _connections_for_write;
    std::set<SocketLike*> _connections_for_errors;

    [[nodiscard]] SocketLike* findSocketLike(int fd) const {
        try {
            _mutex->lock();

            for (const auto& it: _connections_for_read) {
                if (it->getSocketFd() == fd) {
                    _mutex->unlock();
                    return it;
                }
            }

            for (const auto& it: _connections_for_write) {
                if (it->getSocketFd() == fd) {
                    _mutex->unlock();
                    return it;
                }
            }

            for (const auto& it: _connections_for_errors) {
                if (it->getSocketFd() == fd) {
                    _mutex->unlock();
                    return it;
                }
            }

            _mutex->unlock();

            return nullptr; // Impossible case (poll/select/epoll/... cannot return fd that wasn't added to them)
        } catch (const Errors::AbstractError& error) {
            throw Errors::SocketLikeSelectorError("Searching for the fd in the connections-list failed", error);
        }
    }
};

Utils::SocketLikeSelector::SocketLikeSelector() {
    _pimpl = new Impl();
}

void Utils::SocketLikeSelector::addForEverything(SocketLike* socket_like) {
    _pimpl->addForEverything(socket_like);
}

void Utils::SocketLikeSelector::addForReading(SocketLike* socket_like) {
    _pimpl->addForReading(socket_like);
}

void Utils::SocketLikeSelector::addForWriting(SocketLike* socket_like) {
    _pimpl->addForWriting(socket_like);
}

void Utils::SocketLikeSelector::addForErrors(SocketLike* socket_like) {
    _pimpl->addForErrors(socket_like);
}

void Utils::SocketLikeSelector::deleteForEverything(SocketLike* socket_like) {
    _pimpl->deleteForEverything(socket_like);
}

void Utils::SocketLikeSelector::deleteForReading(SocketLike* socket_like) {
    _pimpl->deleteForReading(socket_like);
}

void Utils::SocketLikeSelector::deleteForWriting(SocketLike* socket_like) {
    _pimpl->deleteForWriting(socket_like);
}

void Utils::SocketLikeSelector::deleteForErrors(SocketLike* socket_like) {
    _pimpl->deleteForErrors(socket_like);
}

[[nodiscard]] Utils::Triplet<std::vector<Utils::SocketLike*>> Utils::SocketLikeSelector::select() const {
    return _pimpl->select();
}

Utils::SocketLikeSelector::~SocketLikeSelector() noexcept {
    delete _pimpl;
}
