#include "thread.hpp"
#include "cerror.hpp"
#include <pthread.h>

class Utils::Thread::Impl {
public:
    Impl(void* (* start_routine)(void*), void* arg) {
        const int code = pthread_create(&_tid, nullptr, start_routine, arg);

        if (code) {
            throw Errors::ThreadError(cerror("Cannot init thread: pthread_create() failed", code));
        }
    }

    void cancel() const {
        pthread_cancel(_tid);
    }

private:
    pthread_t _tid{};
};

Utils::Thread::Thread(void* (* start_routine)(void*), void* arg) {
    _pimpl = new Impl(start_routine, arg);
}

void Utils::Thread::cancel() {
    _pimpl->cancel();
}

Utils::Thread::~Thread() {
    delete _pimpl;
}
