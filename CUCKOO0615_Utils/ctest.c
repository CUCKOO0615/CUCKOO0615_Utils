
#include "ctest.h"
#include <stdio.h>
#include <string.h>

int Test()
{
	const char* a = "11111111";
	char* b = a;

	b[4] = '2';
	printf("%s", b);

	return 0;
}
