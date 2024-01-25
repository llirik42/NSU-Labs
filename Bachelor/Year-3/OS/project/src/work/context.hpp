#pragma once

#include "../networking/connection.hpp"
#include "../http/dto.hpp"

namespace Work {
    class DestinationContext;

    class ClientContext {
    public:
        explicit ClientContext(Networking::Connection* connection);

        void changeStateToReadingFromCache();

        void changeStateToBypassingCache();

        void setURL(const std::string& url);

        void setCacheRecordDataLength(size_t length);

        void pushDataFromCache(const char* data, size_t length);

        void pushDataFromCache(const std::string& data);

        void pushDataForDestination(const char* data, size_t length);

        void pushDataForDestination(const std::string& data);

        void trimDataFromCache(size_t n);

        [[nodiscard]] bool isReadCacheToTheEnd() const;

        [[nodiscard]] bool isSendingRequestHeaders() const;

        [[nodiscard]] bool isReadingFromCache() const;

        [[nodiscard]] bool isBypassingCache() const;

        [[nodiscard]] bool noDataFromCache() const;

        [[nodiscard]] bool noDataForDestination() const;

        [[nodiscard]] size_t getCacheRecordIndex() const;

        [[nodiscard]] size_t getSizeOfDataFromCache() const;

        [[nodiscard]] size_t getSizeofDataForDestination() const;

        [[nodiscard]] size_t getCacheRecordDataLength() const;

        [[nodiscard]] const std::string& getDataFromCache() const;

        [[nodiscard]] const std::string& getDataForDestination() const;

        [[nodiscard]] const std::string& getURL() const;

        [[nodiscard]] Networking::Connection* getConnection() const;

        ~ClientContext();

    private:
        class Impl;
        Impl* _pimpl;
    };

    class DestinationContext {
    public:
        explicit DestinationContext(Networking::Connection* connection);

        void changeStateToSentOkResponse();

        void changeStateToSentNotOkResponse();

        void pushDataFromClient(const std::string& data);

        void setURL(const std::string& url);

        void setRequest(const HTTP::Request& request);

        void trimDataFromClient(size_t n);

        [[nodiscard]] bool isSendingResponseHeaders() const;

        [[nodiscard]] bool isSentOkResponse() const;

        [[nodiscard]] bool isSentNotOkResponse() const;

        [[nodiscard]] const std::string& getURL() const;

        [[nodiscard]] const std::string& getDataFromClient() const;

        [[nodiscard]] const HTTP::Request& getRequest() const;

        [[nodiscard]] Networking::Connection* getConnection() const;

        ~DestinationContext();

    private:
        class Impl;
        Impl* _pimpl;
    };
}
