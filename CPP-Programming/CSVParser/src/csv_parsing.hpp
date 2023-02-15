#pragma once

#include <fstream>
#include <iterator>
#include <utility>
#include <string>
#include <sstream>
#include <tuple>

#include "custom_exceptions.hpp"
#include "utils.hpp"

inline const char CARRIAGE_RETURN_CHAR = '\r';
inline const char NULL_TERMINATOR = '\0';

namespace Parsing{
    // State of iterator
    enum class State{
        INVALID, // isn't linked to any parser
        MOVABLE, // can read lines
        PRE_END, // can't read lines but is not equal to end()
        END // is equal to end() or rend()
    };

    struct Settings{
        char delimiter;
        char escape_character;
        char newline_character;
    };

    /*
     * Returns whether extraction was successful or not
     * Extraction with not entirely appropriate string is considered unsuccessful
     * For example, if it tries to extract <int> from "1q" it will return False,
     * because "1q" is not entirely numeric
    */
    template<typename T>
    bool extract_token(T& token, const std::string& string){
        if (string.empty()){
            return false;
        }

        std::istringstream string_stream(string);

        string_stream >> token;

        // We have to clear stream to safely call tellg() (else it can throw exception)
        string_stream.clear();

        return static_cast<size_t>(string_stream.tellg()) == string.size() && !string_stream.fail();
    }

    template<>
    bool extract_token(std::string& token, const std::string& string){
        token = string;
        return true;
    }

    // returns string with token, index where next string begins
    std::pair<std::string, size_t> extract_string_with_token(const Settings& settings, std::string_view line,
                                                             size_t offset){
        std::string string_with_token;

        bool is_prev_char_escape = false;

        size_t i;
        for (i = 0; line[i + offset] != NULL_TERMINATOR; i++){
            const char c = line[i + offset];

            const bool is_current_char_delimiter = c == settings.delimiter;
            const bool is_current_char_escape = c == settings.escape_character;

            if (is_current_char_delimiter && !is_prev_char_escape){
                break;
            }

            if (is_prev_char_escape && !is_current_char_delimiter){
                string_with_token.append(1, settings.escape_character);
            }

            if (!is_current_char_escape){
                string_with_token.append(1, c);

            }

            is_prev_char_escape = is_current_char_escape;
        }

        return {string_with_token, i};
    }

    template<size_t index, typename ...Types>
    size_t extract_tuple(std::tuple<Types...>& tuple_ref, std::string_view line,
                       const Settings& settings, size_t offset){
        auto [string_with_token, index_of_next] = extract_string_with_token(settings, line, offset);

        Utils::trim_back_spaces(string_with_token);

        const bool success = extract_token(std::get<index>(tuple_ref), string_with_token);

        if (!success){
            // + 1, because index starts from 0
            return index + 1;
        }

        if constexpr (index < sizeof...(Types) - 1){
            return extract_tuple<index + 1, Types...>(tuple_ref, line, settings, offset + index_of_next + 1);
        }

        return 0;
    }

    // returns number of column (1, 2, ...) where error occurred, 0 in case of success
    template<typename ...Types>
    size_t parse_tuple(std::tuple<Types...>& tuple, std::string_view line, const Settings& settings){
        size_t column_number_with_number = extract_tuple<0, Types...>(tuple, line, settings, 0);
        return column_number_with_number;
    }

    template<typename ...Types>
    class CSVParser;

    template<typename ...Types>
    class Iterator{
    public:
        Iterator():_file_position(0), _line_number(0), _state(State::INVALID), _parser(nullptr),
                   _tuple{} {}

        bool operator==(const Iterator& it) const{
            return it._state == this->_state && it._file_position == this->_file_position;
        }
        bool operator!=(const Iterator& it) const{
            return !(it == *this);
        }

        const std::tuple<Types...>& operator*() const{
            if (this->_state == State::INVALID){
                throw Exceptions::InvalidIteratorError();
            }
            if (this->_state == State::END){
                throw Exceptions::ValueOfEndIteratorError();
            }
            return this->_tuple;
        }

        Iterator& operator+=(size_t delta){
            if (!delta){
                return *this;
            }

            if (this->_state == State::INVALID){
                throw Exceptions::InvalidIteratorError();
            }

            if (this->_state == State::END){
                throw Exceptions::IteratorOutOfRangeError();
            }

            if (this->_state == State::PRE_END){
                if (delta > 1){
                    throw Exceptions::IteratorOutOfRangeError();
                }

                this->_state = State::END;
            }

            if (this->_state == State::MOVABLE){
                read_tuple(delta);
            }

            return *this;
        }

        virtual ~Iterator() = default;
    protected:
        std::streamoff _file_position;
        size_t _line_number;
        State _state;
        CSVParser<Types...>* _parser;
        std::tuple<Types...> _tuple;

        virtual void read_tuple(size_t offset) = 0;
    };

    template<typename ...Types>
    class ForwardIterator : public Iterator<Types...>{
        friend CSVParser<Types...>;
    public:
        ForwardIterator(): Iterator<Types...>() {}

        ForwardIterator& operator++(){
            *this += 1;
            return *this;
        }

        ForwardIterator operator++(int){
            auto result = *this;
            (*this) += 1;
            return result;
        }
    protected:
        void read_tuple(size_t offset) override{
            // We have to clear, because other iterator could read and eof-flag in ifstream could be set
            this->_parser->_ifstream.clear();

            // We have seekg, because other iterator could work with ifstream and could move
            // it to somewhere
            this->_parser->_ifstream.seekg(this->_file_position);

            std::string file_line;
            for (size_t i = 0; i < offset ; i++){
                std::getline(this->_parser->_ifstream, file_line,
                             this->_parser->_settings.newline_character);

                if (this->_state == State::END && this->_parser->_ifstream.fail()){
                    throw Exceptions::IteratorOutOfRangeError();
                }

                if (this->_parser->_ifstream.fail()){
                    this->_state = State::END;
                }
                else if (this->_parser->_ifstream.eof()){
                    this->_state = State::PRE_END;
                }
            }

            // On some systems, newline-character isn't only "\n", it might be "\r\n" (like on Windows)
            // It's all in case newline-character is '\n'

            if (file_line.back() == CARRIAGE_RETURN_CHAR){
                file_line.erase(file_line.size() - 1);
            }

            auto column_number_with_error = parse_tuple<Types...>(this->_tuple, file_line,
                                                                  this->_parser->_settings);
            if (column_number_with_error){
                throw Exceptions::IncorrectCSVContentError(this->_line_number + 1,
                                                           column_number_with_error, true);
            }

            if (Utils::is_next_line_empty(this->_parser->_ifstream)){
                this->_state = State::PRE_END;
                this->_file_position = EOF;
            }
            else{
                this->_file_position = this->_parser->_ifstream.tellg();
                this->_line_number += offset;
            }
        }
    private:
        explicit ForwardIterator(std::streamoff start_position, size_t start_line_number, State state,
                                 CSVParser<Types...>* parser){
            this->_file_position = start_position;
            this->_line_number = start_line_number;
            this->_state = state;
            this->_parser = parser;
            this->_tuple = {};

            if (state != State::END){
                read_tuple(1);
            }
        }
    };

    template<typename ...Types>
    ForwardIterator<Types...> operator+(const ForwardIterator<Types...>& it, size_t delta){
        auto result = it;
        result += delta;
        return result;
    }

    template<typename ...Types>
    ForwardIterator<Types...> operator+(size_t delta, const ForwardIterator<Types...>& it){
        return it + delta;
    }

    template<typename ...Types>
    class BackwardIterator : public Iterator<Types...>{
        friend CSVParser<Types...>;
    public:
        BackwardIterator(): Iterator<Types...>(), _min_file_position(0) {}

        BackwardIterator& operator++(){
            *this += 1;
            return *this;
        }

        BackwardIterator operator++(int){
            auto result = *this;
            (*this) += 1;
            return result;
        }
    protected:
        void read_tuple(size_t offset) override{
            // We have to clear, because other iterator could read and eof-flag in ifstream could be set
            this->_parser->_ifstream.clear();

            // We have seekg, because other iterator could work with ifstream and could move
            // it to somewhere
            this->_parser->_ifstream.seekg(this->_file_position);

            std::string file_line;
            for (size_t i = 0; i < offset ; i++){
                if (this->_state == State::END || this->_parser->_ifstream.tellg() == _min_file_position){
                    throw Exceptions::IteratorOutOfRangeError();
                }

                Utils::read_line_backwards(this->_parser->_ifstream, file_line,
                                           this->_parser->_settings.newline_character);

                if (this->_parser->_ifstream.fail() || this->_parser->_ifstream.tellg() == _min_file_position){
                    if (this->_state == State::MOVABLE){
                        this->_state = State::PRE_END;
                    }
                    else{
                        this->_state = State::END;
                    }
                }
            }

            if (file_line.back() == CARRIAGE_RETURN_CHAR){
                file_line.erase(file_line.size() - 1);
            }

            auto column_number_with_error = parse_tuple<Types...>(this->_tuple, file_line,
                                                                  this->_parser->_settings);
            if (column_number_with_error){
                throw Exceptions::IncorrectCSVContentError(this->_line_number, column_number_with_error, false);
            }

            this->_file_position = this->_parser->_ifstream.tellg();
            this->_line_number += offset;
        }
    private:
        explicit BackwardIterator(std::streamoff min_position, std::streamoff start_position,
                                  size_t start_line_number, State state, CSVParser<Types...>* parser){
            this->_file_position = start_position;
            this->_line_number = start_line_number;
            this->_state = state;
            this->_parser = parser;
            this->_tuple = {};

            _min_file_position = min_position;

            if (state != State::END){
                read_tuple(1);
            }
        }

        std::streamoff _min_file_position;
    };

    template<typename ...Types>
    BackwardIterator<Types...> operator+(const BackwardIterator<Types...>& it, size_t delta){
        auto result = it;
        result += delta;
        return result;
    }

    template<typename ...Types>
    BackwardIterator<Types...> operator+(size_t delta, const BackwardIterator<Types...>& it){
        return it + delta;
    }

    template<typename ...Types>
    class CSVParser{
    public:
        CSVParser(const char* path, const Settings& settings,
                  size_t skip_first_lines_count){
            // We have to use binary mode, because we have to be able to do seekg()
            // Without it, it doesn't work normally
            _ifstream = std::ifstream(path, std::fstream::binary);

            if (!_ifstream.is_open()){
                throw Exceptions::OpeningCSVError();
            }

            _forward_start_position = Utils::count_line_pos(_ifstream, skip_first_lines_count,
                                                            settings.newline_character);

            if (_ifstream.fail()){
                throw Exceptions::IncorrectSkipFirstLinesCountError();
            }

            // we have to clear after call of Utils::count_line_pos()
            _ifstream.clear();

            _backward_start_position = Utils::get_last_position_in_file(_ifstream);

            _min_position =  _forward_start_position ? _forward_start_position - 2 : EOF;

            _iterators_start_line_number = skip_first_lines_count;

            _settings = settings;
        }

        ForwardIterator<Types...> begin(){
            return ForwardIterator<Types...>(_forward_start_position, _iterators_start_line_number,
                                             State::MOVABLE, this);
        }
        ForwardIterator<Types...> end(){
            // start line number doesn't matter here
            return ForwardIterator<Types...>(EOF, 0, State::END, this);
        }

        BackwardIterator<Types...> rbegin(){
            return BackwardIterator<Types...>(_min_position, _backward_start_position, 0,
                                              State::MOVABLE, this);
        }
        BackwardIterator<Types...> rend(){
            // start line and min position don't matter here
            return BackwardIterator<Types...>(0, _min_position, 0, State::END, this);
        }
    private:
        friend ForwardIterator<Types...>;
        friend BackwardIterator<Types...>;

        std::ifstream _ifstream;
        std::streamoff _forward_start_position;
        std::streamoff _backward_start_position;
        std::streamoff _min_position;
        size_t _iterators_start_line_number;
        Settings _settings{};
    };
}
