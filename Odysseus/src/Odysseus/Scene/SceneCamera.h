#pragma once

#include "Odysseus/Renderer/Camera.h"

namespace Odysseus
{
	class ODYSSEUS_API SceneCamera: public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t newWidth, uint32_t newHeight);

		float GetOrthographicSize() const { return fOrthographicSize; }
		void SetOrthographicSize(float newSize) { fOrthographicSize = newSize; RecalculateProjection(); }
	private: //For private functions
		void RecalculateProjection();
	private: // For private variables
		float fOrthographicSize = 10.0f;
		float fOrthographicNear = -1.0f;
		float fOrthographicFar = 1.0f;

		float fAspectRatio = 1.0f;
	};

}
