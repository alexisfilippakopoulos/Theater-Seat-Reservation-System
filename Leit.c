#include <stdio.h>

int main(int argc, char** argv) {
	int i;
	i = 10;
	int *ptr = &i;
	printf("*ptr = %d \n", *ptr);
	return 0;
}
