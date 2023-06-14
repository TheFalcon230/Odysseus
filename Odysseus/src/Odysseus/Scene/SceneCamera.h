#pragma once

#include "Odysseus/Renderer/Camera.h"

namespace Odysseus
{
	class ODYSSEUS_API SceneCamera : public Camera
	{
	public:
		enum class EProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);
		void SetViewportSize(uint32_t newWidth, uint32_t newHeight);

		float GetPerspectiveSize() const { return fPerspectiveFOV; }
		void SetPerspectiveSize(float newFOV) { fPerspectiveFOV = newFOV; RecalculateProjection(); }

		float GetPerspectiveNearClip() const { return fPerspectiveNear; }
		void SetPerspectiveNear(float newNearClip) { fPerspectiveNear = newNearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return fPerspectiveFar; }
		void SetPerspectiveFar(float newFarClip) { fPerspectiveFar = newFarClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return fOrthographicSize; }
		void SetOrthographicSize(float newSize) { fOrthographicSize = newSize; RecalculateProjection(); }

		float GetOrthographicNearClip() const { return fOrthographicNear; }
		void SetOrthographicNear(float newNearClip) { fOrthographicNear = newNearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return fOrthographicFar; }
		void SetOrthographicFar(float newFarClip) { fOrthographicFar = newFarClip; RecalculateProjection(); }

		EProjectionType GetProjectionType() { return projectionType; }
		void SetProjectionType(EProjectionType newProjectionType) { projectionType = newProjectionType; RecalculateProjection(); }
	private: //For private functions
		void RecalculateProjection();
	private: // For private variables
		EProjectionType projectionType = EProjectionType::Orthographic;

		float fOrthographicSize = 10.0f;
		float fOrthographicNear = -1.0f;
		float fOrthographicFar = 1.0f;

		float fPerspectiveFOV = glm::radians(60.f);
		float fPerspectiveNear = 0.01f;
		float fPerspectiveFar = 10000.0f;

		float fAspectRatio = 1.0f;
	};

}
