#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	void* handle = 	dlopen("./dynamic_runtime_lib.so", RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    void (*print_pointer)() = dlsym(handle, "print_hello_from_dynamic_runtime_lib");

    dlerror();    /* Очистка всех результатов ошибок */

    print_pointer();

	dlclose(handle);
}
