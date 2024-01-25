#include "thread_pool.hpp"
#include "worker.hpp"
#include "../utils/mutex.hpp"
#include "../logging/synchronous_logger.hpp"
#include "../logging/file_logger.hpp"

class Work::ThreadPool::Impl {
public:
    explicit Impl(ThreadPool* thread_pool, const std::shared_ptr<Logging::Logger>& logger) {
        try {
            _next_worker_index = 0;
            _thread_pool = thread_pool;
            _mutex = new Utils::Mutex(Utils::FAST);
            _cache = new Cache::Cache();
            _logger = logger;
        } catch (const Errors::AbstractError& error) {
            throw Errors::ThreadPoolError("Cannot init thread-pool", error);
        }
    }

    void createWorkers(unsigned int n) {
        try {
            for (unsigned int i = 0; i < n; i++) {
                const std::string name = std::to_string(i + 1);
                auto worker = new Worker(name, _thread_pool, _cache, createWorkerLogger(name), _logger);
                _workers.push_back(worker);
            }
        } catch (const Errors::WorkerError& error) {
            throw Errors::ThreadPoolError("Cannot init thread-pool", error);
        }
    }

    void addClientToHandle(Networking::Connection* client) {
        try {
            if (!_workers.empty()) {
                _workers[_next_worker_index]->addClientToHandle(client);
                _next_worker_index = (_next_worker_index + 1) % _workers.size();
            }
        } catch (const Errors::WorkerError& error) {
            throw Errors::ThreadPoolError("Cannot add client to handle to thread-pool", error);
        }
    }

    void onWorkerReady(const Worker* worker) {
        try {
            _logger->logInfo("Worker " + worker->getName() + " is ready");
        } catch (const Errors::AbstractError& error) {
            logError("An error occurred during handling ready worker", error);
        }
    }

    void onWorkerError(Worker* worker, const std::string& message, const Errors::AbstractError& error) {
        try {
            _logger->logError("An error occurred with worker " + worker->getName() + ": " + message, error);

            _mutex->lock();
            _cache->removeListener(worker);
            for (auto it = _workers.begin(); it != _workers.end(); it++) {
                if (*it == worker) {
                    _workers.erase(it);
                }
            }
            _mutex->unlock();

            delete worker;
        } catch (const Errors::AbstractError& err) {
            logError("An error occurred during handling error on worker", error);
        }
    }

    ~Impl() {
        for (auto& worker: _workers) {
            _cache->removeListener(worker);
            delete worker;
        }

        delete _mutex;
        delete _cache;
    }

private:
    size_t _next_worker_index;
    ThreadPool* _thread_pool;
    std::shared_ptr<Logging::Logger> _logger;
    Utils::Mutex* _mutex;
    Cache::Cache* _cache;
    std::vector<Worker*> _workers{};

    static std::shared_ptr<Logging::Logger> createWorkerLogger(const std::string& name) {
        const std::string file_path = name + ".log";
        auto tmp = std::make_shared<Logging::FileLogger>(file_path);
        return std::make_shared<Logging::SynchronousLogger>(tmp);
    }

    void logError(const std::string& message, const Errors::AbstractError& error) {
        try {
            _logger->logError(message, error);
        } catch (const Errors::AbstractError& e) {
            exit(1);
        }
    }
};

Work::ThreadPool::ThreadPool(unsigned int n, const std::shared_ptr<Logging::Logger>& logger) {
    _pimpl = new Impl(this, logger);
    _pimpl->createWorkers(n);
}

void Work::ThreadPool::addClientToHandle(Networking::Connection* client) {
    _pimpl->addClientToHandle(client);
}

void Work::ThreadPool::onWorkerReady(Worker* worker) {
    _pimpl->onWorkerReady(worker);
}

void Work::ThreadPool::onWorkerError(Worker* worker, const std::string& message, const Errors::AbstractError& error) {
    _pimpl->onWorkerError(worker, message, error);
}

Work::ThreadPool::~ThreadPool() {
    delete _pimpl;
}
