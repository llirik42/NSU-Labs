#include "http_utils.hpp"
#include <cstring>
#include <set>

const size_t LENGTH_OF_SHORT_HTTP_DATA_REPR = 50;

std::string tail(std::string const& source, size_t const length) {
    return length > source.size() ? source : source.substr(source.size() - length);
}

long HTTP::findIndexOfFirstCharAfterHeaders(const std::string& old, const std::string& cur) {
    const std::string needle = "\r\n\r\n";

    const std::string old_tail = tail(old, needle.size() - 1);
    const std::string str = old_tail + cur;
    const char* search_res = strstr(str.c_str(), needle.c_str());

    if (search_res == nullptr) {
        return -1;
    } else {
        return search_res - str.c_str() + static_cast<long>(needle.size());
    }
}

HTTP::Request HTTP::transformToHTTP1Dot0Request(const Request& request) {
    const std::set<std::string> forbiddenHeaders = {
            "Connection",
            "Proxy-Connection"
    };

    Request ret = {
            .method = request.method,
            .uri = request.uri,
            .major_version = 1,
            .minor_version = 0,
            .headers = {}
    };

    for (const auto& it: request.headers) {
        if (forbiddenHeaders.find(it.name) == forbiddenHeaders.end()) {
            ret.headers.push_back(it);
        }
    }

    return ret;
}

std::string HTTP::getShortReprOfHTTPData(const char* buffer, size_t length) {
    const char* needle = "\n";
    const char* res = strstr(buffer, needle);

    // Buffer doesn't contains "\r\n"
    if (res == nullptr) {
        if (length < LENGTH_OF_SHORT_HTTP_DATA_REPR) {
            return buffer;
        } else {
            return std::string(buffer, LENGTH_OF_SHORT_HTTP_DATA_REPR) + " ...";
        }
    }

    // Buffer contains "\r\n"
    const long i = res - buffer;
    std::string tmp = {buffer, static_cast<size_t>(i)};
    if (tmp.back() == '\r') {
        tmp.erase(tmp.size() - 1);
    }

    if (static_cast<size_t>(i) == length - strlen(needle)) {
        return tmp;
    } else {
        return tmp + " ...";
    }
}

std::string HTTP::requestToString(const Request& request) {
    std::string ret =
            request.method +
            " " +
            request.uri +
            " HTTP/" +
            std::to_string(request.major_version) +
            "." +
            std::to_string(request.minor_version) +
            "\r\n";

    for (const auto& it: request.headers) {
        ret.append(it.name + ": " + it.value + "\r\n");
    }

    return ret + "\r\n";
}
