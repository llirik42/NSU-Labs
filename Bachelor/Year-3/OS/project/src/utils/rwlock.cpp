#include "rwlock.hpp"
#include "cerror.hpp"
#include <pthread.h>

class Utils::RWLock::Impl {
public:
    Impl() {
        const int code = pthread_rwlock_init(&_rwlock, nullptr);

        if (code) {
            throw Errors::RWLockError(cerror("Cannot init RWlock : pthread_rwlock_init() failed", code));
        }

    }

    void readLock() {
        const int code = pthread_rwlock_rdlock(&_rwlock);

        if (code) {
            throw Errors::RWLockError(cerror("Cannot lock RWlock for read: pthread_rwlock_rdlock() failed", code));
        }
    }

    void writeLock() {
        const int code = pthread_rwlock_wrlock(&_rwlock);

        if (code) {
            throw Errors::RWLockError(cerror("Cannot lock RWLock for write: pthread_rwlock_wrlock() failed", code));
        }
    }

    void unlock() {
        const int code = pthread_rwlock_unlock(&_rwlock);

        if (code) {
            throw Errors::RWLockError(cerror("Cannot unlock RWLock: pthread_rwlock_unlock() failed", code));
        }
    }

    ~Impl() {
        pthread_rwlock_destroy(&_rwlock);
    }

private:
    pthread_rwlock_t _rwlock{};
};

Utils::RWLock::RWLock() {
    _pimpl = new Impl();
}

void Utils::RWLock::readLock() {
    _pimpl->readLock();
}

void Utils::RWLock::writeLock() {
    _pimpl->writeLock();
}

void Utils::RWLock::unlock() {
    _pimpl->unlock();
}

Utils::RWLock::~RWLock() {
    delete _pimpl;
}
