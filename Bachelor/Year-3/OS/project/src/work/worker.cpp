#include "worker.hpp"
#include "context.hpp"
#include "worker_transmitter.hpp"
#include "../utils/thread.hpp"
#include "../utils/socket_like_selector.hpp"
#include "../http/http_errors.hpp"
#include "../http/http_request_parser.hpp"
#include "../http/request_analyzer.hpp"
#include "../http/http_utils.hpp"
#include "../http/http_response_creator.hpp"
#include "../http/http_response_parser.hpp"
#include <map>
#include <algorithm>

constexpr size_t RECV_BUFFER_LENGTH = 32 * 1024 * 1024;
constexpr size_t MAX_REQUEST_LENGTH = 1024 * 1024 * 256;

class Work::Worker::Impl {
public:
    explicit Impl(Worker* worker, WorkerListener* listener, Cache::Cache* cache,
                  const std::shared_ptr<Logging::Logger>& worker_logger,
                  const std::shared_ptr<Logging::Logger>& thread_pool_logger) {
        try {
            _recv_buffer = new char[RECV_BUFFER_LENGTH];
            _worker = worker;
            _listener = listener;
            _worker_logger = worker_logger;
            _thread_pool_logger = thread_pool_logger;
            _selector = new Utils::SocketLikeSelector();
            _transmitter = nullptr;
            _cache = cache;
            _thread = new Utils::Thread(threadRoutine, this);

            logInfo("Worker is ready!");
            notifyListenerAboutReady();
        } catch (const Errors::AbstractError& error) {
            throw Errors::WorkerError("Cannot init worker", error);
        }
    }

    void addClientToHandle(Networking::Connection* client) {
        try {
            _transmitter->sendConnection(client);
        } catch (const Errors::AbstractError& error) {
            throw Errors::WorkerError("Sending event of new-connection failed");
        }
    }

    void onRecordData(const std::string& url) {
        try {
            _transmitter->sendCacheRecordNewData(url);
        } catch (const Errors::AbstractError& error) {
            throw Errors::WorkerError("Sending event of cache-record-new-data failed");
        }
    }

    void onRecordFinished(const std::string& url) {
        try {
            _transmitter->sendCacheRecordFinished(url);
        } catch (const Errors::AbstractError& error) {
            throw Errors::WorkerError("Sending event of cache-record-finished failed");
        }
    }

    ~Impl() {
        _thread->cancel();
        delete _thread;
        delete _recv_buffer;
        delete _selector;
    }

private:
    size_t _recv_count{};
    char* _recv_buffer;

    Worker* _worker;
    WorkerListener* _listener;

    std::shared_ptr<Logging::Logger> _worker_logger;
    std::shared_ptr<Logging::Logger> _thread_pool_logger;

    Utils::SocketLikeSelector* _selector;
    Cache::Cache* _cache;
    WorkerTransmitter* _transmitter;
    HTTP::RequestParser _request_parser;
    HTTP::RequestAnalyzer _request_analyzer;
    HTTP::ResponseCreator _response_creator;
    HTTP::ResponseParser _response_parser;

    std::map<Networking::Connection*, Work::DestinationContext*> _destinations;
    std::map<Networking::Connection*, Work::ClientContext*> _clients;
    std::map<std::string, std::vector<ClientContext*>> _bound_clients;

    Utils::Thread* _thread;


    static std::string getClientRepr(const Networking::Connection* connection) {
        return "client(" + connection->repr() + ")";
    }

    static std::string getClientRepr(const ClientContext* ctx) {
        return getClientRepr(ctx->getConnection());
    }

    static std::string getClientReprCapitalized(const Networking::Connection* connection) {
        return "Client(" + connection->repr() + ")";
    }

    static std::string getClientReprCapitalized(const ClientContext* ctx) {
        return getClientReprCapitalized(ctx->getConnection());
    }

    static std::string getDestinationRepr(const Networking::Connection* connection) {
        return "destination(" + connection->repr() + ")";
    }

    static std::string getDestinationRepr(const DestinationContext* ctx) {
        return getDestinationRepr(ctx->getConnection());
    }

    static std::string getDestinationReprCapitalized(const Networking::Connection* connection) {
        return "Destination(" + connection->repr() + ")";
    }

    static std::string getDestinationReprCapitalized(const DestinationContext* ctx) {
        return getDestinationReprCapitalized(ctx->getConnection());
    }


    void notifyListenerAboutReady() {
        _listener->onWorkerReady(_worker);
    }

    void notifyListenerAboutError(const std::string& message, const Errors::AbstractError& error) {
        _listener->onWorkerError(_worker, message, error);
    }


    void logInfo(const std::string& message) {
        try {
            _worker_logger->logInfo(message);
        } catch (const Errors::LoggingError& error) {
            notifyListenerAboutError("Logging failed", error);
        }
    }

    void logInfo(const std::string& message, const std::shared_ptr<Logging::Logger>& logger) {
        try {
            logger->logInfo(message);
        } catch (const Errors::LoggingError& error) {
            notifyListenerAboutError("Logging failed", error);
        }
    }

    void logInfo(const std::string& message, const Errors::AbstractError& error) {
        try {
            _worker_logger->logInfo(message, error);
        } catch (const Errors::LoggingError& error) {
            notifyListenerAboutError("Logging failed", error);
        }
    }

    void logError(const std::string& message) {
        try {
            _worker_logger->logError(message);
        } catch (const Errors::LoggingError& error) {
            notifyListenerAboutError("Logging failed", error);
        }
    }

    void logError(const std::string& message, const Errors::AbstractError& error) {
        try {
            _worker_logger->logError(message, error);
        } catch (const Errors::LoggingError& error) {
            notifyListenerAboutError("Logging failed", error);
        }
    }


    void changeStateToReadingFromCache(ClientContext* ctx) {
        ctx->changeStateToReadingFromCache();
        logInfo(getClientReprCapitalized(ctx) + " now is \"Reading-From-Cache\"");
    }

    void changeStateToBypassingCache(ClientContext* ctx) {
        ctx->changeStateToBypassingCache();
        logInfo(getClientReprCapitalized(ctx) + " now is \"Bypassing-Cache\"");
    }

    void changeStateToSentOkResponse(DestinationContext* ctx) {
        ctx->changeStateToSentOkResponse();
        logInfo(getDestinationReprCapitalized(ctx) + " now is \"Sent-Ok-Response\"");
    }

    void changeStateToSentNotOkResponse(DestinationContext* ctx) {
        ctx->changeStateToSentNotOkResponse();
        logInfo(getDestinationReprCapitalized(ctx) + " now is \"Sent-Not-Ok-Response\"");
    }


    void addConnectionForReading(Networking::Connection* connection) {
        try {
            _selector->addForReading(connection);
        } catch (const Errors::SocketLikeSelectorError& error) {
            const std::string message = "Adding connection " + connection->repr() + " for reading failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    void addConnectionForWriting(Networking::Connection* connection) {
        try {
            _selector->addForWriting(connection);
        } catch (const Errors::SocketLikeSelectorError& error) {
            const std::string message = "Adding connection " + connection->repr() + " for writing failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    void deleteConnectionForEverything(Networking::Connection* connection) {
        try {
            _selector->deleteForEverything(connection);
        } catch (const Errors::SocketLikeSelectorError& error) {
            const std::string message = "Deleting connection " + connection->repr() + " for everything failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    void deleteConnectionForWriting(Networking::Connection* connection) {
        try {
            _selector->deleteForWriting(connection);
        } catch (const Errors::SocketLikeSelectorError& error) {
            const std::string message = "Deleting connection " + connection->repr() + " for writing failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }


    std::vector<ClientContext*> getBoundClients(const std::string& url) {
        return _bound_clients.find(url) != _bound_clients.end() ? _bound_clients.at(url)
                                                                : std::vector<ClientContext*>();
    }

    void addClientToBound(ClientContext* ctx, const std::string& url) {
        auto value = getBoundClients(url);
        value.push_back(ctx);
        _bound_clients[url] = value;
        logInfo(getClientReprCapitalized(ctx) + " is bound to " + url);
    }

    void removeClientFromBound(ClientContext* ctx, const std::string& url) {
        auto value = getBoundClients(url);

        for (auto it = value.begin(); it != value.end(); it++) {
            if (*it.base() == ctx) {
                value.erase(it);
                logInfo(getClientReprCapitalized(ctx) + " isn't bound to " + url + " anymore");
                break;
            }
        }

        if (value.empty()) {
            _bound_clients.erase(url);
        } else {
            _bound_clients[url] = value;
        }
    }

    void unbindClientFromCacheRecord(ClientContext* ctx, const std::string& url) {
        try {
            _cache->unbind(url, _worker);
            removeClientFromBound(ctx, url);
        } catch (const Errors::CacheError& error) {
            const std::string message = "Unbinding client to cache-record of " + url + " failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    bool initCacheRecordIfNotExistsAndBind(const std::string& url, ClientContext* ctx) {
        try {
            const bool ret = _cache->initRecordIfNotExistsAndBind(url, _worker);

            if (ret) {
                logInfo("Record of " + url + " is inited");
            }

            addClientToBound(ctx, url);

            return ret;
        } catch (const Errors::CacheError& error) {
            const std::string message = "Checking if record with url exists (and if not - create) failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
            return false;
        }
    }

    void pushDataToCacheRecord(const std::string& url, const char* data, size_t length) {
        try {
            _cache->pushDataToRecord(url, data, length);
            logInfo(std::to_string(length) + " bytes are pushed to record of " + url);
        } catch (const Errors::CacheError& error) {
            const std::string message = "Pushing data to cache failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    void markCacheRecordFinished(const std::string& url) {
        try {
            _cache->markRecordFinished(url);
            logInfo("Record of " + url + " is marked \"Finished\"");
        } catch (const Errors::CacheError& error) {
            const std::string message = "Marking record \"Finished\" failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    void markCacheRecordForRemoving(const std::string& url) {
        try {
            _cache->removeRecord(url);
            logInfo("Record of " + url + " is marked for removing");
        } catch (const Errors::CacheError& error) {
            const std::string message = "Removing record failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    bool isCacheRecordFinished(const std::string& url) {
        try {
            return _cache->isRecordFinished(url);
        } catch (const Errors::CacheError& error) {
            const std::string message = "Checking if record is finished failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
            return false;
        }
    }

    size_t getCacheRecordLength(const std::string& url) {
        try {
            return _cache->getRecordLength(url);
        } catch (const Errors::CacheError& error) {
            const std::string message = "Getting record length failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
            return 0;
        }
    }

    size_t getCacheRecordBoundClientsCount(const std::string& url) {
        try {
            return _cache->getBoundCount(url);
        } catch (const Errors::CacheError& error) {
            const std::string message = "Getting count of clients bound to record " + url;
            logError(message, error);
            notifyListenerAboutError(message, error);
            return 0;
        }
    }

    std::string getCacheRecordData(const std::string& url) {
        try {
            return _cache->getRecordData(url);
        } catch (const Errors::CacheError& error) {
            const std::string message = "Getting record data failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
            return "ERROR";
        }
    }

    std::string getCacheRecordData(const std::string& url, size_t index) {
        try {
            return _cache->getRecordData(url, index);
        } catch (const Errors::CacheError& error) {
            const std::string message = "Getting record data failed";
            logError(message, error);
            notifyListenerAboutError(message, error);
            return "ERROR";
        }
    }


    ClientContext* findClientContext(Networking::Connection* client) {
        return _clients.at(client);
    }

    DestinationContext* findDestinationContext(Networking::Connection* destination) {
        return _destinations.at(destination);
    }

    void setClientContext(Networking::Connection* client, ClientContext* ctx) {
        _clients[client] = ctx;
    }

    void setDestinationContext(Networking::Connection* destination, DestinationContext* ctx) {
        _destinations[destination] = ctx;
    }

    void removeClient(Networking::Connection* client) {
        _clients.erase(client);
    }

    void removeDestination(Networking::Connection* destination) {
        _destinations.erase(destination);
    }

    [[nodiscard]] bool isClient(Networking::Connection* connection) {
        return _clients.find(connection) != _clients.end();
    }

    [[nodiscard]] bool isDestination(Networking::Connection* connection) {
        return _destinations.find(connection) != _destinations.end();
    }

    [[nodiscard]] bool isTransmitter(Utils::SocketLike* socket_like) const {
        return socket_like->getSocketFd() == _transmitter->getSocketFd();
    }


    void pushDataToClient(ClientContext* ctx, const char* data, size_t length) {
        ctx->pushDataFromCache(data, length);

        if (length > 0) {
            addConnectionForWriting(ctx->getConnection());
        }
    }

    void pushDataToClient(ClientContext* ctx, const std::string& data) {
        pushDataToClient(ctx, data.c_str(), data.size());
    }


    void receive(Networking::Connection* connection, bool receive_from_client) {
        _recv_count = connection->recv(_recv_buffer, RECV_BUFFER_LENGTH);
        const std::string short_data_repr = HTTP::getShortReprOfHTTPData(_recv_buffer, _recv_count);

        const std::string sender_repr = receive_from_client ? getClientRepr(connection) : getDestinationRepr(
                connection);
        logInfo("Received " + std::to_string(_recv_count) + " bytes from " + sender_repr + " \"" + short_data_repr +
                "\"");
    }

    void receiveFromClient(ClientContext* ctx) {
        Networking::Connection* connection = ctx->getConnection();
        receive(connection, true);
    }

    void receiveFromDestination(DestinationContext* ctx) {
        Networking::Connection* connection = ctx->getConnection();
        receive(connection, false);
    }


    size_t send(Networking::Connection* connection, const std::string& data, bool send_to_client) {
        const size_t sent_count = connection->send(data.c_str(), data.size());
        const std::string short_data_repr = HTTP::getShortReprOfHTTPData(data.c_str(), sent_count);

        const std::string receiver_repr = send_to_client ? getClientRepr(connection) : getDestinationRepr(connection);
        logInfo("Sent " + std::to_string(sent_count) + " bytes to " + receiver_repr + " \"" + short_data_repr + "\"");

        if (data.size() == sent_count) {
            deleteConnectionForWriting(connection);
        }

        return sent_count;
    }

    void sendToClient(ClientContext* ctx) {
        size_t sent_count = send(ctx->getConnection(), ctx->getDataFromCache(), true);
        ctx->trimDataFromCache(sent_count);

        if (!ctx->noDataFromCache()) {
            return;
        }

        if (ctx->isReadCacheToTheEnd() && ctx->isReadingFromCache() || ctx->isBypassingCache()) {
            forgetClient(ctx);
        }
    }

    void sendToDestination(DestinationContext* ctx) {
        const size_t sent_count = send(ctx->getConnection(), ctx->getDataFromClient(), false);
        ctx->trimDataFromClient(sent_count);
    }


    void forgetConnection(Networking::Connection* connection, bool log_to_thread_pool_logger) {
        const std::string connection_repr = connection->repr();
        logInfo("Forgetting connection " + connection_repr + " ...");
        deleteConnectionForEverything(connection);
        delete connection;

        if (log_to_thread_pool_logger) {
            logInfo("Proxy closed connection " + connection_repr, _thread_pool_logger);
        }
    }

    void forgetClientConnection(Networking::Connection* connection) {
        forgetConnection(connection, true);
    }

    void forgetDestinationConnection(Networking::Connection* connection) {
        forgetConnection(connection, false);
    }

    void forgetDownloadingDestination(const std::string& url) {
        DestinationContext* downloading_destination = nullptr;

        for (const auto& [destination, destination_ctx] : _destinations) {
            if (destination_ctx->getURL() == url) {
                downloading_destination = destination_ctx;
            }
        }

        if (downloading_destination != nullptr) {
            forgetDestination(downloading_destination, true);
        }
    }

    void forgetClient(ClientContext* ctx) {
        const std::string& url = ctx->getURL();
        Networking::Connection* connection = ctx->getConnection();
        const std::string client_repr = getClientRepr(connection);
        logInfo("Forgetting " + client_repr + " ...");
        unbindClientFromCacheRecord(ctx, url);
        removeClient(connection);
        forgetClientConnection(connection);

        if (getCacheRecordBoundClientsCount(url) == 0) {
            forgetDownloadingDestination(url);
        }

        delete ctx;
        logInfo("Forgetting " + client_repr + " succeeded");
    }

    void forgetDestination(DestinationContext* ctx, bool forced) {
        if (!forced) {
            markCacheRecordFinished(ctx->getURL());
        }

        Networking::Connection* connection = ctx->getConnection();
        const std::string destination_repr = getDestinationRepr(connection);
        logInfo("Forgetting " + destination_repr + " ...");

        removeDestination(connection);
        forgetDestinationConnection(connection);

        if (ctx->isSentNotOkResponse() || forced) {
            markCacheRecordForRemoving(ctx->getURL());
        }

        delete ctx;

        logInfo("Forgetting " + destination_repr + " succeeded");
    }


    bool establishDestinationConnection(const std::string& url, const HTTP::Request& request) {
        HTTP::Host host;

        try {
            host = _request_analyzer.parseHost(request);

            // Trying to establish client with destination
            uint16_t port = host.port;
            const char* address = host.host_name.c_str();
            const Utils::SocketInfo destination_info = Utils::openDestinationSocket(port, address);
            logInfo("Established connection with " + std::string(destination_info.address));

            // Handle new destination
            auto destination = new Networking::Connection(destination_info);
            auto destination_ctx = new DestinationContext(destination);
            setDestinationContext(destination, destination_ctx);
            destination_ctx->setURL(url);

            addConnectionForReading(destination);
            addConnectionForWriting(destination);

            // Transform request to HTTP/1.0
            const HTTP::Request transformedRequest = transformToHTTP1Dot0Request(request);
            destination_ctx->pushDataFromClient(requestToString(transformedRequest));
            return true;
        } catch (const Errors::NetworkError& error) {
            const std::string host_repr = host.host_name + ":" + std::to_string(static_cast<unsigned int>(host.port));
            logInfo("Failed to establish destination connection with " + host_repr, error);
            return false;
        }
    }

    void handleEstablishingConnectionFailed(const std::string& url) {
        const std::string data = _response_creator.createResponseToUnknownDestination();
        pushDataToCacheRecord(url, data.c_str(), data.size());
        markCacheRecordFinished(url);
        markCacheRecordForRemoving(url);
    }


    void handleReadableSendingRequestHeadersClient(ClientContext* ctx) {
        const std::string recv_data = std::string(_recv_buffer, _recv_count);
        const long index = HTTP::findIndexOfFirstCharAfterHeaders(ctx->getDataForDestination(), recv_data);

        // Client didn't complete request (just push data to buffer)
        if (index == -1) {
            ctx->pushDataForDestination(recv_data);
            return;
        }

        Networking::Connection* client = ctx->getConnection();
        const std::string part_of_headers_of_recv_data = std::string(_recv_buffer, static_cast<size_t>(index));
        const std::string headers_data = ctx->getDataForDestination() + part_of_headers_of_recv_data;

        try {
            // Parsing request and host from it
            const HTTP::Request request = _request_parser.parse(headers_data);
            const bool is_get_request = _request_analyzer.isGetRequest(request);
            const std::string url = _request_analyzer.parseURL(request);

            if (is_get_request) {
                ctx->setURL(url);
                changeStateToReadingFromCache(ctx);

                const bool inited_record = initCacheRecordIfNotExistsAndBind(url, ctx);

                if (inited_record) {
                    // Establishing connection failed
                    if (!establishDestinationConnection(url, request)) {
                        handleEstablishingConnectionFailed(url);
                    }
                } else {
                    if (isCacheRecordFinished(url)) {
                        ctx->setCacheRecordDataLength(getCacheRecordLength(url));
                    }
                    pushDataToClient(ctx, getCacheRecordData(url));
                }
            } else {
                logInfo(getClientReprCapitalized(client) + " sent unsupported method " + request.method);
                changeStateToBypassingCache(ctx);
                pushDataToClient(ctx, _response_creator.createResponseToUnsupportedMethod());
            }
        } catch (const Errors::InvalidHTTPRequestError& error) {
            logInfo(getClientReprCapitalized(client) + " sent invalid HTTP-request");
            changeStateToBypassingCache(ctx);
            pushDataToClient(ctx, _response_creator.createResponseToIncorrectRequest());
        }
    }

    void handleReadableReadingForCacheClient(ClientContext* ctx) {
        logInfo(getClientReprCapitalized(ctx) + " sent data after request, but it's supposed to read from cache");
        changeStateToBypassingCache(ctx);
        pushDataToClient(ctx, _response_creator.createResponseToIncorrectRequest());
    }


    void handleReadableSendingResponseHeadersDestination(DestinationContext* ctx) {
        const std::string url = ctx->getURL();
        const std::string cache_data = getCacheRecordData(ctx->getURL());
        const std::string recv_data = std::string(_recv_buffer, _recv_count);
        const long index = HTTP::findIndexOfFirstCharAfterHeaders(cache_data, recv_data);

        pushDataToCacheRecord(url, _recv_buffer, _recv_count);

        // Destination didn't complete response-headers
        if (index == -1) {
            return;
        }

        const std::string part_of_headers_of_recv_data = std::string(_recv_buffer, static_cast<size_t>(index));
        const std::string headers_data = cache_data + part_of_headers_of_recv_data;

        try {
            const auto response = _response_parser.parseResponse(headers_data);

            if (response.ok) {
                changeStateToSentOkResponse(ctx);
            } else {
                changeStateToSentNotOkResponse(ctx);
            }
        } catch (Errors::InvalidHTTPResponseError& error) {
            logInfo(getDestinationReprCapitalized(ctx) + " sent invalid HTTP-response", error);
            forgetDestination(ctx, false);
        }
    }

    void handleReadableSentResponseHeadersDestination(DestinationContext* ctx) {
        pushDataToCacheRecord(ctx->getURL(), _recv_buffer, _recv_count);
    }


    void handleReadableClient(Networking::Connection* client) {
        ClientContext* ctx = findClientContext(client);

        try {
            receiveFromClient(ctx);

            if (ctx->getSizeofDataForDestination() + _recv_count > MAX_REQUEST_LENGTH) {
                logInfo(getClientReprCapitalized(client) + " sent very big request/part of request");
                forgetClient(ctx);
            } else {
                if (ctx->isSendingRequestHeaders()) {
                    handleReadableSendingRequestHeadersClient(ctx);
                } else {
                    handleReadableReadingForCacheClient(ctx);
                }
            }
        } catch (const Errors::ConnectionClosedError& error) {
            logInfo(getClientReprCapitalized(client) + " closed the connection");
            forgetClient(ctx);
        } catch (const Errors::ConnectionError& error) {
            logError("Error when receiving data from " + getClientRepr(client), error);
            forgetClient(ctx);
        }
    }

    void handleReadableDestination(Networking::Connection* destination) {
        DestinationContext* ctx = findDestinationContext(destination);

        try {
            receiveFromDestination(ctx);

            if (ctx->isSendingResponseHeaders()) {
                handleReadableSendingResponseHeadersDestination(ctx);
            } else {
                handleReadableSentResponseHeadersDestination(ctx);
            }
        } catch (const Errors::ConnectionClosedError& error) {
            logInfo(getDestinationReprCapitalized(destination) + " closed the connection");
            forgetDestination(ctx, false);
        } catch (const Errors::ConnectionError& error) {
            logError("Error when receiving data from " + getDestinationRepr(destination), error);
            markCacheRecordForRemoving(ctx->getURL());
            forgetDestination(ctx, false);
        }
    }

    void handleWriteableClient(Networking::Connection* client) {
        ClientContext* ctx = findClientContext(client);

        try {
            sendToClient(ctx);
        } catch (const Errors::ConnectionError& error) {
            logError("Error when sending data to " + getClientRepr(client), error);
            forgetClient(ctx);
        }
    }

    void handleWriteableDestination(Networking::Connection* destination) {
        auto ctx = findDestinationContext(destination);

        try {
            sendToDestination(ctx);
        } catch (const Errors::ConnectionError& error) {
            logError("Error when sending data to " + getDestinationRepr(destination), error);
            forgetDestination(ctx, false);
        }
    }

    void handleExceptionalClient(Networking::Connection* client) {
        logInfo(getClientReprCapitalized(client) + " became exceptional");
        forgetClient(findClientContext(client));
    }

    void handleExceptionalDestination(Networking::Connection* destination) {
        logInfo(getDestinationReprCapitalized(destination) + " became exceptional");
        DestinationContext* ctx = findDestinationContext(destination);
        forgetDestination(ctx, false);
    }


    void handleReadableConnection(Networking::Connection* connection) {
        if (isClient(connection)) {
            handleReadableClient(connection);
        } else {
            handleReadableDestination(connection);
        }
    }

    void handleWriteableConnection(Networking::Connection* connection) {
        if (isClient(connection)) {
            handleWriteableClient(connection);
        } else {
            handleWriteableDestination(connection);
        }
    }

    void handleExceptionalConnection(Networking::Connection* connection) {
        if (isClient(connection)) {
            handleExceptionalClient(connection);
        } else {
            handleExceptionalDestination(connection);
        }
    }


    void handleCacheRecordFinished(const std::string& url) {
        const auto bound_clients = getBoundClients(url);

        if (bound_clients.empty()) {
            return;
        }

        const size_t cache_record_length = getCacheRecordLength(url);

        for (const auto& it : bound_clients) {
            it->setCacheRecordDataLength(cache_record_length);
            if (it->isReadCacheToTheEnd() && it->noDataFromCache()) {
                forgetClient(it);
            }
        }
    }

    void handleCacheRecordData(const std::string& url) {
        const auto bound_clients = getBoundClients(url);

        for (const auto& it : bound_clients) {
            const std::string new_data = getCacheRecordData(url, it->getCacheRecordIndex());
            pushDataToClient(it, new_data);

            if (it->isReadCacheToTheEnd() && it->noDataFromCache()) {
                forgetClient(it);
            }
        }
    }


    void handleReadableTransmitter() {
        try {
            const WorkerTransmitterEvent event = _transmitter->receive();

            if (event.type == CACHE_RECORD_FINISHED) {
                handleCacheRecordFinished(event.data.url);
            } else if (event.type == CACHE_RECORD_DATA) {
                handleCacheRecordData(event.data.url);
            } else {
                handleNewConnection(event.data.connection);
            }
        } catch (const Errors::WorkerTransmitterError& error) {
            const std::string message = "Cannot receive event";
            logError(message, error);
            notifyListenerAboutError(message, error);
        }
    }

    void handleExceptionalTransmitter() {
        const std::string message = "Event-Transmitter became exceptional";
        logInfo(message);
        notifyListenerAboutError(message, Errors::WorkerTransmitterError("Became exceptional"));
    }

    void handleNewConnection(Networking::Connection* connection) {
        logInfo("Adding " + getClientRepr(connection) + " to worker ... ");
        setClientContext(connection, new ClientContext(connection));
        addConnectionForReading(connection);
        logInfo("Adding " + getClientRepr(connection) + " to worker succeeded");
    }


    void handleReadable(Utils::SocketLike* socket_like) {
        if (!isTransmitter(socket_like)) {
            handleReadableConnection(dynamic_cast<Networking::Connection*>(socket_like));
        } else {
            handleReadableTransmitter();
        }
    }

    void handleWriteable(Utils::SocketLike* socket_like) {
        if (!isTransmitter(socket_like)) {
            handleWriteableConnection(dynamic_cast<Networking::Connection*>(socket_like));
        } else {
            // Impossible case
            const std::string message = "Transmitter became writable";
            logError(message);
            notifyListenerAboutError(message, Errors::AbstractError("..."));
        }
    }

    void handleExceptional(Utils::SocketLike* socket_like) {
        if (!isTransmitter(socket_like)) {
            handleExceptionalConnection(dynamic_cast<Networking::Connection*>(socket_like));
        } else {
            handleExceptionalTransmitter();
        }
    }


    static void* threadRoutine(void* arg) {
        auto* pimpl = static_cast<Impl*>(arg);

        // We need to create transmitter here so signals will be sent exactly wo worker-thread (not thread that creates worker)
        try {
            pimpl->_transmitter = new WorkerTransmitter();
            pimpl->_selector->addForReading(pimpl->_transmitter);
            pimpl->_selector->addForErrors(pimpl->_transmitter);
        } catch (const Errors::AbstractError& error) {
            const std::string message = "Creating transmitter failed";
            pimpl->logError(message, error);
            pimpl->notifyListenerAboutError(message, error);
        }

        while (true) {
            try {
                const auto [readable, writeable, exceptional] = pimpl->_selector->select();

                if (!exceptional.empty()) {
                    pimpl->handleExceptional(exceptional[0]);
                } else if (!writeable.empty()) {
                    pimpl->handleWriteable(writeable[0]);
                } else {
                    pimpl->handleReadable(readable[0]);
                }
            } catch (const Errors::SocketLikeSelectorError& error) {
                const std::string message = "Select failed";
                pimpl->logError(message, error);
                pimpl->notifyListenerAboutError(message, error);
            }
        }
    }
};

Work::Worker::Worker(const std::string& name, Work::WorkerListener* listener, Cache::Cache* cache,
                     const std::shared_ptr<Logging::Logger>& worker_logger,
                     const std::shared_ptr<Logging::Logger>& thread_pool_logger) {
    _name = name;
    _pimpl = new Impl(this, listener, cache, worker_logger, thread_pool_logger);
}

const std::string& Work::Worker::getName() const {
    return _name;
}

void Work::Worker::addClientToHandle(Networking::Connection* client) {
    _pimpl->addClientToHandle(client);
}

void Work::Worker::onRecordData(const std::string& url) {
    _pimpl->onRecordData(url);
}

void Work::Worker::onRecordFinished(const std::string& url) {
    _pimpl->onRecordFinished(url);
}

Work::Worker::~Worker() {
    delete _pimpl;
}
