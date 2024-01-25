#include "context.hpp"

enum DestinationState {
    SENDING_RESPONSE_HEADERS,
    SENT_OK_RESPONSE,
    SENT_NOT_OK_RESPONSE,
};

class Work::DestinationContext::Impl {
public:
    explicit Impl(Networking::Connection* connection) {
        _state = SENDING_RESPONSE_HEADERS;
        _connection = connection;
    }

    void changeStateToSentOkResponse() {
        _state = SENT_OK_RESPONSE;
    }

    void changeStateToSentNotOkResponse() {
        _state = SENT_NOT_OK_RESPONSE;
    }

    void pushDataFromClient(const std::string& data) {
        _data_from_client.append(data);
    }

    void setURL(const std::string& url) {
        _url = url;
    }

    void setRequest(const HTTP::Request& request) {
        _request = request;
    }

    void trimDataFromClient(size_t n) {
        _data_from_client = _data_from_client.substr(n);
    }

    [[nodiscard]] bool isSendingResponseHeaders() const {
        return _state == SENDING_RESPONSE_HEADERS;
    }

    [[nodiscard]] bool isSentOkResponse() const {
        return _state == SENT_OK_RESPONSE;
    }

    [[nodiscard]] bool isSentNotOkResponse() const {
        return _state == SENT_NOT_OK_RESPONSE;
    }

    [[nodiscard]] const std::string& getURL() const {
        return _url;
    }

    [[nodiscard]] const std::string& getDataFromClient() const {
        return _data_from_client;
    }

    [[nodiscard]] const HTTP::Request& getRequest() const {
        return _request;
    }

    [[nodiscard]] Networking::Connection* getConnection() const {
        return _connection;
    }

private:
    std::string _url;
    std::string _data_from_client;
    HTTP::Request _request{};
    DestinationState _state;
    Networking::Connection* _connection;

};

Work::DestinationContext::DestinationContext(Networking::Connection* connection) {
    _pimpl = new Impl(connection);
}

void Work::DestinationContext::changeStateToSentOkResponse() {
    _pimpl->changeStateToSentOkResponse();
}

void Work::DestinationContext::changeStateToSentNotOkResponse() {
    _pimpl->changeStateToSentNotOkResponse();
}

void Work::DestinationContext::pushDataFromClient(const std::string& data) {
    _pimpl->pushDataFromClient(data);
}

void Work::DestinationContext::setURL(const std::string& url) {
    _pimpl->setURL(url);
}

void Work::DestinationContext::setRequest(const HTTP::Request& request) {
    _pimpl->setRequest(request);
}

void Work::DestinationContext::trimDataFromClient(size_t n) {
    _pimpl->trimDataFromClient(n);
}

bool Work::DestinationContext::isSendingResponseHeaders() const {
    return _pimpl->isSendingResponseHeaders();
}

bool Work::DestinationContext::isSentOkResponse() const {
    return _pimpl->isSentOkResponse();
}

bool Work::DestinationContext::isSentNotOkResponse() const {
    return _pimpl->isSentNotOkResponse();
}

const std::string& Work::DestinationContext::getURL() const {
    return _pimpl->getURL();
}

const std::string& Work::DestinationContext::getDataFromClient() const {
    return _pimpl->getDataFromClient();
}

const HTTP::Request& Work::DestinationContext::getRequest() const {
    return _pimpl->getRequest();
}

Networking::Connection* Work::DestinationContext::getConnection() const {
    return _pimpl->getConnection();
}

Work::DestinationContext::~DestinationContext() {
    delete _pimpl;
}
