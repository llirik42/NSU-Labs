#include "mutex.hpp"
#include "cerror.hpp"
#include <pthread.h>

class Utils::Mutex::Impl {
public:
    explicit Impl(MutexKind kind) {
        const std::string message = "Cannot init mutex";

        pthread_mutexattr_t mutex_attr;
        const int code1 = pthread_mutexattr_init(&mutex_attr);
        if (code1) {
            throw Errors::MutexError(cerror(message + ": pthread_mutexattr_init() failed", code1));
        }

        int code2;
        if (kind == FAST) {
            code2 = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_FAST_NP);
        } else if (kind == ERROR_CHECKING) {
            code2 = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK_NP);
        } else {
            code2 = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
        }

        if (code2) {
            pthread_mutexattr_destroy(&mutex_attr);
            throw Errors::MutexError(cerror(message + ": pthread_mutexattr_settype() failed", code2));
        }

        const int code3 = pthread_mutex_init(&_mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);

        if (code3) {
            throw Errors::MutexError(cerror(message + ": pthread_mutex_init() failed", code3));
        }
    }

    void lock() {
        const int code = pthread_mutex_lock(&_mutex);

        if (code) {
            throw Errors::MutexError(cerror("Cannot lock mutex: pthread_mutex_lock() failed", code));
        }
    }

    void unlock() {
        const int code = pthread_mutex_unlock(&_mutex);

        if (code) {
            throw Errors::MutexError(cerror("Cannot unlock mutex: pthread_mutex_unlock() failed", code));
        }
    }

    ~Impl() {
        pthread_mutex_destroy(&_mutex);
    }

private:
    pthread_mutex_t _mutex{};
};

Utils::Mutex::Mutex(MutexKind kind) {
    _pimpl = new Impl(kind);
}

void Utils::Mutex::lock() {
    _pimpl->lock();
}

void Utils::Mutex::unlock() {
    _pimpl->unlock();
}

Utils::Mutex::~Mutex() {
    delete _pimpl;
}
