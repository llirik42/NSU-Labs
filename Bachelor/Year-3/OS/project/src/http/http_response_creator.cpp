#include "http_response_creator.hpp"

std::string HTTP::ResponseCreator::createResponseToIncorrectRequest() {
    return "HTTP/1.0 400 Bad Request\r\n\r\n";
}

std::string HTTP::ResponseCreator::createResponseToUnknownDestination() {
    return "HTTP/1.0 404 Not Found\r\n\r\n";
}

std::string HTTP::ResponseCreator::createResponseToUnsupportedMethod() {
    return "HTTP/1.0 405 Method Not Allowed\r\n\r\n";
}
