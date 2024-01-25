#include "request_analyzer.hpp"
#include "http_errors.hpp"
#include "httpparser/urlparser.h"
#include <regex>

constexpr uint16_t DEFAULT_HTTP_PORT = 80;

bool isWhiteSpace(const std::string& string) {
    return std::all_of(string.begin(), string.end(), isspace);
}

class HTTP::RequestAnalyzer::Impl {
public:
    Host parseHost(const Request& request) {
        try {
            return parseHostFromHeaders(request);
        } catch (const std::runtime_error& error) {
            return parseHostFromURI(request);
        }
    }

    std::string parseURL(const Request& request) {
        const auto host = parseHost(request);

        _parser.parse(request.uri);
        if (_parser.isValid()) {
            return _parser.hostname() + _parser.path();
        } else {
            return host.host_name + request.uri;
        }
    }

    static bool isGetRequest(const Request& request) {
        return request.method == "GET";
    }

private:
    httpparser::UrlParser _parser;

    static Host parseHostFromHeaders(const Request& request) {
        const std::string string_host = parseStringHostFromHeaders(request);
        static const std::regex host_regex(R"(^([^:/?#]+)(:[1-9][0-9]*)?$)");

        if (!std::regex_match(string_host, host_regex)) {
            throw std::runtime_error("Invalid value of header \"Host\"");
        }

        std::smatch m;
        std::regex_search(string_host, m, host_regex);

        std::string host_name = m[1];
        std::string host_port = m[2];

        if (isWhiteSpace(host_port)) {
            return {.host_name = host_name, .port = DEFAULT_HTTP_PORT};
        } else {
            host_port.erase(host_port.begin());
            const auto port = static_cast<uint16_t>(std::stoi(host_port));
            return {.host_name = host_name, .port = port};
        }
    }

    Host parseHostFromURI(const Request& request) {
        _parser.parse(request.uri);

        if (_parser.isValid()) {
            return {.host_name = _parser.hostname(), .port = _parser.httpPort()};
        }

        throw Errors::InvalidHTTPRequestError(
                std::string("Invalid URI (doesn't have information about host): ") + request.uri);
    }

    static std::string parseStringHostFromHeaders(const Request& request) {
        for (const auto& it: request.headers) {
            if (it.name == "Host") {
                return it.value;
            }
        }

        throw std::runtime_error("Header \"Host\" not found");
    }
};

HTTP::RequestAnalyzer::RequestAnalyzer() {
    _pimpl = new Impl();
}

HTTP::Host HTTP::RequestAnalyzer::parseHost(const Request& request) {
    return _pimpl->parseHost(request);
}

std::string HTTP::RequestAnalyzer::parseURL(const HTTP::Request& request) {
    return _pimpl->parseURL(request);
}

bool HTTP::RequestAnalyzer::isGetRequest(const HTTP::Request& request) {
    return _pimpl->isGetRequest(request);
}

HTTP::RequestAnalyzer::~RequestAnalyzer() {
    delete _pimpl;
}
