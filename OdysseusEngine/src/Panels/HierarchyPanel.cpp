#include "HierarchyPanel.h"
#include "Odysseus/Scene/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <glm/gtc/type_ptr.hpp>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace Odysseus
{
	extern const std::filesystem::path g_AssetsDirectory;

	HierarchyPanel::HierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void HierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		Context = context;
		OSelectionContext = {};
	}

	void HierarchyPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Hierarchy"))
		{
			if (Context)
			{
				Context->registry.each([&](auto objectID)
				{
					Object object = Object(objectID, Context.get());
					DrawObjectNode(object);
				}
				);

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					OSelectionContext = {};

				if (ImGui::BeginPopupContextWindow(0, 1))
				{
					if (ImGui::MenuItem("Add Empty Gameobject"))
					{
						Context->CreateObject("New Gameobject");
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Square"))
					{
						auto square = Context->CreateSquare("Square");
						square.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
					}
					if (ImGui::MenuItem("Cube"))
					{
						Context->CreateCube("Cube");
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Point Light"))
					{
						Context->CreatePointLight("Point Light");
					}
					if (ImGui::MenuItem("Directional Light"))
					{
						Context->CreateDirectionalLight("Directional Light");
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Details"))
		{
			if (OSelectionContext)
			{
				DrawComponents(OSelectionContext);
			}
			ImGui::End();
		}
	}

	void HierarchyPanel::DrawObjectNode(Object object)
	{
		auto& tc = object.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((OSelectionContext == object) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, tc.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			OSelectionContext = object;
		}

		bool isObjectDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Gameobject"))
				isObjectDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (isObjectDeleted)
		{
			if (OSelectionContext == object)
				OSelectionContext = {};
			Context->DestroyObject(object);
		}
	}

	static void DrawVec3Controls(const std::string& label, glm::vec3& values, float resetValues = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Style.FontSizeBase + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.15f, 0.015f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.25f, 0.025f, 1.0f));
		ImGui::PushFont(boldFont, 14.0f);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValues;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%0.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(2);
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.43f, 0.65f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.53f, 0.75f, 0.1f, 1.0f));
		ImGui::PushFont(boldFont, 14.0f);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValues;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%0.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(2);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.51f, 0.96f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.63f, 0.85f, 0.2f, 1.0f));
		ImGui::PushFont(boldFont, 14.0f);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValues;
		}
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%0.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(2);

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	static void DrawFloatControl(const std::string& label, float& currentValue, float resetValue, float min, float max, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImVec2 avail = ImGui::GetContentRegionAvail();

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, avail.x * 0.5f);
		if (ImGui::Button(label.c_str(), ImVec2(avail.x * 0.5f, 0.0f)))
		{
			currentValue = resetValue;
		}
		ImGui::NextColumn();

		ImGui::SetColumnWidth(0, avail.x * 0.5f);

		float lineHeight = GImGui->Style.FontSizeBase + GImGui->Style.FramePadding.y * 2;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::SameLine();
		ImGui::DragFloat("##float", &currentValue, 0.1f, min, max, "%0.2f");

		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Object object, UIFunction function)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (object.HasComponent<T>())
		{
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
			auto& component = object.GetComponent<T>();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			bool isComponentRemoved = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					isComponentRemoved = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				function(component);
				ImGui::TreePop();
			}

			if (isComponentRemoved)
				object.RemoveComponent<T>();
		}
	}

	void HierarchyPanel::DrawComponents(Object object)
	{
		if (object.HasComponent<TagComponent>())
		{
			auto& tag = object.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				OSelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				OSelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Point light"))
			{
				OSelectionContext.AddComponent<PointLightComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Directional Light"))
			{
				if (!OSelectionContext.HasComponent<PointLightComponent>())
				{
					OSelectionContext.AddComponent<DirectionalLightComponent>();
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", object, [](TransformComponent& component)
		{
			DrawVec3Controls("Position", component.Position);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Controls("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Controls("Scale", component.Scale, 1.0f);
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", object, [](SpriteRendererComponent& component)
		{
			ImVec2 avail = ImGui::GetContentRegionAvail();
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, avail.x * 0.5f);
			ImGui::Text("Albedo");
			ImGui::NextColumn();
			if (ImGui::ImageButton("##Albedo", (ImTextureID)(component.Albedo ? component.Albedo->GetRendererID() : 0), ImVec2{ 64, 64 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				//Clears texture
				component.Albedo = nullptr;
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetsDirectory)/ path;
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
						component.Albedo = texture;
					else
						ODC_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, avail.x * 0.5f);
			ImGui::Text("Normal Map");
			ImGui::NextColumn();
			if (ImGui::ImageButton("##Normal", (ImTextureID)(component.NormalMap ? component.NormalMap->GetRendererID() : 0), ImVec2{ 64, 64 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				//Clears texture
				component.NormalMap = nullptr;
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetsDirectory)/ path;
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
						component.NormalMap = texture;
					else
						ODC_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, avail.x * 0.5f);
			ImGui::Text("ORM Map");
			ImGui::NextColumn();
			if (ImGui::ImageButton("##ORM", (ImTextureID)(component.ORMMap ? component.ORMMap->GetRendererID() : 0), ImVec2{ 64, 64 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				//Clears texture
				component.ORMMap = nullptr;
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetsDirectory)/ path;
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
						component.ORMMap = texture;
					else
						ODC_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::Columns(1);
			DrawFloatControl("Tiling Factor", component.TilingFactor, 1.0f, 0.5f, 10.0f, 100.0f);
			DrawFloatControl("Roughness factor", component.Roughness, 0.5f, 0.0f, 1.0f, 100.0f);
			DrawFloatControl("Metallic factor", component.Metallic, 0.5f, 0.0f, 1.0f, 100.0f);
			DrawFloatControl("AO factor", component.AO, 0.0f, 0.0f, 1.0f, 100.0f);

		});

		DrawComponent<PointLightComponent>("Point Light", object, [](auto& component) {ImGui::ColorEdit4("Color", glm::value_ptr(component.Color)); ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, 0.1f, 100.0f); });

		DrawComponent<DirectionalLightComponent>("Directional Light", object, [](auto& component) {ImGui::ColorEdit4("Color", glm::value_ptr(component.Color)); ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, 0.1f, 100.0f); });


		DrawComponent<CameraComponent>("Camera", object, [](CameraComponent& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Use this camera as main camera", &component.bIsMainCamera);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						component.Camera.SetProjectionType((SceneCamera::EProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			SceneCamera::EProjectionType cameraProjectionType = component.Camera.GetProjectionType();


			if (cameraProjectionType == SceneCamera::EProjectionType::Perspective)
			{
				float perspectiveSize = glm::degrees(camera.GetPerspectiveFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveSize))
					camera.SetPerspectiveFOV(glm::radians(perspectiveSize));

				float perspectiveNearClip = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near clip", &perspectiveNearClip))
					camera.SetPerspectiveNear(perspectiveNearClip);

				float perspectiveFarClip = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat(" ", &perspectiveFarClip))
					camera.SetPerspectiveFar(perspectiveFarClip);
			}

			if (cameraProjectionType == SceneCamera::EProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near clip", &orthoNearClip))
					camera.SetOrthographicNear(orthoNearClip);

				float orthoFarClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat(" ", &orthoFarClip))
					camera.SetOrthographicFar(orthoFarClip);

			}
		});
	}

}