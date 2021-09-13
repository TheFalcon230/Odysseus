#pragma once


#ifdef ODC_PLATFORM_WINDOWS
	#ifdef ODC_BUILD_DLL
		#define ODYSSEUS_API __declspec(dllexport)
	#else
		#define ODYSSEUS_API __declspec(dllimport)
	#endif // ODC_BUILD_DLL
#else
	#error Odysseus only support Winwdow!
#endif // ODC_PLATFORM_WINDOWS
