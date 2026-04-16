project "Odysseus"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "odcpch.h"
	pchsource "src/odcpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/imguizmo/ImGuizmo.h",
		"vendor/imguizmo/ImGuizmo.cpp",
		"vendor/imgui/imgui.h",
		"vendor/imgui/imgui.cpp",

		"vendor/implot/implot.h",
		"vendor/implot/implot.cpp",
		"vendor/implot/implot_demo.cpp",
		"vendor/implot/implot_internal.h",
		"vendor/implot/implot_items.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
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
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}


	filter "files:vendor/ImPlot/**.cpp"
	flags { "NoPCH" }

	filter "files:vendor/imgui/**.cpp"
	flags { "NoPCH" }

	filter "files:vendor/imguizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "ODC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ODC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ODC_DIST"
		runtime "Release"
		optimize "on"
