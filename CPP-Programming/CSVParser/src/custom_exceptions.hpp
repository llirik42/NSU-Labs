#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace Exceptions{
    enum StatusCode{
        UNEXPECTED_ERROR_CODE = -1,
        SUCCESS = 0,
        OPENING_CSV_ERROR_CODE = 1,
        INCORRECT_CSV_CONTENT_ERROR_CODE = 2,
        INCORRECT_SKIP_FIRST_LINES_COUNT_ERROR_CODE = 4,
        ITERATOR_OUT_OF_RANGE_ERROR_CODE = 5,
        VALUE_OF_END_ITERATORS_ERROR_CODE = 6,
        INVALID_ITERATOR_ERROR_CODE = 7,
    };

    std::string get_error_code(size_t line, size_t column, bool from_beginning){
        const std::string tmp = from_beginning ? " (from the beginning)" : " (from the end)";
        return "Error occurred in column " + std::to_string(column) + ", line " + std::to_string(line) + tmp;
    }

    class AbstractException : public std::runtime_error{
    public:
        explicit AbstractException(const char* message, int code): std::runtime_error(message),
            _code(code) {}
        explicit AbstractException(std::string&& message, int code): std::runtime_error(message),
            _code(code) {}

        [[nodiscard]] virtual int code() const noexcept {return _code;}
    private:
        int _code;
    };
    class OpeningCSVError : public AbstractException{
    public:
        OpeningCSVError(): AbstractException("CSV can't be opened", OPENING_CSV_ERROR_CODE) {}
    };

    class IncorrectCSVContentError : public AbstractException{
    public:
        // constructor receives number of line and column where error occurred
        explicit IncorrectCSVContentError(size_t line, size_t column, bool from_beginning):
            AbstractException(get_error_code(line, column, from_beginning), INCORRECT_CSV_CONTENT_ERROR_CODE) {}
    };

    class IncorrectSkipFirstLinesCountError : public AbstractException{
    public:
        IncorrectSkipFirstLinesCountError(): AbstractException("Incorrect skip first lines count",
                                                               INCORRECT_SKIP_FIRST_LINES_COUNT_ERROR_CODE) {}
    };

    class IteratorOutOfRangeError : public AbstractException{
    public:
        IteratorOutOfRangeError(): AbstractException("Iterator is out of range",
                                                     ITERATOR_OUT_OF_RANGE_ERROR_CODE) {}
    };

    class ValueOfEndIteratorError : public AbstractException{
    public:
        ValueOfEndIteratorError(): AbstractException("Attempt of application operator* to the end iterator",
                                                     VALUE_OF_END_ITERATORS_ERROR_CODE) {}
    };

    class InvalidIteratorError : public AbstractException{
    public:
        InvalidIteratorError(): AbstractException("Attempt of operations with invalid iterator",
                                                  INVALID_ITERATOR_ERROR_CODE) {}
    };
}
