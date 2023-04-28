#include "smart_vector.h"
#include "numeric_string.h"
#include "utils.h"

inline const unsigned char DISJUNCTION_CODE = 0;
inline const unsigned char CONJUNCTION_CODE = 1;
inline const unsigned char XOR_CODE = 2;
inline const unsigned char ADDITION_CODE = 3;
inline const unsigned char SUBTRACTION_CODE = 4;
inline const unsigned char MULTIPLICATION_CODE = 5;
inline const unsigned char DIV_CODE = 6;
inline const unsigned char MOD_CODE = 7;

SmartVector& SmartVector::logic_operation(const SmartVector& operand, const unsigned char& operation_type){
    const unsigned int max_length = get_max(_length, operand._length);

    SmartVector tmp = operand;

    extend_and_copy(max_length - _length);
    tmp.extend_and_copy(max_length - _length);

    for (unsigned int i = 0; i < max_length; i++){
        Block& result_digit = _data[i];
        const Block& operand_digit = tmp._data[i];

        switch(operation_type){
            case XOR_CODE:
                result_digit = result_digit ^ operand_digit;
                break;
            case CONJUNCTION_CODE:
                result_digit = result_digit & operand_digit;
                break;
            case DISJUNCTION_CODE:
                result_digit = result_digit | operand_digit;
                break;
            default:
                break;
        }
    }
    reduce_first_zero_blocks();
    return *this;
}
SmartVector operation(const SmartVector& operand1, const SmartVector& operand2, const unsigned char& operation_type){
    SmartVector result = operand1;

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

void SmartVector::extend_and_copy(const unsigned int& delta){
    // Extends array of vector and copies data to the end

    SmartVector tmp = (*this);

    _length += delta;
    _data = new Block[_length];

    for (unsigned int i = 0; i < delta; i++){
        _data[i] = 0;
    }

    for (unsigned int i = 0; i < tmp._length; i++){
        _data[_length - i - 1] = tmp._data[tmp._length - i - 1];
    }
}
void SmartVector::append_zero_blocks(const unsigned int& count){
    SmartVector tmp = *this;

    delete[] _data;
    _data = new Block[_length + count];

    for (unsigned int i = 0; i < _length; i++){
        _data[i] = tmp._data[i];
    }
    for (unsigned int i = 0; i < count; i++){
        _data[_length + i] = 0;
    }

    _length += count;
}
void SmartVector::reduce_first_blocks(const unsigned int& count){
    SmartVector tmp = *this;

    _length -= count;
    delete[] _data;
    _data = new Block[_length];

    for (unsigned int i = _length; i > 0; i--){
        _data[i - 1] = tmp._data[i + count - 1];
    }
}
void SmartVector::reduce_first_zero_blocks(){
    unsigned int zero_blocks_count = 0;

    const unsigned int stop = _length - 1;
    for (unsigned int i = 0; i < stop; i++){
        if (_data[i]){
            break;
        }
        zero_blocks_count++;
    }

    reduce_first_blocks(zero_blocks_count);
}
SmartVector SmartVector::divide(const SmartVector& operand, const bool& mod){
    if (operand > *this){
        SmartVector mod_result = *this;
        return mod ? mod_result : SmartVector(0);
    }
    if (operand == *this){
        return mod ? SmartVector(0) : SmartVector(1);
    }
    if (operand == SmartVector(1)){
        SmartVector div_result = *this;
        return mod ? SmartVector(0) : div_result;
    }

    const SmartVector baseSmartVector = SmartVector(BASE);
    const SmartVector zeroSmartVector = SmartVector(0);

    SmartVector q = zeroSmartVector;
    SmartVector r = zeroSmartVector;

    unsigned int current_index = 0;

    while(operand > r){
        r = r * baseSmartVector + SmartVector(_data[current_index++]);
    }

    while(true){
        q = q * baseSmartVector;
        while(r >= operand){
            r -= operand;
            ++q;
        }

        if (current_index == _length){
            break;
        }

        r = r * baseSmartVector + SmartVector(_data[current_index++]);
    }

    return mod ? r : q;
}

SmartVector::SmartVector(): _length(1), _data(new Block[1]) {_data[0] = 0;}
SmartVector::SmartVector(unsigned long long n){
    _length = ceil_log(BASE, n);

    _data = new Block[_length];

    for (unsigned int i = _length; i > 0; i--){
        _data[i - 1] = (Block) (n % BASE);
        n /= BASE;
    }
}
SmartVector::SmartVector(const std::string& s){
    NumericString numerical_str = NumericString(s);

    _length = ceil_log_numerical_str(BASE, numerical_str);

    _data = new Block[_length];

    for (unsigned int i = _length; i > 0; i--){
        _data[i - 1] = Block(numerical_str % BASE);
        numerical_str /= BASE;
    }
}
SmartVector::SmartVector(SmartVector&& v) noexcept{
    _length = v._length;
    _data = v._data;
    v._data = nullptr;
}
SmartVector::SmartVector(const SmartVector& v){
    _length = v._length;
    _data = new Block[_length];

    for (unsigned int i = 0; i < _length; i++){
        _data[i] = v._data[i];
    }
}
SmartVector::~SmartVector(){
    delete[] _data;
}

SmartVector& SmartVector::operator=(const SmartVector& v){
    if (&v == this){
        return (*this);
    }

    _length = v._length;
    delete[] _data;

    _data = new Block[_length];

    for (unsigned int i = 0; i < _length; i++){
        _data[i] = v._data[i];
    }

    return (*this);
}
SmartVector& SmartVector::operator=(SmartVector&& v) noexcept{
    *this = v;
    return (*this);
}

SmartVector& SmartVector::operator+=(const SmartVector& v){
    const unsigned int max_length = get_max(_length, v._length);

    extend_and_copy(max_length - _length);

    Block carry = 0;
    for (unsigned int i = 0; i < max_length; i++){
        unsigned long long current_sum = (unsigned long long) (_data[_length - i - 1]) + carry;

        if (v._length > i){
            current_sum += v._data[v._length - i - 1];
        }

        _data[_length - i - 1] = Block(current_sum % BASE);
        carry = current_sum >= BASE;
    }

    if (carry){
        extend_and_copy(1);
        _data[0] = carry;
    }

    return (*this);
}
SmartVector& SmartVector::operator*=(const SmartVector& v){
    SmartVector zeroSmartVector = SmartVector(0);
    if (v == zeroSmartVector){
        *this = std::move(zeroSmartVector);
        return *this;
    }

    if (v == SmartVector(1)){
        return *this;
    }

    if (v._length == 1){
        Block carry = 0;

        const unsigned long long operand_only_digit = v._data[0];
        for (unsigned int i = _length; i > 0; i--){
            const unsigned long long current_digit = _data[i - 1];

            const unsigned long long product = current_digit * operand_only_digit + carry;

            _data[i - 1] = Block(product % BASE);

            carry = Block(product / BASE);
        }

        if (carry){
            extend_and_copy(1);
            _data[0] = carry;
        }
    }
    else{
        SmartVector result = SmartVector(0);

        for (unsigned int i = v._length; i > 0; i--){
            SmartVector current_digit = SmartVector(v._data[i - 1]);

            SmartVector current_product = (*this) * current_digit;

            if (current_product != zeroSmartVector){
                current_product.append_zero_blocks(v._length - i);
            }

            result += current_product;
        }

        *this = result;
    }

    return *this;
}
SmartVector& SmartVector::operator-=(const SmartVector& v){
    // It's supposed that *this >= v

    const unsigned int delta_length = (_length - v._length);

    unsigned int debt = 0;
    for (unsigned int i = _length; i > 0; i--){
        unsigned long long digit1 = _data[i - 1];

        const unsigned long long digit2 = i > delta_length ? v._data[i - delta_length - 1] : 0;

        if (debt){
            debt = !digit1;
            digit1 = digit1 ? digit1 - 1 : BASE - 1;
        }

        debt = debt || digit1 < digit2;
        digit1 += BASE * debt;

        _data[i - 1] = Block(digit1 - digit2);
    }

    reduce_first_zero_blocks();

    return *this;
}
SmartVector& SmartVector::operator/=(const SmartVector& v){
    *this = divide(v, false);
    return *this;
}
SmartVector& SmartVector::operator%=(const SmartVector& v){
    *this = divide(v, true);
    return *this;
}

SmartVector& SmartVector::operator^=(const SmartVector& v){
    *this = logic_operation(v, XOR_CODE);
    return *this;
}
SmartVector& SmartVector::operator&=(const SmartVector& v){
    *this = logic_operation(v, CONJUNCTION_CODE);
    return *this;
}
SmartVector& SmartVector::operator|=(const SmartVector& v){
    *this = logic_operation(v, DISJUNCTION_CODE);
    return *this;

}
SmartVector& SmartVector::operator~(){
    for (unsigned int i = _length; i > 0; i--){
        _data[i - 1] = ~(_data[i - 1]);
    }

    reduce_first_zero_blocks();

    return (*this);
}

SmartVector& SmartVector::operator++(){
    *this = *this + SmartVector(1);
    return *this;
}

bool SmartVector::operator==(const SmartVector& v) const{
    return !compare(v);
}
bool SmartVector::operator!=(const SmartVector& v) const{
    return compare(v);
}
bool SmartVector::operator>(const SmartVector& v) const{
    return compare(v) == 1;
}
bool SmartVector::operator>=(const SmartVector& v) const{
    return compare(v) >= 0;
}

SmartVector::operator int() const{
    int result = 0;
    auto m = BASE;
    decltype(m) current_m = 1;
    for (unsigned int i = _length; i > 0; i--, current_m *= m){
        result += int(_data[i - 1] * current_m);
    }

    return result;
}
SmartVector::operator std::string() const{
    NumericString result = NumericString(0);

    NumericString m = NumericString(BASE);
    NumericString current_m = NumericString(1);

    for (unsigned int i = _length; i > 0; i--, current_m = current_m * m){
        result += current_m * NumericString(_data[i - 1]);
    }

    return std::string(result);
}

size_t SmartVector::size() const{
    return sizeof(_length) + sizeof(_data) + _length * sizeof(Block);
}
int SmartVector::compare(const SmartVector& v) const{
    if (_length != v._length){
        return _length > v._length ? 1 : -1;
    }

    for (unsigned int i = 0; i < v._length; i++){
        if (_data[i] != v._data[i]){
            return _data[i] > v._data[i] ? 1 : -1;
        }
    }

    return 0;
}

SmartVector operator+(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, ADDITION_CODE);
}
SmartVector operator-(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, SUBTRACTION_CODE);
}
SmartVector operator*(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, MULTIPLICATION_CODE);
}
SmartVector operator/(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, DIV_CODE);
}
SmartVector operator%(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, MOD_CODE);
}
SmartVector operator^(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, XOR_CODE);
}
SmartVector operator&(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, CONJUNCTION_CODE);
}
SmartVector operator|(const SmartVector& v1, const SmartVector& v2){
    return operation(v1, v2, DISJUNCTION_CODE);
}
