#include <vector>
#include <string>
#include <argparse/argparse.hpp>
#include "custom_exceptions.h"
#include "converters_factory.h"
#include "processor.h"

std::string get_program_description(const Factory::CommandsDescription& commands_description){
    std::string program_description = "The program performs audio conversions specified in the config "
                                      "file and saves result to an output file.\n\n"
                                      "Supports only WAV files with:\n"
                                      "  audio-format - PCM (without encoding)\n"
                                      "  1 channel (mono)\n"
                                      "  bit depth - signed 16 bit (little endian)\n"
                                      "  sample rate - 44100 Hz\n\n"
                                      "Commands work with files, fragment and places of files. All conversions apply to input file with help of additional files. Fragment of file can be presented by two float numbers, place - by one float number.Also, you can use <- and ->, first one means everything before, second one - everything after.\n\n"
                                      "Commands:\n\n";

    const unsigned int default_padding = 22;
    for (const auto& [name, data] : commands_description){
        const auto length = name.length();

        const auto& [description, examples] = data;

        program_description.append("  ");
        program_description.append(name);
        program_description.append(default_padding - length, ' ');
        program_description.append(description);
        program_description.append(1, '\n');

        for (const auto& example : examples){
            program_description.append(1, '\n');
            program_description.append(4, ' ');
            program_description.append(example);
            program_description.append(1, '\n');
        }
        program_description.append(1, '\n');
    }
    return program_description;
}

int main(int argc, char** argv){
    Factory::ConvertersFactory factory;

    const Factory::CommandsDescription& commands_description = factory.get_commands_description();

    std::string program_description = get_program_description(commands_description);

    argparse::ArgumentParser program("SoundProcessor");
    program.add_description(program_description);

    program.add_argument("output_file")
            .help("the output file")
            .required();

    program.add_argument("input_file")
            .help("the input file")
            .required();

    program.add_argument("additional_files")
            .help("additional files")
            .nargs(argparse::nargs_pattern::any);

    program.add_argument("-c", "--config")
            .help("specify config file")
            .required();

    try {
        program.parse_args(argc, argv);

        auto config_path = program.get<std::string>("-c");
        auto output_file = program.get<std::string>("output_file");
        auto input_file = program.get<std::string>("input_file");

        // parser can only give std::vector<std::string>, so std::vector<std::string_view> creates
        auto additional_files = program.get<std::vector<std::string>>("additional_files");

        Processing::Processor processor(config_path, output_file, input_file, additional_files, factory);

        processor.process();
    }
    catch(const Exceptions::AbstractException& exception){
        std::cerr << exception.what() << '\n';
        return exception.code();
    }
    catch (const std::runtime_error& exception){ // Incorrect args
        std::cerr << exception.what() << '\n';
        return Exceptions::INCORRECT_ARGS_ERROR_CODE;
    }
    catch(...){ // Something bad happened ..
        std::cerr << "Something really bad and unpredicted happened ...\n";
        return Exceptions::UNEXPECTED_ERROR_CODE;
    }

    return Exceptions::SUCCESS;
}
