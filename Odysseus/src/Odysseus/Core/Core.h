#pragma once
#include <memory>


#ifdef ODC_PLATFORM_WINDOWS
		#ifdef ODC_BUILD_DLL
			#define ODYSSEUS_API __declspec(dllexport)
			#define IMGUI_API __declspec(dllexport)
			#define IMPLOT_API __declspec(dllexport)
		#else
			#define ODYSSEUS_API __declspec(dllimport)
			#define IMGUI_API __declspec(dllimport)
			#define IMPLOT_API __declspec(dllimport)
		#endif // ODC_BUILD_DLL
#else
		#error Odysseus only support Winwdow!
#endif // ODC_PLATFORM_WINDOWS

#ifdef ODC_ENABLE_ASSERTS
	#define ODC_ASSERTS(x, ...) {if(!(x)){ ODC_ERROR("Assertion Failed: {0}", __VAR_ARGS__); __debugbreak();}}
	#define ODC_CORE_ASSERT(x, ...) {if(!(x)){ ODC_CORE_ERROR("Assertion Failed: {0}", __VAR_ARGS__); __debugbreak();}}
#else
	#define ODC_ASSERT(x, ...)
	#define ODC_CORE_ASSERT(x, ...)
#endif // ODC_ENABLE_ASSERTS


#define BIT(x) ( 1 << x )

#define ODC_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Odysseus
{
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