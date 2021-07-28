#pragma once

#include <memory>

#include "Crystal/Core/PlatformDetection.h"

// DLL support
#ifdef CRYSTAL_PLATFORM_WINDOWS
	#if CRYSTAL_DYNAMIC_LINK
		#ifdef CRYSTAL_BUILD_DLL
			#define CRYSTAL_API __declspec(dllexport)
		#else
			#define CRYSTAL_API __declspec(dllimport)
		#endif
	#else
	#define CRYSTAL_API
	#endif
#else
	#error Hazel only supports Windows!
#endif // End of DLL support

#ifdef CRYSTAL_DEBUG
	#if defined(CRYSTAL_PLATFORM_WINDOWS)
		#define CRYSTAL_DEBUGBREAK() __debugbreak()
	#elif defined(Crystal_PLATFORM_LINUX)
		#include <signal.h>
		#define CRYSTAL_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debug break yet1"
	#endif
	#define CRYSTAL_ENABLE_ASSERTS
#endif

#ifdef CRYSTAL_ENABLE_ASSERTS
	#define CRYSTAL_CLIENT_ASSERT(x, ...) { if(!(x)) { CRYSTAL_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); CRYSTAL_DEBUGBREAK(); } }
	#define CRYSTAL_CORE_ASSERT(x, ...) { if(!(x)) { CRYSTAL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); CRYSTAL_DEBUGBREAK(); } }
#else
	#define CRYSTAL_CLIENT_ASSERT(x, ...)
	#define CRYSTAL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CRYSTAL_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Crystal {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
