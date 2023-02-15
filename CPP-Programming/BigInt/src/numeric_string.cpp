#include "numeric_string.h"
#include "utils.h"

// Copies chars for the end of "from" to the end of "to"
void copy_into_the_end(const char* from, const unsigned int& from_length, char* to, const unsigned int& to_length){
    const unsigned int length = get_min(from_length, to_length);

    for (unsigned int i = length; i > 0; i--){
        to[to_length - (length - i) - 1] = from[from_length - (length - i) - 1];
    }
}

void NumericString::append(char c, unsigned int count){
    char* tmp = new char[this->_length + count];

    for (unsigned int i = this->_length; i > 0; i--){
        tmp[i - 1] = this->_string[i - 1];
    }
    for (unsigned int i = 0; i < count; i++){
        tmp[i + this->_length] = c;
    }

    delete[] this->_string;
    this->_length += count;

    this->_string = new char[this->_length];

    copy_into_the_end(tmp, this->_length, this->_string, this->_length);

    delete[] tmp;

}
void NumericString::insert_front(char c){
    NumericString tmp = (*this);

    delete[] this->_string;

    this->_string = new char[++this->_length];

    copy_into_the_end(tmp._string, this->_length - 1, this->_string, this->_length);

    this->_string[0] = c;
}
void NumericString::add_zero(){
    this->insert_front('0');
}
void NumericString::erase_front(){
    NumericString tmp = (*this);

    delete[] this->_string;

    this->_string = new char[--this->_length];

    copy_into_the_end(tmp._string, this->_length, this->_string, this->_length);
}
void NumericString::reduce_zeroes(){
    while(this->get_length() > 1 && this->_string[0] == '0'){
        this->erase_front();
    }
}

bool NumericString::is_zero() const{
    return this->_length == 1 && this->_string[0] == '0';
}
unsigned int NumericString::get_length() const{
    return this->_length;
}

unsigned int ceil_log_numerical_str(const unsigned long long& base, const NumericString& s){
    if (s.is_zero()){
        return 1;
    }

    unsigned int result = 0;

    NumericString tmp = s;
    while (!tmp.is_zero()){
        result++;

        tmp /= base;
    }

    return result;
}

NumericString::NumericString(): _length(0), _string(nullptr) {}
NumericString::NumericString(unsigned long long n){
    if (!n){
        this->_length = 1;
        this->_string = new char[1];
        this->_string[0] = '0';
        return;
    }

    this->_length = 0;
    this->_string = nullptr;

    while(n){
        this->insert_front(digit_to_char(n % 10));
        n /= 10;
    }
}
NumericString::NumericString(const std::string& s){
    this->_length = 0;
    this->_string = nullptr;

    for (unsigned int i = s.length(); i > 0; i--){
        this->insert_front(s[i - 1]);
    }
}
NumericString::NumericString(NumericString&& s) noexcept{
    this->_length = s._length;
    this->_string = s._string;
    s._string = nullptr;
}
NumericString::NumericString(const NumericString &s){
    this->_length = s._length;

    this->_string = this->_length ? new char[this->_length] : nullptr;

    if (this->_length){
        copy_into_the_end(s._string, s._length, this->_string, this->_length);
    }
}
NumericString::~NumericString(){
    delete[] this->_string;
}

NumericString& NumericString::operator=(const NumericString& s){
    if (&s == this){
        return (*this);
    }

    this->_length = 0;
    delete[] this->_string;
    this->_string = nullptr;

    for (unsigned int i = s._length; i > 0; i--){
        this->insert_front(s._string[i - 1]);
    }

    return (*this);
}
NumericString& NumericString::operator=(NumericString&& s) noexcept{
    this->_length = s._length;
    this->_string = s._string;

    s._string = nullptr;

    return (*this);
}

NumericString NumericString::operator+(const NumericString& operand) const{
    const unsigned int max_operand_length = get_max(this->get_length(), operand.get_length());

    NumericString result = NumericString();

    NumericString tmp1; tmp1 = (*this);
    NumericString tmp2; tmp2 = operand;

    while(tmp1.get_length() < max_operand_length){
        tmp1.add_zero();
    }
    while(tmp2.get_length() < max_operand_length){
        tmp2.add_zero();
    }

    unsigned int carry = 0;
    for (unsigned int i = max_operand_length; i > 0; i--){
        const unsigned int current_digit1 = char_to_digit(tmp1._string[i - 1]);
        const unsigned int current_digit2 = char_to_digit(tmp2._string[i - 1]);

        const unsigned int current_sum = current_digit1 + current_digit2 + carry;

        result.insert_front(digit_to_char(current_sum % 10));

        carry = current_sum / 10;
    }

    if (carry){
        result.insert_front('1');
    }

    return result;
}
NumericString NumericString::operator*(const NumericString& operand) const{
    NumericString result = NumericString();

    const unsigned int operand_length = operand.get_length();

    if (this->is_zero()){
        return NumericString(0);
    }

    if (operand_length == 1){
        unsigned char carry = 0;

        const unsigned char operand_only_digit = char_to_digit(operand._string[0]);
        for (unsigned int i = this->get_length(); i > 0; i--){
            const unsigned char current_digit1 = char_to_digit(this->_string[i - 1]);

            const unsigned char product = current_digit1 * operand_only_digit + carry;

            result.insert_front(digit_to_char(product % 10));

            carry = product / 10;
        }

        if (carry){
            result.insert_front(digit_to_char(carry));
        }

        result.reduce_zeroes();
    }
    else{
        result = NumericString(0);

        for (unsigned int i = operand_length; i > 0; i--){
            NumericString current_digit = NumericString(char_to_digit(operand._string[i - 1]));

            NumericString current_product = (*this) * current_digit;

            current_product.append('0', operand_length - i);

            result += current_product;
        }
    }

    return result;
}
NumericString NumericString::operator/(const unsigned long long& operand) const{
    NumericString tmp = NumericString(operand);

    if (tmp > *this){
        return NumericString(0);
    }

    unsigned long long r = 0;
    unsigned int current_index = 0;

    NumericString result;

    while(r < operand){
        r = r * 10 + char_to_digit(this->_string[current_index++]);
    }

    while(true){
        result.append(digit_to_char((unsigned char) (r / operand)), 1);
        r %= operand;

        if (current_index == this->_length){
            break;
        }

        r = r * 10 + char_to_digit(this->_string[current_index++]);

        while(r < operand){
            if (current_index == this->_length){
                break;
            }
            result.append('0', 1);
            r = r * 10 + char_to_digit(this->_string[current_index++]);
        }
    }

    return result;
}
unsigned long long NumericString::operator%(const unsigned long long& operand) const{
    NumericString tmp = NumericString(operand);

    if (tmp > *this){
        return (unsigned long long)(*this);
    }

    unsigned long long r = 0;
    unsigned int current_index = 0;

    while(r < operand){
        r = r * 10 + char_to_digit(this->_string[current_index++]);
    }

    while(true){
        r %= operand;

        if (current_index == this->_length){
            break;
        }

        r = r * 10 + char_to_digit(this->_string[current_index++]);

        while(r < operand){
            if (current_index == this->_length){
                break;
            }
            r = r * 10 + char_to_digit(this->_string[current_index++]);
        }
    }

    return r;
}

NumericString& NumericString::operator+=(const NumericString& operand){
    (*this) = (*this) + operand;
    return (*this);
}
NumericString& NumericString::operator/=(const unsigned long long& operand){
    (*this) = (*this) / operand;
    return (*this);
}

bool NumericString::operator==(const std::string& s) const{
    if (this->_length != s.length()){
        return false;
    }

    for (unsigned int i = 0; i < s.length(); i++){
        if (this->_string[i] != s[i]){
            return false;
        }
    }

    return true;
}
bool NumericString::operator==(const NumericString& s) const{
    if (this->_length != s._length){
        return false;
    }

    for (unsigned int i = 0; i < s._length; i++){
        if (this->_string[i] != s._string[i]){
            return false;
        }
    }

    return true;
}
bool NumericString::operator!=(const std::string& s) const{
    return this->_string != s;
}
bool NumericString::operator!=(const NumericString& s) const{
    return !(s == (*this));
}
bool NumericString::operator>(const NumericString& s) const{
    const unsigned int length1 = this->get_length();
    const unsigned int length2 = s.get_length();

    if (length1 != length2){
        return length1 > length2;
    }

    for (unsigned int i = 0; i < length1; i++){
        const char current_digit1 = this->_string[i];
        const char current_digit2 = s._string[i];

        if (current_digit1 != current_digit2){
            return current_digit1 > current_digit2;
        }
    }

    return false;
}

NumericString::operator unsigned long long() const{
    const NumericString ullong_max_numeric = NumericString(ULLONG_MAX);

    if ((*this) > ullong_max_numeric){
        return ULLONG_MAX;
    }

    unsigned long long result = 0;

    unsigned long current_m = 1;
    for (unsigned int i = this->_length; i > 0; i--, current_m *= 10){
        result += current_m * char_to_digit(this->_string[i - 1]);
    }

    return result;
}
NumericString::operator std::string() const{
    std::string result;

    for (unsigned int i = 0; i < this->_length; i++){
        result.append(1, this->_string[i]);
    }

    return result;
}
