
/*
 *	hooking library for Windows
 *	https://github.com/LoukaMB/hook
 */

#ifdef _DEBUG
#pragma comment(lib, "../Debug/hook.lib")
#else
#pragma comment(lib, "../Release/hook.lib")
#endif
#include "../hook/hook.hpp"
#include <iostream>

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
	using hook = hk::hook;
	std::cout << "Return value before hook: " << add(10, 5) << std::endl;
	auto fn = hook::create(add, sub);
	if (fn->instate())
	{
		std::cout << "Return value after hook: " << add(10, 5) << std::endl;
		fn->remove();
		std::cout << "Return value after removing hook: " << add(10, 5) << std::endl;
		return 0;
	}

	std::cout << "Failed to instate hook" << std::endl;
	return -1;
}