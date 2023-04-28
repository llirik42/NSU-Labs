#include <regex>
#include "config_parser.h"
#include "custom_exceptions.h"
#include "utils.h"

void reduce_spaces(std::string& string){
    static const std::regex regex(R"(\s+)");

    while (isspace(string[0])){
        string.erase(0, 1);
    }

    while (std::isspace(string.back() - 1)){
        string.erase(string.length() - 1, 1);
    }

    string = std::regex_replace(string, regex, " ");
}

ConfigParsing::Command extract_command(std::string& command_string){
    static const std::string_view delimiter = " ";

    ConfigParsing::Command result;

    if (!isspace(command_string.back())){
        command_string.push_back(' ');
    }

    size_t counter = 0;

    size_t pos;
    while ((pos = command_string.find(delimiter)) != std::string::npos){
        auto current_token = command_string.substr(0, pos);

        if (counter == 0){
            result.command_name = current_token;
        }
        else{
            result.command_args.push_back(current_token);
        }

        counter++;

        command_string.erase(0, pos + delimiter.size());
    }

    return result;
}

ConfigParsing::Command parse_command(std::string& command_string){
    static const std::regex empty_line_regex(R"((#.*)|(\s*))");

    if (std::regex_match(command_string.data(), empty_line_regex)){
        return {};
    }

    reduce_spaces(command_string);

    return extract_command(command_string);
}

struct ConfigParsing::ConfigParser::Impl{
    std::vector<Command> commands;

    void parse(std::ifstream& config_file);
};

void ConfigParsing::ConfigParser::Impl::parse(std::ifstream& config_file){
    while (!config_file.eof()){
        std::string command_string;

        std::getline(config_file, command_string);

        const Command current_command = parse_command(command_string);

        // Current string's without #
        if (!current_command.command_name.empty()){
            commands.push_back(current_command);
        }
    }
}

ConfigParsing::ConfigParser::ConfigParser(std::string_view config_path){
    _pimpl = new Impl;

    std::ifstream config_file(config_path.data());

    if (!config_file.is_open() || Utils::is_file_empty(config_file)){
        throw Exceptions::IncorrectConfigError();
    }

    _pimpl->parse(config_file);
}

ConfigParsing::ConfigParser::ConfigParserIterator ConfigParsing::ConfigParser::begin() const{
    return _pimpl->commands.begin();
}

ConfigParsing::ConfigParser::ConfigParserIterator ConfigParsing::ConfigParser::end() const{
    return _pimpl->commands.end();
}

ConfigParsing::ConfigParser::~ConfigParser(){
    delete _pimpl;
}
