#pragma once

#include <vector>
#include <string>

namespace ConfigParsing{
    struct Command{
        std::string command_name;
        std::vector<std::string> command_args;
    };

    class ConfigParser{
    public:
        using ConfigParserIterator = std::vector<Command>::const_iterator;

        explicit ConfigParser(std::string_view config_path);

        [[nodiscard]] ConfigParserIterator begin() const;

        [[nodiscard]] ConfigParserIterator end() const;

        ~ConfigParser();
    private:
        struct Impl;
        Impl* _pimpl;
    };
}
