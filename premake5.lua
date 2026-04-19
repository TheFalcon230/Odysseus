include "./MyLib/premake/premake_customization/solution_items.lua"

workspace "Odysseus"
	architecture "x64"
	startproject "OdysseusEngine"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Odysseus/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Odysseus/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Odysseus/vendor/imgui"
IncludeDir["ImPlot"] = "%{wks.location}/Odysseus/vendor/implot"
IncludeDir["glm"] = "%{wks.location}/Odysseus/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Odysseus/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Odysseus/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Odysseus/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Odysseus/vendor/imguizmo"
IncludeDir["Icons"] = "%{wks.location}/Odysseus/vendor/Icons"

group "Dependencies"
	include "MyLib/premake"
	include "Odysseus/vendor/GLFW"
	include "Odysseus/vendor/GLAD"
	include "Odysseus/vendor/imgui"
	include "Odysseus/vendor/yaml-cpp"
group ""

include "Odysseus"
include "Sandbox"
include "OdysseusEngine"