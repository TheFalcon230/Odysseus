#pragma once

#include "RendererAPI.h"

namespace Odysseus
{
	class  RenderCommand
	{
	public:
		ODYSSEUS_API inline static void Init()
		{
			s_RendererAPI->Init();
		}

		ODYSSEUS_API inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		ODYSSEUS_API inline static void Clear()
		{
			s_RendererAPI->Clear();
		}


		ODYSSEUS_API inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		ODYSSEUS_API inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}
	private:
		static RendererAPI* s_RendererAPI;

	};
}