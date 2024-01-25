#include "http_response_parser.hpp"
#include "httpparser/response.h"
#include "http_errors.hpp"
#include "httpparser/httpresponseparser.h"

class HTTP::ResponseParser::Impl {
public:
    Response parseResponse(const std::string& data) {
        httpparser::Response response;

        const auto result = _parser.parse(response, data.c_str(), data.c_str() + data.size());
        if (result == httpparser::HttpResponseParser::ParsingError) {
            throw Errors::InvalidHTTPResponseError();
        }

        return {
            .ok = response.statusCode == 200
        };
    }

private:
    httpparser::HttpResponseParser _parser;
};

HTTP::ResponseParser::ResponseParser() {
    _pimpl = new Impl();
}

HTTP::Response HTTP::ResponseParser::parseResponse(const std::string& data) {
    return _pimpl->parseResponse(data);
}

HTTP::ResponseParser::~ResponseParser() {
    delete _pimpl;
}
