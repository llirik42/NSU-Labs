#pragma once

#include "../utils/abstract_error.hpp"

namespace Errors {
    class CacheError : public AbstractError {
    public:
        explicit CacheError(const char* string) : AbstractError(string) {}

        explicit CacheError(const std::string& string) : AbstractError(string) {}

        explicit CacheError(const std::runtime_error& error) : AbstractError(error) {}

        explicit CacheError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit CacheError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };
}

namespace Cache {
    class Cache;

    class CacheListener {
    public:
        virtual void onRecordData(const std::string& url) = 0;

        virtual void onRecordFinished(const std::string& url) = 0;

        virtual ~CacheListener() = default;
    };

    class Cache {
    public:
        Cache();

        void removeListener(CacheListener* listener);

        void unbind(const std::string& url, CacheListener* listener);

        bool initRecordIfNotExistsAndBind(const std::string& url, CacheListener* listener);

        void pushDataToRecord(const std::string& url, const char* data, size_t length);

        void markRecordFinished(const std::string& url);

        void removeRecord(const std::string& url);

        bool isRecordFinished(const std::string& url);

        size_t getRecordLength(const std::string& url);

        size_t getBoundCount(const std::string& url);

        std::string getRecordData(const std::string& url);

        std::string getRecordData(const std::string& url, size_t index);

        ~Cache();

    private:
        class Impl;

        Impl* _pimpl;
    };
}
