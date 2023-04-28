# Тесты и функции

## Задание

Научиться писать функции для работы с массивами через тесты.

В приложенном файле есть набор тестов и заглушки для четырёх функций, помеченных коментарием **TODO**. Требуется реализовать эти функции так, чтобы программа завершалась без ошибок, т.е. все тесты проходили успешно.

```C
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <assert.h>

// *****************************************************************************************
// TODO Реализовать функцию
// Вернуть индекс минимального элемента, или -1, если массив пустой
int find_min(int array[], size_t size) {
	return 42;
}

// TODO Реализовать функцию
// Вычислить среднее арифметическое массива, принимает только непустой массив
float calculate_avg(int array[], size_t size) {
	return 42;
}

// TODO Реализовать функцию
// Отсортировать массив методом пузырька
void bubble_sort(int array[], size_t size) {

}

// TODO Реализовать функцию
// Поменять порядок элементов в массиве на противоположный
void reverse_array(int array[], size_t size) {

}

// *****************************************************************************************
// Тесты для написанных функций
#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

void test_min() {
	{
		int array[] = { 1, 2, 3 };
		assert(0 == find_min(array, ARR_SIZE(array)));
	}
	{
		int array[] = { 1, -2, 3 };
		assert(1 == find_min(array, ARR_SIZE(array)));
	}
	{
		int array[] = { 3, 2, 1 };
		assert(2 == find_min(array, ARR_SIZE(array)));
	}
	{
		int empty[] = { 1 };
		assert(-1 == find_min(empty, 0));
	}
	{
		assert(-1 == find_min(NULL, 2));
	}
}

void test_avg() {
	{
		int single[] = { 1 };
		assert(1 == calculate_avg(single, ARR_SIZE(single)));
	}
	{
		int several[] = { 1, 3, 5, 7 };
		assert(4 == calculate_avg(several, ARR_SIZE(several)));
	}
	{
		int odd[] = { 2, 4, 6};
		assert(4 == calculate_avg(odd, ARR_SIZE(odd)));
	}
	{
		int nonint[] = { 1, 2, 3, 4, 5 };
		assert((1 + 2 + 3 + 4 + 5) / 5 == calculate_avg(nonint, ARR_SIZE(nonint)));
	}
}

// Запустить сортировку пузырьком и проверить на отсортированность
int test_bubble_sample(int array[], size_t size) {
	bubble_sort(array, size);
	for (size_t i = 0; array && size && i < size - 1; i++) {
		if (array[i] > array[i + 1]) {
			printf("FAIL: Array is not sorted at element %d: %d > %d\n", i, array[i], array[i + 1]);
			return 0;
		}
	}
	return 1;
}

void test_bubble() {
	{
		int simple[] = { 1, 3, 2, 5, 4 };
		assert(test_bubble_sample(simple, ARR_SIZE(simple)));
	}
	{
		int reverse[] = { 5, 4, 3, 2, 1 };
		assert(test_bubble_sample(reverse, ARR_SIZE(reverse)));
	}
	{
		int empty[] = { 1 };
		assert(test_bubble_sample(empty, 0));
		assert(test_bubble_sample(NULL, 4));
	}
}

// Запустить обращение массива и проверить результат
int test_reverse_sample(int array[], size_t size, int expected[]) {
	reverse_array(array, size);
	if (!array || !size) {
		return 1;
	}
	for (size_t i = 0; i < size; i++) {
		if (array[i] != expected[i]) {
			printf("FAIL: Difference in element %d: got %d vs expected %d\n", i, array[i], expected[i]);
			return 0;
		}
	}
	return 1;
}

void test_reverse() {
	{
		int rising[] = { 1, 2, 3, 4, 5 };
		int expected[] = { 5, 4, 3, 2, 1 };
		assert(test_reverse_sample(rising, ARR_SIZE(rising), expected));
	}
	{
		int single[] = { 1 };
		int expected[] = { 1 };
		assert(test_reverse_sample(single, ARR_SIZE(single), expected));
	}
	{
		int random[] = { 1, 3, 2, 9, 0 };
		int expected[] = { 0, 9, 2, 3, 1 };
		assert(test_reverse_sample(random, ARR_SIZE(random), expected));
	}
	{
		int empty[] = { 1 };
		assert(test_reverse_sample(empty, 0, empty));
		assert(test_reverse_sample(NULL, 5, NULL));
	}
}

// *****************************************************************************************

void main() {
	test_min();
	test_avg();
	test_bubble();
	test_reverse();
}
```
