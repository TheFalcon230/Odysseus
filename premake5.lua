include "./MyLib/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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