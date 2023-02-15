#include "big_int.h"
#include "utils.h"

inline const unsigned char DISJUNCTION_CODE = 0;
inline const unsigned char CONJUNCTION_CODE = 1;
inline const unsigned char XOR_CODE = 2;
inline const unsigned char ADDITION_CODE = 3;
inline const unsigned char SUBTRACTION_CODE = 4;
inline const unsigned char MULTIPLICATION_CODE = 5;
inline const unsigned char DIV_CODE = 6;
inline const unsigned char MOD_CODE = 7;

void BigInt::handle_minus_zero(){
    // If _is_positive = false, but object = 0, sets _is_positive = true

    _is_positive = _is_positive || is_zero();
}
BigInt& BigInt::logic_operation(const BigInt& operand, const unsigned char& operation_type){
    BigInt result;
    result._is_positive = _is_positive == operand._is_positive;

    switch(operation_type){
        case DISJUNCTION_CODE:
            result._vector = _vector | operand._vector;
            break;
        case CONJUNCTION_CODE:
            result._vector = _vector & operand._vector;
            break;
        case XOR_CODE:
            result._vector = _vector ^ operand._vector;
            break;
        default:
            result._vector = _vector;
    }

    *this = result;
    return *this;


}
BigInt operation(const BigInt& operand1, const BigInt& operand2, const unsigned char& operation_type){
    BigInt result = operand1;

    switch(operation_type){
        case DISJUNCTION_CODE:
            result |= operand2;
            break;
        case CONJUNCTION_CODE:
            result &= operand2;
            break;
        case XOR_CODE:
            result ^= operand2;
            break;
        case ADDITION_CODE:
            result += operand2;
            break;
        case SUBTRACTION_CODE:
            result -= operand2;
            break;
        case MULTIPLICATION_CODE:
            result *= operand2;
            break;
        case DIV_CODE:
            result /= operand2;
            break;
        case MOD_CODE:
            result %= operand2;
            break;
        default:
            break;
    }

    return result;
}

BigInt::BigInt(): _is_positive(true), _vector(SmartVector()) {}
BigInt::BigInt(const int& number): _is_positive(number >= 0), _vector(SmartVector(abs(number))) {}
BigInt::BigInt(const std::string& str) {
    if (!is_string_numeric(str)){
        throw std::invalid_argument("Invalid string for BigInt");
    }

    _is_positive = str[0] != '-';

    std::string tmp = str;

    if (!_is_positive){
        tmp.erase(0, 1);
    }

    _vector = SmartVector(tmp);
}
BigInt::BigInt(const BigInt& b)=default;
BigInt::BigInt(BigInt&& b) noexcept: _is_positive(b._is_positive), _vector(std::move(b._vector)) {}
BigInt::~BigInt()=default;

BigInt& BigInt::operator=(const BigInt& b)=default;
BigInt& BigInt::operator=(BigInt&& b) noexcept{
    _is_positive = b._is_positive;
    _vector = std::move(b._vector);
    return *this;
}

BigInt& BigInt::operator+=(const BigInt& b){
    BigInt result;

    result._is_positive = _vector > b._vector ? _is_positive : b._is_positive;

    if (_is_positive != b._is_positive){
        result._vector = _vector > b._vector ? _vector - b._vector : b._vector - _vector;
    }
    else{
        result._vector = _vector + b._vector;
    }

    *this = result;
    handle_minus_zero();
    return *this;
}
BigInt& BigInt::operator*=(const BigInt& b){
    _is_positive = _is_positive == b._is_positive;
    _vector *= b._vector;

    handle_minus_zero();

    return *this;
}
BigInt& BigInt::operator-=(const BigInt& b){
    BigInt tmp = b;
    tmp._is_positive = !tmp._is_positive;
    *this += tmp;

    handle_minus_zero();

    return *this;
}
BigInt& BigInt::operator/=(const BigInt& b){
    if (b.is_zero()){
        throw std::invalid_argument("Division by zero");
    }
    _is_positive = _is_positive == b._is_positive;
    _vector /= b._vector;

    handle_minus_zero();


    return *this;
}
BigInt& BigInt::operator%=(const BigInt& b){
    if (b.is_zero()){
        throw std::invalid_argument("Division by zero");
    }
    if (!b._is_positive){
        throw std::invalid_argument("Attempt of getting reminded for division by a negative number");
    }

    while(!_is_positive){
        *this += b;
    }
    _vector %= b._vector;

    return *this;
}

BigInt BigInt::operator+() const{
    return *this;
}
BigInt BigInt::operator-(){
    _is_positive = !_is_positive;
    return *this;
}
BigInt& BigInt::operator~(){
    _vector = ~_vector;
    return *this;
}

BigInt& BigInt::operator++(){
    *this += BigInt(1);
    return *this;
}
const BigInt BigInt::operator++(int){
    BigInt result = *this;
    *this += BigInt(1);
    return result;
}
BigInt& BigInt::operator--(){
    *this -= BigInt(1);
    return *this;
}
const BigInt BigInt::operator--(int){
    BigInt result = *this;
    *this -= BigInt(1);
    return result;
}

bool BigInt::operator==(const BigInt& b) const{
    return !compare(b);
}
bool BigInt::operator!=(const BigInt& b) const{
    return compare(b);
}
bool BigInt::operator>(const BigInt& b) const{
    return compare(b) == 1;
}
bool BigInt::operator<(const BigInt& b) const{
    return compare(b) == -1;
}
bool BigInt::operator>=(const BigInt& b) const{
    return compare(b) >= 0;
}
bool BigInt::operator<=(const BigInt& b) const{
    return compare(b) <= 0;
}

BigInt& BigInt::operator^=(const BigInt& b){
    *this = logic_operation(b, XOR_CODE);
    return *this;
}
BigInt& BigInt::operator&=(const BigInt& b){
    *this = logic_operation(b, CONJUNCTION_CODE);
    return *this;
}
BigInt& BigInt::operator|=(const BigInt& b){
    *this = logic_operation(b, DISJUNCTION_CODE);
    return *this;
}

BigInt::operator int() const{
    if (_is_positive && _vector > SmartVector(INT_MAX)){
        return INT_MAX;
    }
    if (!_is_positive && _vector > SmartVector(-(long long)(INT_MIN))){
        return INT_MIN;
    }

    int abs_value = int(_vector);

    return _is_positive ? abs_value : -abs_value;
}
BigInt::operator std::string() const{
    std::string result = std::string(_vector);

    if (!_is_positive){
        result.insert(0, 1, '-');
    }
    return result;
}

size_t BigInt::size() const{
    return sizeof(_is_positive) + _vector.size();
}
bool BigInt::is_zero() const{
    return (int(*this)) == 0;
}
int BigInt::compare(const BigInt& b) const{
    if (_is_positive != b._is_positive){
        return _is_positive ? 1 : -1;
    }

    int vectors_comparison_result = _vector.compare(b._vector);

    return _is_positive ? vectors_comparison_result : -vectors_comparison_result;
}

BigInt operator+(const BigInt& a, const BigInt& b){
    return operation(a, b, ADDITION_CODE);
}
BigInt operator-(const BigInt& a, const BigInt& b){
    return operation(a, b, SUBTRACTION_CODE);
}
BigInt operator*(const BigInt& a, const BigInt& b){
    return operation(a, b, MULTIPLICATION_CODE);
}
BigInt operator/(const BigInt& a, const BigInt& b){
    return operation(a, b, DIV_CODE);
}
BigInt operator%(const BigInt& a, const BigInt& b){
    return operation(a, b, MOD_CODE);
}
BigInt operator^(const BigInt& a, const BigInt& b){
    return operation(a, b, XOR_CODE);
}
BigInt operator&(const BigInt& a, const BigInt& b){
    return operation(a, b, CONJUNCTION_CODE);
}
BigInt operator|(const BigInt& a, const BigInt& b){
    return operation(a, b, DISJUNCTION_CODE);
}

std::ostream& operator<<(std::ostream& out, const BigInt &b){
    out << std::string(b);
    return out;
}
std::istream& operator>>(std::istream& in, BigInt& b){
    std::string in_str;
    std::string result_input;

    in >> in_str;

    bool met_minus = false;
    bool is_prev_char_minus = false;
    char first_digit = -1;

    const unsigned int length = in_str.length();
    for (unsigned int i = 0; i < length; i++){
        char current_char = in_str[i];

        if (std::isspace(current_char)){
            if (result_input.length()){
                break;
            }
            continue;
        }

        if (current_char == '-'){
            if (met_minus){
                break;
            }
            met_minus = true;
            is_prev_char_minus = true;
            continue;
        }
        else if (!std::isdigit(current_char)){
            break;
        }

        if (first_digit == -1){
            first_digit = current_char;
        }

        if (is_prev_char_minus && current_char == '0'){
            break;
        }

        if (first_digit == '0'){
            break;
        }

        if (is_prev_char_minus){
            result_input.append(1, '-');
        }

        result_input.append(1, current_char);

        is_prev_char_minus = false;
    }

    b = result_input.length() ? BigInt(result_input) : BigInt(0);

    return in;
}
