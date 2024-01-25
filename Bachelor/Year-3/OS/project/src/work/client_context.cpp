#include "context.hpp"

enum ClientState {
    SENDING_REQUEST_HEADERS,
    READING_FROM_CACHE,
    BYPASSING_CACHE
};

class Work::ClientContext::Impl {
public:
    explicit Impl(Networking::Connection* connection) {
        _cache_record_index = 0;
        _cache_record_length = 0;
        _state = SENDING_REQUEST_HEADERS;
        _data_from_cache = {};
        _data_from_client = {};
        _connection = connection;
    }

    void changeStateToReadingFromCache() {
        _state = READING_FROM_CACHE;
    }

    void changeStateToBypassingCache() {
        _state = BYPASSING_CACHE;
    }

    void setURL(const std::string& url) {
        _url = url;
    }

    void setCacheRecordDataLength(size_t length) {
        _cache_record_length = length;
    }

    void pushDataFromCache(const char* data, size_t length) {
        _data_from_cache.append(data, length);
        _cache_record_index += length;
    }

    void pushDataFromCache(const std::string& data) {
        _data_from_cache.append(data);
        _cache_record_index += data.size();
    }

    void pushDataForDestination(const char* data, size_t length) {
        _data_from_client.append(data, length);
    }

    void pushDataForDestination(const std::string& data) {
        _data_from_client.append(data);
    }

    void trimDataFromCache(size_t n) {
        _data_from_cache = _data_from_cache.substr(n);
    }

    [[nodiscard]] bool isReadCacheToTheEnd() const {
        return _cache_record_index == _cache_record_length;
    }

    [[nodiscard]] bool isSendingRequestHeaders() const {
        return _state == SENDING_REQUEST_HEADERS;
    }

    [[nodiscard]] bool isReadingFromCache() const {
        return _state == READING_FROM_CACHE;
    }

    [[nodiscard]] bool isBypassingCache() const {
        return _state == BYPASSING_CACHE;
    }

    [[nodiscard]] bool noDataFromCache() const {
        return _data_from_cache.empty();
    }

    [[nodiscard]] bool noDataForDestination() const {
        return _data_from_client.empty();
    }

    [[nodiscard]] size_t getCacheRecordIndex() const {
        return _cache_record_index;
    }

    [[nodiscard]] size_t getSizeOfDataFromCache() const {
        return _data_from_cache.size();
    }

    [[nodiscard]] size_t getSizeofDataForDestination() const {
        return _data_from_client.size();
    }

    [[nodiscard]] size_t getCacheRecordDataLength() const {
        return _cache_record_length;
    }

    [[nodiscard]] const std::string& getDataFromCache() const {
        return _data_from_cache;
    }

    [[nodiscard]] const std::string& getDataForDestination() const {
        return _data_from_client;
    }

    [[nodiscard]] const std::string& getURL() const {
        return _url;
    }

    [[nodiscard]] Networking::Connection* getConnection() const {
        return _connection;
    }

private:
    size_t _cache_record_length;
    size_t _cache_record_index;
    std::string _url;
    HTTP::Request _request;
    std::string _body;
    ClientState _state{};
    std::string _data_from_cache;
    std::string _data_from_client;
    Networking::Connection* _connection;
};

Work::ClientContext::ClientContext(Networking::Connection* connection) {
    _pimpl = new Impl(connection);
}

void Work::ClientContext::changeStateToReadingFromCache() {
    _pimpl->changeStateToReadingFromCache();
}

void Work::ClientContext::changeStateToBypassingCache() {
    _pimpl->changeStateToBypassingCache();
}

void Work::ClientContext::pushDataFromCache(const char* data, size_t length) {
    _pimpl->pushDataFromCache(data, length);
}

void Work::ClientContext::setURL(const std::string& url) {
    _pimpl->setURL(url);
}

void Work::ClientContext::setCacheRecordDataLength(size_t length) {
    _pimpl->setCacheRecordDataLength(length);
}

void Work::ClientContext::pushDataFromCache(const std::string& data) {
    _pimpl->pushDataFromCache(data);
}

void Work::ClientContext::pushDataForDestination(const char* data, size_t length) {
    _pimpl->pushDataForDestination(data, length);
}

void Work::ClientContext::pushDataForDestination(const std::string& data) {
    _pimpl->pushDataForDestination(data);
}

void Work::ClientContext::trimDataFromCache(size_t n) {
    _pimpl->trimDataFromCache(n);
}

bool Work::ClientContext::isReadCacheToTheEnd() const {
    return _pimpl->isReadCacheToTheEnd();
}

bool Work::ClientContext::isSendingRequestHeaders() const {
    return _pimpl->isSendingRequestHeaders();
}

bool Work::ClientContext::isReadingFromCache() const {
    return _pimpl->isReadingFromCache();
}

bool Work::ClientContext::isBypassingCache() const {
    return _pimpl->isBypassingCache();
}

bool Work::ClientContext::noDataFromCache() const {
    return _pimpl->noDataFromCache();
}

bool Work::ClientContext::noDataForDestination() const {
    return _pimpl->noDataForDestination();
}

size_t Work::ClientContext::getCacheRecordIndex() const {
    return _pimpl->getCacheRecordIndex();
}

size_t Work::ClientContext::getSizeOfDataFromCache() const {
    return _pimpl->getSizeOfDataFromCache();
}

size_t Work::ClientContext::getSizeofDataForDestination() const {
    return _pimpl->getSizeofDataForDestination();
}

size_t Work::ClientContext::getCacheRecordDataLength() const {
    return 0;
}

const std::string& Work::ClientContext::getDataFromCache() const {
    return _pimpl->getDataFromCache();
}

const std::string& Work::ClientContext::getDataForDestination() const {
    return _pimpl->getDataForDestination();
}

const std::string& Work::ClientContext::getURL() const {
    return _pimpl->getURL();
}

Networking::Connection* Work::ClientContext::getConnection() const {
    return _pimpl->getConnection();
}

Work::ClientContext::~ClientContext() {
    delete _pimpl;
}
