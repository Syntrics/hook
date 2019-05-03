
/*
 *	hooking library for Windows
 *	https://github.com/LoukaMB/hook
 */

#include "hook.h"

inline hook* fasthook(void* addr, void* to)
{
	hook* hk = preparehook(addr, to, 5);
	if (hk)
	{
		if (!instatehook(hk))
		{
			freehook(hk);
			return NULL;
		}
		return hk;
	}
	return NULL;
}

inline hook* longhook(void* addr, void* to, long sz)
{
	hook* hk = preparehook(addr, to, sz);
	if (hk)
	{
		if (!instatehook(hk))
		{
			freehook(hk);
			return NULL;
		}
		return hk;
	}
	return NULL;
}

hook* preparehook(void* addr, void* to, long sz)
{
	DWORD nperms = 0;
	hook* dt = malloc(sizeof(hook));
	if (!dt) return NULL;
	memset(dt, 0, sizeof(hook));
	dt->data = malloc(sz);
	if (!dt->data)
	{
		freehook(dt);
		return NULL;
	}
	dt->origin = addr;
	dt->jmpto = to;
	dt->isz = sz;
	dt->on = 0;

	if (!VirtualProtect(addr, sz, PAGE_EXECUTE_READWRITE, &nperms))
	{
		freehook(dt);
		return NULL;
	}
	memmove(dt->data, addr, sz);
	VirtualProtect(addr, sz, nperms, &nperms);
	return dt;
}

int instatehook(hook* hk)
{
	DWORD nperms = 0;
	if (!VirtualProtect(hk->origin, hk->isz, PAGE_EXECUTE_READWRITE, &nperms))
	{
		return 0;
	}
	memset(hk->origin, 0x90, hk->isz); /* 0x90 = NOP */
	*(unsigned char*)(hk->origin) = 0xE9;
	*(unsigned long*)(((unsigned long)hk->origin) + 1) = (unsigned long)(hk->jmpto) - (unsigned long)(hk->origin) - 5;
	VirtualProtect(hk->origin, hk->isz, nperms, &nperms);
	hk->on = 1;
	return 1;
}

int removehook(hook* hk)
{
	DWORD nperms = 0;
	if (!VirtualProtect(hk->origin, hk->isz, PAGE_EXECUTE_READWRITE, &nperms))
	{
		return 0;
	}
	memmove(hk->origin, hk->data, hk->isz);
	VirtualProtect(hk->origin, hk->isz, nperms, &nperms);
	hk->on = 0;
	return 1;
}

int freehook(hook* hk)
{
	if (!hk) return 0;
	if (hk->on) if (!removehook(hk)) return 0;
	free(hk->data);
	free(hk);
	return 1;
}
