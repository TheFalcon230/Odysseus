project "OdysseusEngine"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
	
		targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
		files
		{
			"src/**.h",
			"src/**.cpp"
		}
	
		includedirs
		{
			"%{wks.location}/Odysseus/vendor/spdlog/include",
			"%{wks.location}/Odysseus/src",
			"%{wks.location}/Odysseus/vendor",
			"%{IncludeDir.glm}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.ImPlot}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.ImGuizmo}"
		}
	
		links
		{
			"Odysseus"
		}
	
		filter "system:windows"
			systemversion "latest"
	
		filter "configurations:Debug"
			defines "ODC_DEBUG"
			runtime "Debug"
			symbols "On"
	
		filter "configurations:Release"
			defines "ODC_RELEASE"
			runtime "Release"
			optimize "On"
	
		filter "configurations:Dist"
			defines "ODC_DIST"
			runtime "Release"
			optimize "On"