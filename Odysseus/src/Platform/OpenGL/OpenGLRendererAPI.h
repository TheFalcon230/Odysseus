#pragma once
#include "Odysseus/Renderer/RendererAPI.h"

namespace Odysseus
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		// Inherited via RendererAPI
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};
}