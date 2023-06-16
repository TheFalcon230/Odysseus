#pragma once
#include <Odysseus.h>
#include "Panels/HierarchyPanel.h"

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


		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveAs();
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

		glm::vec2 vec_ViewportSize = {0,0};
		bool bIsViewportFocused = true;
		bool bIsViewportHovered = true;
		bool bIsUsingGizmo;

		int iGizmoType = -1;

		HierarchyPanel hierarchyPanel;

		/*--------------------------------------------------------*/

		struct ProfileResult
		{
			const char* Name;
			float Time;

			ProfileResult(const char* name, float time)
				:Time(time), Name(name)
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