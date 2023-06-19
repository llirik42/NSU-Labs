#pragma once

#include <aio.h>
#include <bits/types/FILE.h>

#define ERROR_CODE (-1)
#define SUCCESS_CODE 0

/*
 * RETURN VALUE: SUCCESS_CODE — success, ERROR_CODE — can't init my_heap and errno is set appropriately.
 */
int init_my_heap();


/*
 * RETURN VALUE: SUCCESS_CODE — success, ERROR_CODE — can't destroy my_heap and errno is set appropriately or
 * my_heap isn't initialized.
 */
int destroy_my_heap();

/*
 * RETURN VALUE: NULL — not enough memory or my_heap isn't initialized, non-NULL — else.
 */
void* my_malloc(size_t size);


/*
 * RETURN VALUE: ERROR_CODE — invalid pointer or my_heap isn't initialized, SUCCESS_CODE — else.
 *
 * It's not guaranteed that in case of invalid pointer ERROR code will be returned.
 * But if ERROR_CODE is returned — pointer was invalid.
 */
int my_free(void* ptr);

/*
 * RETURN VALUE: SUCCESS_CODE — success, ERROR_CODE — I/O error and errno is set appropriately or my_heap isn't
 * initialized.
 */
int dump_my_heap(FILE* file);
