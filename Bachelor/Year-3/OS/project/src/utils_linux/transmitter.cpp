#include "../utils/transmitter.hpp"
#include "../utils/cerror.hpp"
#include <pthread.h>
#include <csignal>
#include <sys/signalfd.h>
#include <unistd.h>

#define EVENT_1_INT_SIGNAL (SIGRTMIN + 1)
#define EVENT_1_PTR_SIGNAL (SIGRTMIN + 2)
#define EVENT_2_INT_SIGNAL (SIGRTMIN + 3)
#define EVENT_2_PTR_SIGNAL (SIGRTMIN + 4)
#define EVENT_3_INT_SIGNAL (SIGRTMIN + 5)
#define EVENT_3_PTR_SIGNAL (SIGRTMIN + 6)
#define EVENT_4_INT_SIGNAL (SIGRTMIN + 6)
#define EVENT_4_PTR_SIGNAL (SIGRTMIN + 7)

class Utils::Transmitter::Impl {
public:
    Impl() {
        _creator_tid = pthread_self();

        sigset_t mask;
        initEmptySigSet(&mask);
        addSignalToSigSet(&mask, EVENT_1_INT_SIGNAL);
        addSignalToSigSet(&mask, EVENT_1_PTR_SIGNAL);
        addSignalToSigSet(&mask, EVENT_2_INT_SIGNAL);
        addSignalToSigSet(&mask, EVENT_2_PTR_SIGNAL);
        addSignalToSigSet(&mask, EVENT_3_INT_SIGNAL);
        addSignalToSigSet(&mask, EVENT_3_PTR_SIGNAL);
        addSignalToSigSet(&mask, EVENT_4_INT_SIGNAL);
        addSignalToSigSet(&mask, EVENT_4_PTR_SIGNAL);
        setSigMask(SIG_BLOCK, &mask, nullptr);
        intiSignalFd(&mask);
    }

    [[nodiscard]] int getFd() const {
        return _signal_fd;
    }

    void sendInt(int value, Utils::TransmitterEventType type) const {
        sigval sigval = {.sival_int = value};
        sendSignal(sigval, eventTypeToSignalInt(type));
    }

    void sendPtr(void* ptr, Utils::TransmitterEventType type) const {
        sigval sigval = {.sival_ptr = ptr};
        sendSignal(sigval, eventTypeToSignalPtr(type));
    }

    [[nodiscard]] Utils::TransmitterEvent receive() const {
        struct signalfd_siginfo fdsi{};
        ssize_t total_read_count = 0;

        const int old_errno = errno;
        while (total_read_count != sizeof(struct signalfd_siginfo)) {
            char* char_ptr = reinterpret_cast<char*>(&fdsi);
            void* ptr = reinterpret_cast<void*>(char_ptr + total_read_count);

            // Read until reading isn't interrupted by signal
            ssize_t read_count;
            while (true) {
                read_count = read(_signal_fd, ptr, sizeof(struct signalfd_siginfo) -
                                                   static_cast<unsigned long>(total_read_count));

                // Error happened
                if (read_count == -1 && errno != EINTR) {
                    break;
                }

                // Successfully read some data (maybe not all)
                if (read_count > 0) {
                    break;
                }
            }

            // Serious error
            if (read_count == -1) {
                total_read_count = -1; // Signalize about error
                break;
            }

            total_read_count += read_count;
        }
        const int current_errno = errno;
        errno = old_errno;

        if (total_read_count == -1) {
            if (current_errno == EAGAIN) {
                throw Errors::NoEventsInTransmitter();
            } else {
                throw Errors::TransmitterError(Utils::cerror("Error when receiving signal: read() failed", current_errno));
            }
        }

        const int signo = static_cast<int>(fdsi.ssi_signo);

        TransmitterData data{};
        TransmitterEventType type{};

        if (signo == EVENT_1_INT_SIGNAL) {
            data = createIntData(fdsi.ssi_int);
            type = TYPE_1;
        } else if (signo == EVENT_1_PTR_SIGNAL) {
            data = createPtrData(fdsi.ssi_ptr);
            type = TYPE_1;
        } else if (signo == EVENT_2_INT_SIGNAL) {
            data = createIntData(fdsi.ssi_int);
            type = TYPE_2;
        } else if (signo == EVENT_2_PTR_SIGNAL) {
            data = createPtrData(fdsi.ssi_ptr);
            type = TYPE_2;
        } else if (signo == EVENT_3_INT_SIGNAL) {
            data = createIntData(fdsi.ssi_int);
            type = TYPE_3;
        } else if (signo == EVENT_3_PTR_SIGNAL) {
            data = createPtrData(fdsi.ssi_ptr);
            type = TYPE_3;
        } else if (signo == EVENT_4_INT_SIGNAL) {
            data = createIntData(fdsi.ssi_int);
            type = TYPE_4;
        } else if (signo == EVENT_4_PTR_SIGNAL) {
            data = createPtrData(fdsi.ssi_ptr);
            type = TYPE_4;
        } else {
            throw Errors::TransmitterError("Received unknown signal");
        }

        return {
                .data = data,
                .type = type
        };
    }

private:
    pthread_t _creator_tid;
    int _signal_fd{};

    void sendSignal(sigval sigval, int sig) const {
        const int code = pthread_sigqueue(_creator_tid, sig, sigval);

        if (code) {
            throw Errors::TransmitterError(Utils::cerror("Sending signal failed: pthread_sigqueue() failed", code));
        }
    }

    void intiSignalFd(sigset_t* mask) {
        const int old_errno = errno;
        _signal_fd = signalfd(-1, mask, SFD_NONBLOCK);
        const int current_errno = errno;
        errno = old_errno;

        if (_signal_fd == -1) {
            throw Errors::TransmitterError(Utils::cerror("Cannot open fd for signals: signalfd() failed", current_errno));
        }
    }

    static int eventTypeToSignalInt(TransmitterEventType type) {
        if (type == TYPE_1) {
            return EVENT_1_INT_SIGNAL;
        } else if (type == TYPE_2) {
            return EVENT_2_INT_SIGNAL;
        } else if (type == TYPE_3) {
            return EVENT_3_INT_SIGNAL;
        } else {
            return EVENT_4_INT_SIGNAL;
        }
    }

    static int eventTypeToSignalPtr(TransmitterEventType type) {
        if (type == TYPE_1) {
            return EVENT_1_PTR_SIGNAL;
        } else if (type == TYPE_2) {
            return EVENT_2_PTR_SIGNAL;
        } else if (type == TYPE_3) {
            return EVENT_3_PTR_SIGNAL;
        } else {
            return EVENT_4_PTR_SIGNAL;
        }
    }

    static TransmitterData createIntData(int value) {
        return {
                .value = value,
                .ptr = nullptr
        };
    }

    static TransmitterData createPtrData(uint64_t ptr) {
        return {
                .value = 0,
                .ptr = reinterpret_cast<void*>(ptr)
        };
    }

    static void initEmptySigSet(sigset_t* set) {
        const int old_errno = errno;
        const int code = sigemptyset(set);
        const int current_errno = errno;
        errno = old_errno;

        if (code) {
            throw Errors::TransmitterError(Utils::cerror("Cannot init empty sig-set: sigemptyset() failed", current_errno));
        }
    }

    static void addSignalToSigSet(sigset_t* set, int signo) {
        const int old_errno = errno;
        const int code = sigaddset(set, signo);
        const int current_errno = errno;
        errno = old_errno;

        if (code) {
            throw Errors::TransmitterError(Utils::cerror("Cannot add signal to sig-set: sigaddset() failed", current_errno));
        }
    }

    static void setSigMask(int how, sigset_t* new_mask, sigset_t* old_mask) {
        const int code = pthread_sigmask(how, new_mask, old_mask);

        if (code) {
            throw Errors::TransmitterError(Utils::cerror("Cannot set signals-mask: pthread_sigmask() failed", code));
        }
    }
};

Utils::Transmitter::Transmitter() {
    _pimpl = new Impl();
    setSocketFd(_pimpl->getFd());
}

void Utils::Transmitter::sendInt(int value, Utils::TransmitterEventType type) const {
    _pimpl->sendInt(value, type);
}

void Utils::Transmitter::sendPtr(void* ptr, Utils::TransmitterEventType type) const {
    _pimpl->sendPtr(ptr, type);
}

Utils::TransmitterEvent Utils::Transmitter::receive() const {
    return _pimpl->receive();
}

Utils::Transmitter::~Transmitter() {
    delete _pimpl;
}
