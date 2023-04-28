#include <filesystem>
#include "streams.h"
#include "custom_exceptions.h"
#include "utils.h"
#include "processor.h"
#include "config_parser.h"

const char FILE_INDEX_SYMBOL = '$';

struct Processing::Processor::Impl{
    std::string_view output_file_path;

    std::string_view input_file_path;

    std::vector<Streams::InputStream> additional_streams;

    ConfigParsing::ConfigParser config_parser;

    const Factory::ConvertersFactory& factory;

    Converters::ConverterParams create_params(const std::vector<std::string>& command_args);
};

Converters::ConverterParams Processing::Processor::Impl::create_params(const std::vector<std::string>& command_args){
    Converters::ConverterParams result;

    for (const auto& current_arg : command_args){
        if (current_arg[0] == FILE_INDEX_SYMBOL){
            try{
                auto index = Utils::string_to_positive_number(current_arg.data() + 1);

                if (index == 0 || index > additional_streams.size()){
                    throw Exceptions::IncorrectCommandParams();
                }

                result.push_back(&additional_streams[index - 1]);
            }
            // errors of conversion
            catch(const std::runtime_error&){
                throw Exceptions::IncorrectCommandParams();
            }
        }
        else{
            if (current_arg == Processing::LEFT_ARROW){
                result.push_back(Processing::LEFT_TIME_ARROW_ALIAS);
                continue;
            }

            if (current_arg == Processing::RIGHT_ARROW){
                result.push_back(Processing::RIGHT_TIME_ARROW_ALIAS);
                continue;
            }

            try{
                result.push_back(std::stof(current_arg));
            }
            catch(...){
                throw Exceptions::IncorrectCommandParams();
            }
        }
    }

    return result;
}

Processing::Processor::Processor(const std::string_view& config,
                                 const std::string_view& out,
                                 const std::string_view& in,
                                 const std::vector<std::string>& additional_files,
                                 const Factory::ConvertersFactory& factory){

    // For validation
    WAVManagement::parse_wav(in);

    _pimpl = new Impl{out,
                      in,
                      {},
                      ConfigParsing::ConfigParser(config),
                      factory};

    for (const auto& file : additional_files){
        _pimpl->additional_streams.emplace_back(file);
    }
}

void Processing::Processor::process() const{
    std::string tmp_path_1 = Utils::generate_random_wav_file_name();
    std::string tmp_path_2 = Utils::generate_random_wav_file_name();

    std::string_view current_input_path = tmp_path_1;
    std::string_view current_output_path = tmp_path_2;

    Utils::copy_file(_pimpl->input_file_path, tmp_path_1);

    for (const auto& [command_name, command_args] : _pimpl->config_parser){
        try{
            const Converters::Converter& current_converter = _pimpl->factory.create_converter(command_name);

            auto current_params = _pimpl->create_params(command_args);

            Streams::InputStream input_stream(current_input_path);
            Streams::OutputStream output_stream(current_output_path);

            current_converter->convert(
                    command_name,
                    output_stream,
                    input_stream,
                    current_params
            );

            std::swap(current_input_path, current_output_path);
        }
        catch(...){
            Utils::remove_file(current_input_path);
            Utils::remove_file(current_output_path);
            throw;
        }
    }

    Utils::remove_file(current_output_path);

    if (std::filesystem::exists(_pimpl->output_file_path)){
        Utils::remove_file(_pimpl->output_file_path);
    }

    Utils::rename_file(current_input_path, _pimpl->output_file_path);
}

Processing::Processor::~Processor(){
    delete _pimpl;
}
