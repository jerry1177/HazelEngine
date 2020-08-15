#pragma once
#include <memory>
#ifdef _WIND32
	#ifdef _WIND64
		#define HZ_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif // _WIND64

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	
	/*
		TARGET_OS_MAC exists on all the platforms
		so we must check all of them (in this order)
		to unsure that we are running on MAC
		and not some other apple platform
	*/
	#if TARGET_IPHONE_SIMULATION == 1
		#error "IOS simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported"
	#elif TARGET_OS_MAC == 1
		#define HZ_PLATFORM_MACOS
		#error "MACOS is not supported"
	#else
		#error "Unkown Apple platform"
	#endif
#elif defined(__ANDROID__)
	#define HZ_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined (__linux__)
	#define HZ_PLATFORM_LINUX
	#error "Linux is not supported"
#else
	#error "Unknown platform"
#endif // _WIND32






// dll support

#ifdef HZ_PLATFORM_WINDOWS
	#if HZ_DYNAMIC_LINK
		#ifdef HZ_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)
		#else
			#define HAZEL_API __declspec(dllimport)
		#endif // HZ_BUILD_DLL
	#else
		#define HAZEL_API
	#endif // HZ_DYNAMIC_LINK

#else
	#error Hazel only supports windows!
#endif // HZ_PLATFORM_WINDOWS

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if (!(x)){ HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if (!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)

#endif // HZ_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;
}