
/*
 *	hooking library for Windows
 *	https://github.com/LoukaMB/hook
 */

#pragma once

#include <Windows.h>

typedef struct
{
	void* data;
	void* origin;
	void* jmpto;
	long isz;
	int on;
} hook;

/* Rapidly place hook at given address, jumping to address "to" */
hook* fasthook(void* addr, void* to);

/* Place hook at given address, taking account of instruction size "sz" */
hook* longhook(void* addr, void* to, long sz);

/* Prepare hook data without hooking */
hook* preparehook(void* addr, void* to, long sz);

/* Place hook from hook data */
int instatehook(hook* hk);

/* Remove hook from hook data, restoring original code */
int removehook(hook* hk);

/* Free hook structure*/
int freehook(hook* hk);