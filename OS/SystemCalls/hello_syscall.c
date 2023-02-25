#include <syscall.h>
#include <unistd.h>

void my_write(int fd, void* buffer, unsigned int len){
	syscall(SYS_write, 1, buffer, len);
}

int main(){
	my_write(1, "Hello, world! (from sys call)\n", 30);
	return 0;
}
