#pragma once

#include "../utils/abstract_error.hpp"
#include "../logging/logger.hpp"
#include "../networking/connection.hpp"
#include "../cache/cache.hpp"
#include <memory>

namespace Errors {
    class WorkerError : public AbstractError {
    public:
        explicit WorkerError(const char* string) : AbstractError(string) {}

        explicit WorkerError(const std::string& string) : AbstractError(string) {}

        explicit WorkerError(const std::runtime_error& error) : AbstractError(error) {}

        explicit WorkerError(const std::string& message, const AbstractError& error) : AbstractError(message,
                                                                                                     error) {}

        explicit WorkerError(const char* message, const AbstractError& error) : AbstractError(message,
                                                                                              error) {}
    };
}

namespace Work {
    class Worker;

    class WorkerListener {
    public:
        virtual void onWorkerReady(Worker* worker) = 0;

        virtual void onWorkerError(Worker* worker, const std::string& message, const Errors::AbstractError& error) = 0;

        virtual ~WorkerListener() = default;
    };

    class Worker : public Cache::CacheListener {
    public:
        explicit Worker(const std::string& name, WorkerListener* listener, Cache::Cache* cache,
                        const std::shared_ptr<Logging::Logger>& worker_logger,
                        const std::shared_ptr<Logging::Logger>& thread_pool_logger);

        [[nodiscard]] const std::string& getName() const;

        void addClientToHandle(Networking::Connection* client);

        void onRecordFinished(const std::string& url) override;

        void onRecordData(const std::string& url) override;

        ~Worker() override;

    private:
        std::string _name;

        class Impl;

        Impl* _pimpl;
    };
}
