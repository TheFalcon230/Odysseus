VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Odysseus/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Odysseus/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Odysseus/vendor/imgui"
IncludeDir["ImPlot"] = "%{wks.location}/Odysseus/vendor/implot"
IncludeDir["glm"] = "%{wks.location}/Odysseus/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Odysseus/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Odysseus/vendor/entt/include"
IncludeDir["shaderc"] = "%{wks.location}/Hazel/vendor/shaderc/include/shaderc"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Hazel/vendor/SPIRV-Cross"
IncludeDir["yaml_cpp"] = "%{wks.location}/Odysseus/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Odysseus/vendor/imguizmo"
IncludeDir["Icons"] = "%{wks.location}/Odysseus/vendor/Icons"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"