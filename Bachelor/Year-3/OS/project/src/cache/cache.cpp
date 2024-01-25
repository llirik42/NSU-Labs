#include "cache.hpp"
#include "../utils/rwlock.hpp"
#include <map>
#include <set>

struct CacheRecord {
    std::string data;
    bool is_finished = false;
    bool is_removed = false;
    size_t bound_clients_count = 0;
};

class Cache::Cache::Impl {
public:
    Impl() {
        try {
            _rwlock = new Utils::RWLock();
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Cannot init cache", error);
        }
    }

    void removeListener(CacheListener* listener) {
        try {
            _rwlock->writeLock();

            for (auto& [url, _listeners_of_url] : _listeners) {
                _listeners_of_url.erase(listener);
            }

            _rwlock->unlock();
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method removeListener() failed", error);
        }
    }

    void unbind(const std::string& url, CacheListener* listener) {
        try {
            _rwlock->writeLock();

            _data[url].bound_clients_count--;
            if (_data[url].bound_clients_count == 0 && _data[url].is_removed) {
                _data.erase(url);
                _listeners.erase(url);
            }

            _listeners[url].erase(listener);

            _rwlock->unlock();
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method unbind() failed", error);
        }
    }

    bool initRecordIfNotExistsAndBind(const std::string& url, CacheListener* listener) {
        try {
            _rwlock->writeLock();

            bool ret;
            if (_data.count(url) == 0) {
                _data[url] = {};
                _listeners[url] = {};
                ret = true;
            } else {
                ret = false;
            }

            _data[url].bound_clients_count++;
            _listeners[url].insert(listener);

            _rwlock->unlock();

            return ret;
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method initRecordIfNotExists() failed", error);
        }
    }

    void pushDataToRecord(const std::string& url, const char* data, size_t length) {
        const std::string additional_data = std::string(data, length);

        try {
            _rwlock->writeLock();

            _data[url].data.append(additional_data);
            for (const auto& it: _listeners[url]) {
                it->onRecordData(url);
            }

            _rwlock->unlock();
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method pushDataToRecord() failed", error);
        }
    }

    void markRecordFinished(const std::string& url) {
        try {
            _rwlock->writeLock();

            _data[url].is_finished = true;
            for (const auto& it: _listeners[url]) {
                it->onRecordFinished(url);
            }

            _rwlock->unlock();
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method markRecordFinished() failed", error);
        }
    }

    void removeRecord(const std::string& url) {
        try {
            _rwlock->writeLock();

            if (_data[url].bound_clients_count == 0) {
                _data.erase(url);
            } else {
                _data[url].is_removed = true;
            }

            _rwlock->unlock();
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method removeRecord() failed", error);
        }
    }

    bool isRecordFinished(const std::string& url) {
        try {
            _rwlock->readLock();
            const bool ret = _data[url].is_finished;
            _rwlock->unlock();

            return ret;
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method isRecordFinished() failed", error);
        }
    }

    size_t getRecordLength(const std::string& url) {
        try {
            _rwlock->readLock();
            const size_t ret = _data[url].data.size();
            _rwlock->unlock();

            return ret;
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method getRecordLength() failed", error);
        }
    }

    size_t getBoundCount(const std::string& url) {
        try {
            _rwlock->readLock();

            size_t ret;
            if (_data.count(url) == 0) {
                ret = 0;
            } else {
                ret = _data[url].bound_clients_count;
            }

            _rwlock->unlock();

            return ret;
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method getBoundCount() failed", error);
        }
    }

    std::string getRecordData(const std::string& url, size_t index) {
        try {
            _rwlock->readLock();
            const std::string ret = _data[url].data.substr(index);
            _rwlock->unlock();

            return ret;
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method getRecordData() failed", error);
        }
    }

    std::string getRecordData(const std::string& url) {
        try {
            _rwlock->readLock();
            const std::string ret = _data[url].data;
            _rwlock->unlock();

            return ret;
        } catch (const Errors::RWLockError& error) {
            throw Errors::CacheError("Method getRecordData() failed", error);
        }
    }

    ~Impl() {
        delete _rwlock;
    }

private:
    Utils::RWLock* _rwlock;
    std::map<std::string, std::set<CacheListener*>> _listeners;
    std::map<std::string, CacheRecord> _data;
    // TODO: implement time of cache-life
};

Cache::Cache::Cache() {
    _pimpl = new Impl();
}

void Cache::Cache::removeListener(CacheListener* listener) {
    _pimpl->removeListener(listener);
}

void Cache::Cache::unbind(const std::string& url, CacheListener* listener) {
    _pimpl->unbind(url, listener);
}

bool Cache::Cache::initRecordIfNotExistsAndBind(const std::string& url, CacheListener* listener) {
    return _pimpl->initRecordIfNotExistsAndBind(url, listener);
}

void Cache::Cache::pushDataToRecord(const std::string& url, const char* data, size_t length) {
    _pimpl->pushDataToRecord(url, data, length);
}

void Cache::Cache::markRecordFinished(const std::string& url) {
    _pimpl->markRecordFinished(url);
}

void Cache::Cache::removeRecord(const std::string& url) {
    _pimpl->removeRecord(url);
}

bool Cache::Cache::isRecordFinished(const std::string& url) {
    return _pimpl->isRecordFinished(url);
}

size_t Cache::Cache::getRecordLength(const std::string& url) {
    return _pimpl->getRecordLength(url);
}

size_t Cache::Cache::getBoundCount(const std::string& url) {
    return _pimpl->getBoundCount(url);
}

std::string Cache::Cache::getRecordData(const std::string& url) {
    return _pimpl->getRecordData(url);
}

std::string Cache::Cache::getRecordData(const std::string& url, size_t index) {
    return _pimpl->getRecordData(url, index);
}

Cache::Cache::~Cache() {
    delete _pimpl;
}
