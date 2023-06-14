#include "HierarchyPanel.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Odysseus
{
	HierarchyPanel::HierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void HierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		Context = context;
	}

	void HierarchyPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Hierarchy"))
		{
			Context->registry.each([&](auto objectID)
				{
					Object object = Object(objectID, Context.get());
			DrawObjectNode(object);
				}
			);

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				OSelectionContext = {};

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
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, tc.Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			OSelectionContext = object;
		}
		if (opened)
		{
			ImGui::TreePop();
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
			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}
		if (object.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = object.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.01f);
				ImGui::DragFloat("Rotation", &(transform[2][2]));
				ImGui::DragFloat2("Scale", glm::value_ptr(transform[1]), 0.125f);
				ImGui::TreePop();
			}
		}
		if (object.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite renderer"))
			{
				auto& src = object.GetComponent<SpriteRendererComponent>();
				ImGui::Text("WIP");
				ImGui::TreePop();
			}

		}
		if (object.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cc = object.GetComponent<CameraComponent>();
				auto& camera = cc.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)cc.Camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							cc.Camera.SetProjectionType((SceneCamera::EProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				SceneCamera::EProjectionType cameraProjectionType = cc.Camera.GetProjectionType();
				

				if (cameraProjectionType == SceneCamera::EProjectionType::Perspective)
				{
					float perspectiveSize = glm::degrees(camera.GetPerspectiveSize());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveSize))
						camera.SetPerspectiveSize(glm::radians(perspectiveSize));

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

				ImGui::TreePop();
			}
		}
	}

}