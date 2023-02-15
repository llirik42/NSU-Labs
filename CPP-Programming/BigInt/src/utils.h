#include <iostream>

inline unsigned int get_max(unsigned int a, unsigned int b){
    return a > b ? a : b;
}
inline unsigned int get_min(unsigned int a, unsigned int b){
    return a > b ? b : a;
}
inline unsigned char char_to_digit(char c){
    return c - '0';
}
inline char digit_to_char(unsigned char d){
    return char(d + '0');
}

// Ceil of log. If number = 0, returns 1
unsigned int ceil_log(const unsigned long long& base, unsigned long long number);

// Is string can be converted to integer (positive of negative)
bool is_string_numeric(const std::string& str);
