#pragma once
#include "Renderer/Camera.h"
#include "Core/Timestep.h"

#include "Odysseus/Events/MouseEvent.h"
#include "Odysseus/Events/ApplicationEvent.h"

namespace Odysseus
{
	class ODYSSEUS_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep time);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }
		float GetZoomLevel() { return m_ZoomLevel; }
	private:
		void CalculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio = 16.f/9.f;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.f,0.f,0.f };
		float m_CameraMoveSpeed = 5.f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 90.0f;

		/// <summary>
		/// Does the camera rotation is active.
		/// </summary>
		bool m_Rotation;
	};
}