#include "odcpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Odysseus
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding, BufferUsageType usage)
		: m_RendererID(0), m_Size(size), m_BindingPoint(binding), m_UsageType(usage)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, (m_UsageType == BufferUsageType::DYNAMIC_DRAW ) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	}

	void OpenGLUniformBuffer::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
	}

	void OpenGLUniformBuffer::SetBindingPoint(uint32_t binding)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
		m_BindingPoint = binding;
	}

	void OpenGLUniformBuffer::AddData(uint32_t offset, uint32_t size, const void* data)
	{
		//glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

}