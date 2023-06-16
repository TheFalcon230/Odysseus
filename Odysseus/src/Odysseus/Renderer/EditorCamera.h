#pragma once
#include "Camera.h"
#include "Odysseus/Core/Timestep.h"
#include "Odysseus/Events/Event.h"
#include "Odysseus/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Odysseus
{
	class ODYSSEUS_API EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRation, float nearClip, float farClip);

		void Update(Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const {return fDistance;}
		inline void SetDistance(float distance) { fDistance = distance; }

		inline void SetViewportSize(float width, float height) { fViewportWidth = width; fViewportHeight = height; }

		const glm::mat4 GetViewMatrix() const { return viewMatrix; }
		glm::mat4 GetViewProjection() const { return projectionMatrix * viewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3 GetPosition() const { return Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return fPitch; }
		float GetYaw() const { return fYaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float fFOV = 45.0f, fAspectRatio = 16.0f / 9.0f, fNearClip = 0.1f, fFarClip = 1000.0f;

		glm::mat4 viewMatrix;
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f }, vecFocalPoint = {0.0f, 0.0f, 0.0f};

		glm::vec2 vecInitialMousePos;

		float fDistance = 10.0f;
		float fPitch = 0.0f, fYaw = 0.0f;

		float fViewportWidth = 1280, fViewportHeight = 720;
	};

}
