#include "odcpch.h"
#include "OrthographicCameraController.h"

#include "Odysseus/Core/Input.h"
#include "Odysseus/Core/KeyCode.h"


namespace Odysseus
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), m_Rotation(rotation), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Odysseus::Timestep time)
	{
		if (Input::IsKeyPressed(ODC_KEY_A))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}
		else if (Input::IsKeyPressed(ODC_KEY_D))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}
		if (Input::IsKeyPressed(ODC_KEY_W))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}
		else if (Input::IsKeyPressed(ODC_KEY_S))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(ODC_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * time.AsSeconds();
			else if (Input::IsKeyPressed(ODC_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * time.AsSeconds();

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ODC_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ODC_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * .25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}