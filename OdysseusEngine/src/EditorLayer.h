#pragma once
#include <Odysseus.h>
#include "Panels/HierarchyPanel.h"
#include "Panels/WindowTitleBar/WindowTitleBar.h"
#include <imguizmo/ImGuizmo.h>

#include "Odysseus/Renderer/EditorCamera.h"

namespace Odysseus
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep updateTime) override;
		virtual void OnImGuiRender() override;

		void NewScene();
		void OpenScene();
		void SaveAs();

		void OnEvent(Event& e) override;

		bool GetCameraDebugEnabled() const { return bIsCameraDebugEnabled; }
		void SetCameraDebugEnabled(bool enabled) { bIsCameraDebugEnabled = enabled; }
		bool GetProfilerEnabled() const { return bIsProfilerEnabled; }
		void SetProfilerEnabled(bool enabled) { bIsProfilerEnabled = enabled; }

		void SetSnapTranslation(bool snap) { bSnapTranslation = snap; }
		void SetSnapRotation(bool snap) { bSnapRotation = snap; }
		void SetSnapScale(bool snap) { bSnapScale = snap; }

		bool GetSnapTranslation() const { return bSnapTranslation; }
		bool GetSnapRotation() const { return bSnapRotation; }
		bool GetSnapScale() const { return bSnapScale; }

		void SetSelectionGizmoType() { iGizmoType = (ImGuizmo::OPERATION) - 1; }
		void SetTranslateGizmoType() { iGizmoType = ImGuizmo::TRANSLATE; }
		void SetRotateGizmoType() { iGizmoType = ImGuizmo::ROTATE; }
		void SetScaleGizmoType() { iGizmoType = ImGuizmo::SCALE; }

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		static float DrawFloatControl(const std::string& label, float currentValue, float resetValue = 0.0f, float min = 0.0f, float max = 100.0f, float columnWidth = 100.0f);
	private:
		OrthographicCameraController m_CameraController;

		//Temp variables
		ShaderLibrary m_ShaderLibrary;
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> T_Spritesheet;
		Ref<Sprite> Sp_Bush_01;
		Ref<QuadProperties> testQuad;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> activeScene;
		Object ETestSquare, ECamera;

		EditorCamera mainCameraEditor;

		Timestep time;

		std::unordered_map<char, Ref<Sprite>> s_TextureMap;
		uint32_t mapWidth = 24, mapHeight = 12;

		bool bIsViewportFocused = true;
		bool bIsViewportHovered = true;
		glm::vec2 vec_ViewportSize = {0,0};
		glm::vec2 vec_ViewportBounds[2];

		Object hoveredObject;

		bool bIsUsingGizmo;

		bool bIsCameraDebugEnabled = false;
		bool bIsProfilerEnabled = false;

		ImGuizmo::OPERATION iGizmoType = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE iGizmoMode = ImGuizmo::LOCAL;

		bool bSnapTranslation = false;
		bool bSnapRotation = false;
		bool bSnapScale = false;

		HierarchyPanel hierarchyPanel;
		WindowTitleBar windowTitleBar;

		/*--------------------------------------------------------*/

		struct ProfileResult
		{
			const char* Name;
			float ExecutionTime;

			ProfileResult(const char* name, float time)
				:ExecutionTime(time), Name(name)
			{

			}
		};

		struct ScrollingBuffer
		{
			int MaxSize;
			int Offset;
			std::vector<glm::vec2> Data;
			ScrollingBuffer(int max_size = 2000) {
				MaxSize = max_size;
				Offset = 0;
				Data.reserve(MaxSize);
			}
			void AddPoint(float time, float execTime) {
				if (Data.size() < MaxSize)
					Data.push_back({ time, execTime });
				else {
					Data[Offset] = { time, execTime };
					Offset = (Offset + 1) % MaxSize;
				}
			}
			void Erase() {
				if (Data.size() > 0) {
					Data.clear();
					Offset = 0;
				}
			}
		};

		std::vector<ProfileResult> m_ProfileResults;

		glm::vec4 m_SquareColor = { 1.f,1.f,1.f ,1.f };
	};

}