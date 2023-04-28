#include <iostream>

class NumericString{
private:
    unsigned int _length;
    char* _string;

    void append(char c, unsigned int count);
    void insert_front(char c);
    void add_zero(); // Inserts '0' at the beginning
    void erase_front(); // Erases first char
    void reduce_zeroes(); // Erases zeroes from the beginning until only one left

    [[nodiscard]] bool is_zero() const;
    [[nodiscard]] unsigned int get_length() const;

    //Ceil of log. If s = "0", returns 1
    friend unsigned int ceil_log_numerical_str(const unsigned long long& base, const NumericString& s);
public:
    NumericString();
    explicit NumericString(unsigned long long n);
    explicit NumericString(const std::string& s);
    NumericString(NumericString&& s) noexcept;
    NumericString(const NumericString& s);
    ~NumericString();

    NumericString& operator=(const NumericString& s);
    NumericString& operator=(NumericString&& s) noexcept;

    NumericString operator+(const NumericString& operand) const;
    NumericString operator*(const NumericString& operand) const;
    NumericString operator/(const unsigned long long& operand) const;
    unsigned long long operator%(const unsigned long long& operand) const;

    NumericString& operator+=(const NumericString& operand);
    NumericString& operator/=(const unsigned long long& operand);

    bool operator==(const std::string& s) const;
    bool operator==(const NumericString& s) const;
    bool operator!=(const std::string& s) const;
    bool operator!=(const NumericString& s) const;
    bool operator>(const NumericString& s) const;

    explicit operator unsigned long long() const;
    explicit operator std::string() const;
};
