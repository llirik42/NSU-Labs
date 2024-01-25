#include "proxy/proxy.hpp"
#include "logging/std_logger.hpp"
#include "logging/synchronous_logger.hpp"
#include "utils/abstract_error.hpp"
#include "argparse/argparse.hpp"
#include <memory>

int main(int argc, char** argv) {
    argparse::ArgumentParser program("Proxy");

    program.add_argument("-p", "--port")
            .help("port")
            .default_value(std::to_string(1080));

    program.add_argument("-b", "--backlog")
            .help("backlog")
            .default_value(std::to_string(50));

    program.add_argument("-n", "--threads-number")
            .help("Number of working threads")
            .default_value(std::to_string(1));

    program.add_argument("-a", "--address")
            .help("Address of proxy")
            .default_value(std::string("0.0.0.0"));

    try {
        program.parse_args(argc, argv);
        const auto port = static_cast<uint16_t>(std::stoi(program.get<std::string>("-p")));
        const auto address = program.get<std::string>("-a");
        const auto backlog = std::stoi(program.get<std::string>("-b"));
        const auto workers_number = static_cast<unsigned int>(std::stoi(program.get<std::string>("-n")));

        auto tmp = std::make_shared<Logging::StdLogger>();
        auto logger = std::make_shared<Logging::SynchronousLogger>(tmp);
        Proxy::Proxy proxy = Proxy::Proxy(workers_number, port, address.c_str(), backlog, logger);
        proxy.start();
    } catch (const Errors::ProxyError& error) {
        std::cerr << error.what() << '\n';
        exit(1);
    } catch (const std::invalid_argument& exception) {
        std::cerr << "Invalid args!\n";
        exit(2);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << '\n';
        exit(3);
    }

    return 0;
}
