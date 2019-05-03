
/*
 *	hooking library for Windows
 *	https://github.com/LoukaMB/hook
 */

#ifdef _DEBUG
#pragma comment(lib, "../Debug/hook.lib")
#else
#pragma comment(lib, "../Release/hook.lib")
#endif
#include "../hook/hook.h"
#include <stdio.h>

int add(int a, int b)
{
	return a + b;
}

int sub(int a, int b)
{
	return a - b;
}

int main()
{
	hook* fn;

	printf("Return value before hook: %d\n", add(10, 5));
	fn = preparehook(add, sub, 5);
	if (instatehook(fn))
	{
		printf("Return value after hook: %d\n", add(10, 5));
		removehook(fn);
		printf("Return value after removing hook: %d\n", add(10, 5));
		freehook(fn);
		return 0;
	}

	printf("Failed to instate hook\n");
	return -1;
}