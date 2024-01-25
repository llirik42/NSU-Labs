#include "worker_transmitter.hpp"
#include "../utils/transmitter.hpp"
#include <cstring>

class Work::WorkerTransmitter::Impl {
public:
    Impl() {
        try {
            _transmitter = new Utils::Transmitter();
        } catch (const Errors::TransmitterError& error) {
            throw Errors::WorkerTransmitterError("Cannot init worker-transmitter", error);
        }
    }

    void sendCacheRecordFinished(const std::string& url) const {
        auto ptr = new char[url.size() + 1];
        ptr[url.size()] = '\0';
        strcpy(ptr, url.c_str());

        try {
            _transmitter->sendPtr(ptr, getTransmitterEventType(CACHE_RECORD_FINISHED));
        } catch (const Errors::TransmitterError& error) {
            throw Errors::WorkerTransmitterError("Cannot send cache-record-finished-event", error);
        }
    }

    void sendConnection(Networking::Connection* connection) const {
        try {
            _transmitter->sendPtr(connection, getTransmitterEventType(NEW_CONNECTION));
        } catch (const Errors::TransmitterError& error) {
            throw Errors::WorkerTransmitterError("Cannot send new-connection-event", error);
        }
    }

    void sendCacheRecordNewData(const std::string& url) const {
        auto ptr = new char[url.size() + 1];
        ptr[url.size()] = '\0';
        strcpy(ptr, url.c_str());

        try {
            _transmitter->sendPtr(ptr, getTransmitterEventType(CACHE_RECORD_DATA));
        } catch (const Errors::TransmitterError& error) {
            throw Errors::WorkerTransmitterError("Cannot send cache-record-removed-event", error);
        }
    }

    [[nodiscard]] int getSocketFd() {
        return _transmitter->getSocketFd();
    }

    [[nodiscard]] WorkerTransmitterEvent receive() const {
        try {
            Utils::TransmitterEvent res = _transmitter->receive();
            const WorkerTransmitterEventType type = getWorkerTransmitterEventType(res.type);

            if (type == CACHE_RECORD_DATA) {
                void* ptr = res.data.ptr;
                char* char_ptr = reinterpret_cast<char*>(ptr);
                const std::string url = std::string(char_ptr);

                const WorkerTransmitterEventData data = {
                        .connection = nullptr,
                        .url = url,
                };

                delete char_ptr;

                return {
                    .data = data,
                    .type = type,
                };
            }

            if (type == CACHE_RECORD_FINISHED) {
                void* ptr = res.data.ptr;
                char* char_ptr = reinterpret_cast<char*>(ptr);
                const std::string url = std::string(char_ptr);

                const WorkerTransmitterEventData data = {
                        .connection = nullptr,
                        .url = url,
                };

                delete char_ptr;

                return {
                        .data = data,
                        .type = type,
                };
            }

            // New connection event only left
            void* ptr = res.data.ptr;
            auto connection = reinterpret_cast<Networking::Connection*>(ptr);
            const WorkerTransmitterEventData data = {
                    .connection = connection,
                    .url = {},
            };

            return {
                    .data = data,
                    .type = type,
            };
        } catch (const Errors::TransmitterError& error) {
            throw Errors::WorkerTransmitterError("Cannot receive event", error);
        }
    }

    ~Impl() {
        delete _transmitter;
    }

private:
    Utils::Transmitter* _transmitter;

    static Utils::TransmitterEventType getTransmitterEventType(WorkerTransmitterEventType type) {
        if (type == CACHE_RECORD_FINISHED) {
            return Utils::TYPE_1;
        } else if (type == CACHE_RECORD_DATA) {
            return Utils::TYPE_2;
        } else {
            return Utils::TYPE_3;
        }
    }

    static WorkerTransmitterEventType getWorkerTransmitterEventType(Utils::TransmitterEventType type) {
        if (type == Utils::TYPE_1) {
            return CACHE_RECORD_FINISHED;
        } else if (type == Utils::TYPE_2) {
            return CACHE_RECORD_DATA;
        } else {
            return NEW_CONNECTION;
        }
    }
};

Work::WorkerTransmitter::WorkerTransmitter() {
    _pimpl = new Impl();
    setSocketFd(_pimpl->getSocketFd());
}

void Work::WorkerTransmitter::sendCacheRecordFinished(const std::string& url) const {
    _pimpl->sendCacheRecordFinished(url);
}

void Work::WorkerTransmitter::sendConnection(Networking::Connection* connection) const {
    _pimpl->sendConnection(connection);
}

void Work::WorkerTransmitter::sendCacheRecordNewData(const std::string& url) const {
    _pimpl->sendCacheRecordNewData(url);
}

Work::WorkerTransmitterEvent Work::WorkerTransmitter::receive() const {
    return _pimpl->receive();
}

Work::WorkerTransmitter::~WorkerTransmitter() {
    delete _pimpl;
}
