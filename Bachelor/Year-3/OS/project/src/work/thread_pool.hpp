#pragma once

#include "worker.hpp"
#include "../utils/abstract_error.hpp"
#include "../logging/logger.hpp"
#include <memory>

namespace Errors {
    class ThreadPoolError : public AbstractError {
    public:
        explicit ThreadPoolError(const char* string) : AbstractError(string) {}

        explicit ThreadPoolError(const std::string& string) : AbstractError(string) {}

        explicit ThreadPoolError(const std::runtime_error& error) : AbstractError(error) {}

        explicit ThreadPoolError(const std::string& message, const AbstractError& error) : AbstractError(message,
                                                                                                         error) {}

        explicit ThreadPoolError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Work {
    class ThreadPool : public WorkerListener {
    public:
        explicit ThreadPool(unsigned int n, const std::shared_ptr<Logging::Logger>& logger);

        void addClientToHandle(Networking::Connection* client);

        void onWorkerReady(Worker* worker) override;

        void onWorkerError(Worker* worker, const std::string& message, const Errors::AbstractError& error) override;

        ~ThreadPool() override;

    private:
        class Impl;

        Impl* _pimpl;
    };
}
