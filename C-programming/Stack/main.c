#include <stdio.h>
#include "stack.h"

#define check_expr(expr, expected) {\
	int result = (expr); \
	if (result != (expected)) {\
		printf("Ожидал:\t '" #expr "'\t to == '%d', получил '%d' \t(функция %s в строке %d).\n", \
			   (expected), result, __FUNCTION__, __LINE__);\
		error_count++;\
	}\
}

int error_count = 0;

void test_stack() {
    printf("\nТест 1\n");
    {
        struct Stack empty_stack;
        initStack(&empty_stack);
        check_expr(stackSize(&empty_stack), 0);
    }
    printf("\nТест 2\n");
    {
        struct Stack simple_stack;
        initStack(&simple_stack);

        push(&simple_stack, 5);
        check_expr(peek(&simple_stack), 5);
        check_expr(stackSize(&simple_stack), 1);
    }
    printf("\nТест 3\n");
    {
        struct Stack push_pop_stack;
        initStack(&push_pop_stack);

        push(&push_pop_stack, 5);
        check_expr(pop(&push_pop_stack), 5);
    }
    printf("\nТест 4\n");
    {
        struct Stack complex_stack;
        initStack(&complex_stack);
        for (int i = 1; i <= 80; i++) {
            push(&complex_stack, i);
        }
        for (int i = 80; i > 40; i--) {
            pop(&complex_stack);

        }
        check_expr(stackSize(&complex_stack), 40);
        check_expr(peek(&complex_stack), 40);
        check_expr(pop(&complex_stack), 40);
    }
}

int main() {
    test_stack();

    if (error_count) {
        printf("\n\n!!!!!!!!!! Обнаружены ошибки (см. выше), исправьте и попробуйте снова !!!!!!!!!!!!!!!\n");
        return 1;
    } else {
        printf("\n\n************ Все тесты прошли *************\n");
        return 0;
    }
}
