#include "odcpch.h"
#include "EditorCamera.h"

#include "Odysseus/Core/Input.h"
#include "Odysseus/Core/KeyCode.h"
#include "Odysseus/Core/MouseButtonCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Odysseus
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		:fFOV(fov), fAspectRatio(aspectRatio), fNearClip(nearClip), fFarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::Update(Timestep ts)
	{
		const glm::vec2 mouse{ Input::GetMouseX(), Input::GetMouseY()};
		glm::vec2 delta = (mouse - vecInitialMousePos) * 0.003f;
		vecInitialMousePos = mouse;

		if (Input::IsMouseButtonPressed(MouseCode::ButtonMiddle))
			MousePan(delta);
		if (Input::IsMouseButtonPressed(MouseCode::ButtonRight))
			MouseRotate(delta);
		if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
			MouseZoom(delta.y);

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ODC_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-fPitch, -fYaw, 0.0f));
	}

	void EditorCamera::UpdateProjection()
	{
		fAspectRatio = fViewportWidth / fViewportHeight;
		projectionMatrix = glm::perspective(glm::radians(fFOV), fAspectRatio, fNearClip, fFarClip);
	}

	void EditorCamera::UpdateView()
	{
		Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(orientation);
		viewMatrix = glm::inverse(viewMatrix);
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		vecFocalPoint += -GetRightDirection() * delta.x * xSpeed * fDistance;
		vecFocalPoint += GetUpDirection() * delta.y * ySpeed * fDistance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		fYaw += yawSign * delta.x * RotationSpeed();
		fPitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		fDistance -= delta * ZoomSpeed();
		if (fDistance < 1.0f)
		{
			vecFocalPoint += GetForwardDirection();
			fDistance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return vecFocalPoint - GetForwardDirection() * fDistance;
	}


	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(fViewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(fViewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = fDistance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.f);
		return speed;
	}
}