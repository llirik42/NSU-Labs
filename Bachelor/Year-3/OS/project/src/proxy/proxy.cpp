#include "proxy.hpp"
#include "../networking/server.hpp"
#include "../work/thread_pool.hpp"

class Proxy::Proxy::Impl {
public:
    Impl(unsigned int n, uint16_t port, int backlog, const std::shared_ptr<Logging::Logger>& logger) {
        try {
            _server = new Networking::Server(port, backlog);
            _logger = logger;
            _thread_pool = new Work::ThreadPool(n, getThreadPoolLogger());
        } catch (const Errors::AbstractError& error) {
            throw Errors::ProxyError("Cannot init proxy", error);
        }
    }

    Impl(unsigned int n, uint16_t port, const char* address, int backlog,
         const std::shared_ptr<Logging::Logger>& logger) {
        try {
            _server = new Networking::Server(port, address, backlog);
            _logger = logger;
            _thread_pool = new Work::ThreadPool(n, getThreadPoolLogger());
        } catch (const Errors::AbstractError& error) {
            throw Errors::ProxyError("Cannot init proxy", error);
        }
    }

    void start() {
        logInfo("Proxy is listening on address " + _server->repr() + " with backlog of " + std::to_string(_server->getBacklog()));

        while (true) {
            Networking::Connection* connection;

            try {
                connection = _server->acceptConnection();
                _logger->logInfo("Proxy accepted new connection " + connection->repr());
                _thread_pool->addClientToHandle(connection);
            } catch (const Errors::ServerError& error) {
                logError("Proxy failed to accept new connection", error);
            } catch (const Errors::ThreadPoolError& error) {
                logError("Cannot assign client " + connection->repr() + " to thread-pool", error);
                delete connection;
            } catch (const Errors::LoggingError& error) {
                throw Errors::ProxyError("Logging failed", error);
            }
        }
    }

    ~Impl() {
        delete _server;
        delete _thread_pool;
    }

private:
    Networking::Server* _server;
    std::shared_ptr<Logging::Logger> _logger;
    Work::ThreadPool* _thread_pool;

    std::shared_ptr<Logging::Logger> getThreadPoolLogger() {
        return _logger;
    }

    void logInfo(const std::string& message) {
        try {
            _logger->logInfo(message);
        } catch (const Errors::LoggingError& error) {
            throw Errors::ProxyError("Logging failed", error);
        }
    }

    void logError(const std::string& message, const Errors::AbstractError& error) {
        try {
            _logger->logError(message, error);
        } catch (const Errors::LoggingError& error) {
            throw Errors::ProxyError("Logging failed", error);
        }
    }
};

Proxy::Proxy::Proxy(unsigned int n, uint16_t port, int backlog, const std::shared_ptr<Logging::Logger>& logger) {
    _pimpl = new Impl(n, port, backlog, logger);
}

Proxy::Proxy::Proxy(unsigned int n, uint16_t port, const char* address, int backlog,
                    const std::shared_ptr<Logging::Logger>& logger) {
    _pimpl = new Impl(n, port, address, backlog, logger);
}


void Proxy::Proxy::start() {
    _pimpl->start();
}

Proxy::Proxy::~Proxy() {
    delete _pimpl;
}
