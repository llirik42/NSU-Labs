#include <string>

namespace Utils {
    std::string cerror(const char* message, const std::string& error);

    std::string cerror(const char* message, int errno_code);

    std::string cerror(const std::string& message, const std::string& error);

    std::string cerror(const std::string& message, int errno_code);
}
