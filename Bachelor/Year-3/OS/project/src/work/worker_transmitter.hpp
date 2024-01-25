#pragma once

#include "../utils/socket_like.hpp"
#include "../utils/abstract_error.hpp"
#include "../networking/connection.hpp"

namespace Errors {
    class WorkerTransmitterError : public AbstractError {
    public:
        explicit WorkerTransmitterError(const char* string) : AbstractError(string) {}

        explicit WorkerTransmitterError(const std::string& string) : AbstractError(string) {}

        explicit WorkerTransmitterError(const std::runtime_error& error) : AbstractError(error) {}

        explicit WorkerTransmitterError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit WorkerTransmitterError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Work {
    enum WorkerTransmitterEventType {
        CACHE_RECORD_FINISHED,
        CACHE_RECORD_DATA,
        NEW_CONNECTION
    };

    struct WorkerTransmitterEventData {
        Networking::Connection* connection;
        const std::string url;
    };

    struct WorkerTransmitterEvent {
        WorkerTransmitterEventData data;
        WorkerTransmitterEventType type{};
    };


    class WorkerTransmitter : public Utils::SocketLike {
    public:
        WorkerTransmitter();

        void sendCacheRecordFinished(const std::string& url) const;

        void sendConnection(Networking::Connection* connection) const;

        void sendCacheRecordNewData(const std::string& url) const;

        [[nodiscard]] WorkerTransmitterEvent receive() const;

        ~WorkerTransmitter() override;

    private:
        class Impl;
        Impl* _pimpl;
    };
}