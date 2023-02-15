#include <iostream>
#include "smart_vector.h"

class BigInt{
private:
    bool _is_positive;
    SmartVector _vector;

    void handle_minus_zero();
    BigInt& logic_operation(const BigInt& operand, const unsigned char& operation_type);
public:
    BigInt();
    explicit BigInt(const int& number);
    explicit BigInt(const std::string& str);
    BigInt(const BigInt& b);
    BigInt(BigInt&& b) noexcept;
    ~BigInt();

    BigInt& operator=(const BigInt& b);
    BigInt& operator=(BigInt&& b) noexcept;

    BigInt& operator+=(const BigInt& b);
    BigInt& operator*=(const BigInt& b);
    BigInt& operator-=(const BigInt& b);
    BigInt& operator/=(const BigInt& b); // Throws std::invalid_argument in case of division by 0

    // Throws std::invalid_argument in case of division by 0, of by negative number
    // Returns mathematically correct results if dividend < 0. For example, if x = -2, after x % 3, "x" is going to be 1
    BigInt& operator%=(const BigInt& b);

    BigInt operator+() const;
    BigInt operator-();
    BigInt& operator~(); // Saves the sign, but inverts absolute value on the base of 256

    BigInt& operator++();
    const BigInt operator++(int);
    BigInt& operator--();
    const BigInt operator--(int);

    bool operator==(const BigInt& b) const;
    bool operator!=(const BigInt& b) const;
    bool operator<(const BigInt& b) const;
    bool operator>(const BigInt& b) const;
    bool operator<=(const BigInt& b) const;
    bool operator>=(const BigInt& b) const;

    // These operations perform operations only with absolute values, sign of result will be like after multiplication
    BigInt& operator^=(const BigInt& b);
    BigInt& operator&=(const BigInt& b);
    BigInt& operator|=(const BigInt& b);

    explicit operator int() const; // If object > INT_MAX returns INT_MAX, if object < INT_MIN, returns INT_MIN
    explicit operator std::string() const;

    [[nodiscard]] size_t size() const; // Size in bytes for object
    [[nodiscard]] bool is_zero() const;
    [[nodiscard]] int compare(const BigInt& b) const; // Returns -1, 0, 1 if object <, == or > "b"
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);

std::ostream& operator<<(std::ostream&, const BigInt&);
std::istream& operator>>(std::istream&, BigInt&);
