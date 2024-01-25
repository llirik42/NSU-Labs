#pragma once

#include "../logging/logger.hpp"
#include <memory>

namespace Errors {
    class ProxyError : public AbstractError {
    public:
        explicit ProxyError(const char* string) : AbstractError(string) {}

        explicit ProxyError(const std::string& string) : AbstractError(string) {}

        explicit ProxyError(const std::runtime_error& error) : AbstractError(error) {}

        explicit ProxyError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit ProxyError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Proxy {
    class Proxy {
    public:
        Proxy(unsigned int n, uint16_t port, int backlog, const std::shared_ptr<Logging::Logger>& logger);

        Proxy(unsigned int n, uint16_t port, const char* address, int backlog,
              const std::shared_ptr<Logging::Logger>& logger);

        void start();

        ~Proxy();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
