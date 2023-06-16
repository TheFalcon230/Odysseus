workspace "Odysseus"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Odysseus/vendor/GLFW/include"
IncludeDir["GLAD"] = "Odysseus/vendor/GLAD/include"
IncludeDir["ImGui"] = "Odysseus/vendor/imgui"
IncludeDir["ImPlot"] = "Odysseus/vendor/implot"
IncludeDir["glm"] = "Odysseus/vendor/glm"
IncludeDir["stb_image"] = "Odysseus/vendor/stb_image"
IncludeDir["entt"] = "Odysseus/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Odysseus/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Odysseus/vendor/imguizmo"

group "Dependencies"
	include "Odysseus/vendor/GLFW"
	include "Odysseus/vendor/GLAD"
	include "Odysseus/vendor/imgui"
	include "Odysseus/vendor/yaml-cpp"
group ""

project "Odysseus"
	location "Odysseus"
	kind "SharedLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "odcpch.h"
	pchsource "Odysseus/src/odcpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/imguizmo/ImGuizmo.h",
		"%{prj.name}/vendor/imguizmo/ImGuizmo.cpp",

		"%{prj.name}/vendor/implot/implot.h",
		"%{prj.name}/vendor/implot/implot.cpp",
		"%{prj.name}/vendor/implot/implot_demo.cpp",
		"%{prj.name}/vendor/implot/implot_internal.h",
		"%{prj.name}/vendor/implot/implot_items.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"

	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
	}


	filter "files:%{wks.location}/vendor/ImPlot/**.cpp"
	flags { "NoPCH" }
	filter "files:files:vendor/imguizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ODC_PLATFORM_WINDOWS",
			"ODC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/OdysseusEngine")
		}

	filter "configurations:Debug"
		defines "ODC_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ODC_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ODC_DIST"
		buildoptions "/MD"
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
		"Odysseus/src",
		"Odysseus/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
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
			"ODC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "ODC_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ODC_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ODC_DIST"
		buildoptions  "/MD"
		optimize "On"


project "OdysseusEngine"
		location "OdysseusEngine"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
	
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
			"Odysseus/src",
			"Odysseus/vendor",
			"%{IncludeDir.glm}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.ImGuizmo}"
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
				"ODC_PLATFORM_WINDOWS",
			}
	
		filter "configurations:Debug"
			defines "ODC_DEBUG"
			buildoptions "/MDd"
			symbols "On"
	
		filter "configurations:Release"
			defines "ODC_RELEASE"
			buildoptions "/MD"
			optimize "On"
	
		filter "configurations:Dist"
			defines "ODC_DIST"
			buildoptions  "/MD"
			optimize "On"