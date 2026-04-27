#pragma once

#include "odcpch.h"
#include "Odysseus/Renderer/UniformBuffer.h"


namespace Odysseus
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
		public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding, BufferUsageType usage);
		virtual ~OpenGLUniformBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetBindingPoint(uint32_t binding) override;
		virtual void SetData(uint32_t offset, uint32_t size, const void* data) override;
	private:
		uint32_t m_RendererID, m_Size, m_BindingPoint;
		BufferUsageType m_UsageType;
	};
}