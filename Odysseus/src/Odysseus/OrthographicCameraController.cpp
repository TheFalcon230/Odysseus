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

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::OnUpdate(Odysseus::Timestep time)
	{
		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}
		else if (Input::IsKeyPressed(KeyCode::D))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}
		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}
		else if (Input::IsKeyPressed(KeyCode::S))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * time.AsSeconds() * m_ZoomLevel;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KeyCode::Q))
				m_CameraRotation += m_CameraRotationSpeed * time.AsSeconds();
			else if (Input::IsKeyPressed(KeyCode::E))
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

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * .25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth() , (float)e.GetHeight());
		CalculateView();
		return false;
	}

}