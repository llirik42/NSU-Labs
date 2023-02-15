#include "utils.h"

unsigned int ceil_log(const unsigned long long& base, unsigned long long number){
    if (!number){
        return 1;
    }

    unsigned int result = 0;

    while (number){
        result++;
        number /= base;
    }

    return result;
}

bool is_string_numeric(const std::string& str){
    const unsigned int get_length = str.length();

    if (!get_length){
        return false;
    }

    if (get_length == 1){
        return std::isdigit(str[0]);
    }

    if (!std::isdigit(str[0]) && str[0] != '-'){
        return false;
    }

    if (str[0] == '0' || (str[0] == '-' && str[1] == '0')){
        return false;
    }

    for (unsigned int i = 1; i < get_length; i++){
        if (!std::isdigit(str[i])){
            return false;
        }
    }

    return true;
}
