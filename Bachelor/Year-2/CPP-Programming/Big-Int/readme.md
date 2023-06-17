# Большое целое число BigInt

## Задание

Реализовать большое целое число произвольной точности и набор операций над ним.

## Требования

1. Есть билд через *CMake* и тесты через *GoogleTest*, проект собирается без IDE.
2. Для хранения данных длинного числа не используются стандартные контейнеры (включая `std::string`). Данные хранятся в поле типа "сырого" массива/указателя (непосредственно тип данных можете выбрать сами, принимается любой целочисленный тип, включая `char*`, `int*`, и т.д.). Способ хранения данных (по степеням 10 или 2, а может быть придумаете что-то еще) — на ваше усмотрение.
3. Реализованы все особые методы:
	* конструктор без параметров
	* конструктор из целочисленного типа
	* конструктор из строки (десятичной записи числа в строке)
	* конструктор копирования
	* конструктор перемещения
	* копирующий оператор присваивания
	* перемещающий оператор присваивания
	* деструктор
4. Вместо ~~malloc()~~ использовать `new`, вместо ~~free()~~ использовать `delete`.
5. Реализованы следующие операторы:
	* приведение к строке (десятичная запись) и целочисленному типу. В последнем случае, если не влазит в тип, делаем обрезку (кладём в результат только младшие разряды)
	* `+`, `-`, `*`, `+=`, `-=`, `*=`
	* `++`, `--` (префиксная и инфиксная формы)
	* `+`, `-` (унарные)
	* все сравнения: `==`, `!=`, `<`, `<=`, `>`, `>=` 
	* вывод в поток: `std::ostream& operator<<(std::ostream& o, const BigInt& i)`
6. Написаны тесты *GoogleTest* на конструкторы и все операции.
7. Написаны комментарии к основным методам.
8. Чистый хорошо оформленный код, по возможности без дублирования.

## Дополнительные требования

1. Операторы `/`, `/=`, `%`, `%=`.
2. Битовые операции: `~`, `^`, `&`, `|`, `^=`, `&=`, `|=` (можно реализовать без учета семантики дополнительного кода для отрицательных значений).
3. Битовые сдвиги `<<`, `>>`, `<<=`, `>>=`.
4. Оператор ввода из потока с корректной обработкой ошибок.
5. Метод `int BigInt::compare(const BigInt&) const`, который возвращает (-1, 0, 1) в зависимости от того, число меньше, равно или больше аргумента. Если будете это делать, все операторы сравнения должны быть реализованы через неё.
6. Безопасная с точки зрения исключений реализация. Это значит, что при выбросе исключения в процессе работы метода (например, при вызове оператора `new` может быть исключение) все объекты остаются в консистентном состоянии, и не происходит утечек памяти.

## Реализация

```C++
class BigInt {
public:
  	BigInt();
  	BigInt(int);
  	BigInt(std::string); // бросать исключение std::invalid_argument при ошибке
  	BigInt(const BigInt&);
  	BigInt(BigInt&&);
  	~BigInt();

 	BigInt& operator=(const BigInt&);  //возможно присваивание самому себе!
 	BigInt& operator=(BigInt&&);

 	BigInt operator~() const;

 	BigInt& operator++();
 	const BigInt operator++(int) const;
 	BigInt& operator--();
 	const BigInt operator--(int) const;

 	BigInt& operator+=(const BigInt&);
 	BigInt& operator*=(const BigInt&);
 	BigInt& operator-=(const BigInt&);
 	BigInt& operator/=(const BigInt&);
 	BigInt& operator^=(const BigInt&);
 	BigInt& operator%=(const BigInt&);
 	BigInt& operator&=(const BigInt&);
 	BigInt& operator|=(const BigInt&);

 	BigInt operator+() const;  // unary +
 	BigInt operator-() const;  // unary -

 	bool operator==(const BigInt&) const;
 	bool operator!=(const BigInt&) const;
 	bool operator<(const BigInt&) const;
 	bool operator>(const BigInt&) const;
 	bool operator<=(const BigInt&) const;
 	bool operator>=(const BigInt&) const;

	operator int() const;
 	operator std::string() const;

 	size_t size() const;  // size in bytes

 	int compare(const BigInt&) const;
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);


std::ostream& operator<<(std::ostream& o, const BigInt& i);
```
