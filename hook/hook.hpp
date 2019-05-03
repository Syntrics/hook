
/*
 *	hooking library for Windows
 *	https://github.com/LoukaMB/hook
 */

#pragma once

/* C++ wrapper */

extern "C"
{
	#include "hook.h"
}

#include <exception>
#include <type_traits>

namespace hk
{
	class hook
	{
	private:
		::hook* internalhook;
	public:
		template <typename T>
		static hook* create(T addr, T to)
		{
			if (!std::is_pointer<T>::value)
				throw std::exception("attempt to pass non-pointer value to hook::fast");
			return new hook(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(to));
		}

		template <typename T>
		static hook* fast(T addr, T to)
		{
			if (!std::is_pointer<T>::value)
				throw std::exception("attempt to pass non-pointer value to hook::fast");
			auto v = new hook(reinterpret_cast<void*>(addr), reinterpret_cast<void*>(to));
			v->instate();
			return v;
		}

		bool instate() const
		{
			return static_cast<bool>(instatehook(internalhook));
		}

		bool remove() const
		{
			return static_cast<bool>(removehook(internalhook));
		}

		void* addr() const
		{
			return internalhook->origin;
		}

		void* to() const
		{
			return internalhook->jmpto;
		}

		hook(void* addr, void* to, long sz = 5)
		{
			internalhook = preparehook(addr, to, sz);
		}

		~hook()
		{
			freehook(internalhook);
		}
	};
}