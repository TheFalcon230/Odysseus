workspace "Odysseus"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "MyLib/include"

include "Odysseus/vendor/GLFW"

project "Odysseus"
	location "Odysseus"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "odcpch.h"
	pchsource "Odysseus/src/odcpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib",
		"GLFW.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ODC_PLATFORM_WINDOWS",
			"ODC_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ODC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ODC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ODC_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Odysseus/vendor/spdlog/include",
		"Odysseus/src"
	}

	links
	{
		"Odysseus"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ODC_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ODC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ODC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ODC_DIST"
		optimize "On"