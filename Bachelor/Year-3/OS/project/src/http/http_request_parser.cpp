#include "http_request_parser.hpp"
#include "http_errors.hpp"
#include "httpparser/request.h"
#include "httpparser/httprequestparser.h"

class HTTP::RequestParser::Impl {
public:
    Request parse(const char* buffer, size_t length) {
        httpparser::Request request;

        auto parsingResult = _parser.parse(request, buffer, buffer + length);

        if (parsingResult != httpparser::HttpRequestParser::ParsingCompleted) {
            throw Errors::InvalidHTTPRequestError();
        }

        std::vector<Header> headers;
        headers.reserve(request.headers.size());

        for (const auto& it: request.headers) {
            headers.push_back({.name = it.name, .value = it.value});
        }

        return {
                .method = request.method,
                .uri = request.uri,
                .major_version = request.versionMajor,
                .minor_version = request.versionMinor,
                .headers = headers
        };
    }

private:
    httpparser::HttpRequestParser _parser;
};

HTTP::RequestParser::RequestParser() {
    _pimpl = new Impl();
}

HTTP::Request HTTP::RequestParser::parse(const std::string& string) {
    return _pimpl->parse(string.c_str(), string.size());
}

HTTP::RequestParser::~RequestParser() {
    delete _pimpl;
}
