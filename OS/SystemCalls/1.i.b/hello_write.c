#include <unistd.h>

int main(){
	write(1, "Hello, world! (from write)\n", 27);
	return 0;
}
