#include "odcpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Odysseus
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		fOrthographicSize = size;
		fOrthographicNear = nearClip;
		fOrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t newWidth, uint32_t newHeight)
	{
		fAspectRatio = (float)newWidth / (float)newHeight;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -fOrthographicSize * fAspectRatio * 0.5f;
		float orthoRight = fOrthographicSize * fAspectRatio * 0.5f;
		float orthoBottom = -fOrthographicSize * 0.5f;
		float orthoTop = fOrthographicSize * 0.5f;
		projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, fOrthographicNear, fOrthographicFar);

	}
}