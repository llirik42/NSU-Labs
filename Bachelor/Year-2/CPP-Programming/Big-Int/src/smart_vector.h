#pragma once

#include <iostream>
#include <cmath>

typedef unsigned char Block; // Block of array of SmartVector

inline const unsigned int BASE = (unsigned int) (pow(2, sizeof(Block) * 8)); // 2 ^ (size of Block in bits)

class SmartVector{
private:
    unsigned int _length;
    Block* _data;

    void extend_and_copy(const unsigned int& delta);
    void append_zero_blocks(const unsigned int& count);
    void reduce_first_blocks(const unsigned int& count);
    void reduce_first_zero_blocks();

    SmartVector& logic_operation(const SmartVector& operand, const unsigned char& operation_type);

    SmartVector divide(const SmartVector& operand, const bool& mod);
public:
    SmartVector();
    explicit SmartVector(unsigned long long n);
    explicit SmartVector(const std::string& s);
    SmartVector(SmartVector&& s) noexcept;
    SmartVector(const SmartVector& v);
    ~SmartVector();

    SmartVector& operator=(SmartVector&& v) noexcept;
    SmartVector& operator=(const SmartVector& v);

    SmartVector& operator+=(const SmartVector& v);
    SmartVector& operator*=(const SmartVector& v);
    SmartVector& operator-=(const SmartVector& v);
    SmartVector& operator/=(const SmartVector& v);
    SmartVector& operator%=(const SmartVector& v);

    SmartVector& operator^=(const SmartVector& v);
    SmartVector& operator&=(const SmartVector& v);
    SmartVector& operator|=(const SmartVector& v);
    SmartVector& operator~();

    SmartVector& operator++();

    bool operator==(const SmartVector& v) const;
    bool operator!=(const SmartVector& v) const;
    bool operator>(const SmartVector& v) const;
    bool operator>=(const SmartVector& v) const;

    explicit operator int() const;
    explicit operator std::string() const;

    [[nodiscard]] size_t size() const; // Size in bytes for object
    [[nodiscard]] int compare(const SmartVector& v) const; // Returns -1, 0, 1 if object <, == or > "b"
};

SmartVector operator+(const SmartVector& v1, const SmartVector& v2);
SmartVector operator-(const SmartVector& v1, const SmartVector& v2);
SmartVector operator*(const SmartVector& v1, const SmartVector& v2);
SmartVector operator/(const SmartVector& v1, const SmartVector& v2);
SmartVector operator%(const SmartVector& v1, const SmartVector& v2);
SmartVector operator^(const SmartVector& v1, const SmartVector& v2);
SmartVector operator&(const SmartVector& v1, const SmartVector& v2);
SmartVector operator|(const SmartVector& v1, const SmartVector& v2);
