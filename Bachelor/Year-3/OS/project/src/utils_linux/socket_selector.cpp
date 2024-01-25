#include "../utils/socket_selector.hpp"
#include "../utils/cerror.hpp"
#include <set>
#include <sys/epoll.h>

constexpr int DEFAULT_EPOLL_CREATE_FLAGS = 0;
constexpr int MAX_EPOLL_EVENTS_COUNT = 64;
constexpr int NO_EPOLL_TIMEOUT = -1;

class Utils::SocketSelector::Impl {
public:
    Impl() {
        const int old_errno = errno;
        _epoll_fd = epoll_create1(DEFAULT_EPOLL_CREATE_FLAGS);
        const int current_errno = errno;
        errno = old_errno;

        if (_epoll_fd == -1) {
            throw Errors::SocketSelectorError(cerror("Cannot init socket-selector: epoll_create1() failed", current_errno));
        }
    }

    void addForEverything(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_read.insert(socket_fd);
        _fds_for_write.insert(socket_fd);
        _fds_for_errors.insert(socket_fd);
        addToEpoll(socket_fd);
    }

    void addForReading(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_read.insert(socket_fd);
        addToEpoll(socket_fd);
    }

    void addForWriting(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_write.insert(socket_fd);
        addToEpoll(socket_fd);
    }

    void addForErrors(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_errors.insert(socket_fd);
        addToEpoll(socket_fd);
    }

    void deleteForEverything(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_read.erase(socket_fd);
        _fds_for_write.erase(socket_fd);
        _fds_for_errors.erase(socket_fd);
    }

    void deleteForReading(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_read.erase(socket_fd);
        addToEpoll(socket_fd);
    }

    void deleteForWriting(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_write.erase(socket_fd);
        addToEpoll(socket_fd);
    }

    void deleteForErrors(int socket_fd) {
        delFromEpoll(socket_fd);
        _fds_for_errors.erase(socket_fd);
        addToEpoll(socket_fd);
    }

    [[nodiscard]] Triplet<std::vector<int>> select() const {
        struct epoll_event epoll_events[MAX_EPOLL_EVENTS_COUNT];

        const int old_errno = errno;
        int count_of_ready_fd;
        while (true) {
            count_of_ready_fd = epoll_wait(_epoll_fd, epoll_events, MAX_EPOLL_EVENTS_COUNT, NO_EPOLL_TIMEOUT);

            if (count_of_ready_fd > 0 || errno != EINTR) {
                break;
            }
        }
        const int current_errno = errno;
        errno = old_errno;

        if (count_of_ready_fd == -1) {
            throw Errors::SocketSelectorError(cerror("Select of socket-selector failed: epoll_wait() failed", current_errno));
        }

        std::vector<int> readable;
        std::vector<int> writeable;
        std::vector<int> exceptional;

        for (int i = 0; i < count_of_ready_fd; i++) {
            const epoll_event event = epoll_events[i];
            const int fd = event.data.fd;
            const uint32_t events = event.events;

            if (events & EPOLLIN) {
                readable.push_back(fd);
            }

            if (events & EPOLLOUT) {
                writeable.push_back(fd);
            }

            if (events ^ EPOLLOUT && events ^ EPOLLIN && events ^ EPOLLIN ^ EPOLLOUT) {
                exceptional.push_back(fd);
            }
        }

        return {readable, writeable, exceptional};
    }

private:
    int _epoll_fd;
    std::set<int> _fds_for_read;
    std::set<int> _fds_for_write;
    std::set<int> _fds_for_errors;

    void addToEpoll(int socket_fd) const {
        const epoll_data data = {
                .fd = socket_fd
        };

        struct epoll_event configuration_epoll_event = {
                .events = getEventsForFd(socket_fd),
                .data = data
        };

        const int old_errno = errno;
        const int code = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, socket_fd, &configuration_epoll_event);
        const int current_errno = errno;
        errno = old_errno;

        if (code == -1) {
            throw Errors::SocketSelectorError(cerror("Adding socket to socket-selector failed: epoll_ctl() failed", current_errno));
        }
    }

    void delFromEpoll(int socket_fd) const {
        if (!isFdRegistered(socket_fd)) {
            return;
        }

        struct epoll_event some_epoll_event = {};
        struct epoll_event* some_non_null_epoll_event_ptr = &some_epoll_event;

        const int old_errno = errno;
        const int code = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, socket_fd, some_non_null_epoll_event_ptr);
        const int current_errno = errno;
        errno = old_errno;

        if (code == -1) {
            throw Errors::SocketSelectorError(cerror("Deleting socket from socket-selector failed: epoll_ctl() failed", current_errno));
        }
    }

    [[nodiscard]] uint32_t getEventsForFd(int fd) const {
        uint32_t result = 0;

        if (contains(fd, _fds_for_read)) {
            result |= EPOLLIN;
        }

        if (contains(fd, _fds_for_write)) {
            result |= EPOLLOUT;
        }

        if (contains(fd, _fds_for_errors)) {
            result |= EPOLLPRI;
        }

        return result;
    }

    [[nodiscard]] bool isFdRegistered(int fd) const {
        return contains(fd, _fds_for_read) || contains(fd, _fds_for_write) || contains(fd, _fds_for_errors);
    }

    [[nodiscard]] static bool contains(int socket_fd, const std::set<int>& fds) {
        return fds.find(socket_fd) != fds.end();
    }
};

Utils::SocketSelector::SocketSelector() {
    _pimpl = new Impl();
}

void Utils::SocketSelector::addForEverything(int socket_fd) {
    _pimpl->addForEverything(socket_fd);
}

void Utils::SocketSelector::addForReading(int socket_fd) {
    _pimpl->addForReading(socket_fd);
}

void Utils::SocketSelector::addForWriting(int socket_fd) {
    _pimpl->addForWriting(socket_fd);
}

void Utils::SocketSelector::addForErrors(int socket_fd) {
    _pimpl->addForErrors(socket_fd);
}

void Utils::SocketSelector::deleteForEverything(int socket_fd) {
    _pimpl->deleteForEverything(socket_fd);
}

void Utils::SocketSelector::deleteForReading(int socket_fd) {
    _pimpl->deleteForReading(socket_fd);
}

void Utils::SocketSelector::deleteForWriting(int socket_fd) {
    _pimpl->deleteForWriting(socket_fd);
}

void Utils::SocketSelector::deleteForErrors(int socket_fd) {
    _pimpl->deleteForErrors(socket_fd);
}

Utils::Triplet<std::vector<int>> Utils::SocketSelector::select() const {
    return _pimpl->select();
}

Utils::SocketSelector::~SocketSelector() noexcept {
    delete _pimpl;
}
