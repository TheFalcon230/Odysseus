#include "odcpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Odysseus
{
	static const uint32_t MaxFrameBufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool pIsMultisampled)
		{
			return pIsMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool isMultisampled = samples > 1;
			if (isMultisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				// TODO: Put wrapping and filtering in framebuffer specs 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(isMultisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool isMultisampled = samples > 1;
			if (isMultisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				// TODO: Put wrapping and filtering in framebuffer specs 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, Utils::TextureTarget(isMultisampled), id, 0);
		}



		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case Odysseus::FramebufferTextureFormat::None:
				return false;
			case Odysseus::FramebufferTextureFormat::RBGA8:
				return false;
			case Odysseus::FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
			default:
				return false;
			}
		}

		static void BindTexture(bool pIsMultisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(pIsMultisampled), id);
		}

		static void CreateTextures(bool pIsMultisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(pIsMultisampled), count, outID);
		}

		static GLenum OdysseusTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case Odysseus::FramebufferTextureFormat::None:
				return 0;
			case Odysseus::FramebufferTextureFormat::RBGA8:
				return GL_RGBA8;
			case Odysseus::FramebufferTextureFormat::RED_INTEGER:
				return GL_RED_INTEGER;
			case Odysseus::FramebufferTextureFormat::DEPTH24STENCIL8:
				return GL_DEPTH24_STENCIL8;
			default:
				ODC_CORE_ASSERT(false,"No format found")
				return 0;
			}
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				colorAttachmentSpecs.emplace_back(spec);
			else
				depthAttachmentSpecs = spec;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_DepthAttachment);
		glDeleteTextures(ColorAttachments.size(), ColorAttachments.data());

		ColorAttachments.clear();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_DepthAttachment);
			glDeleteTextures(ColorAttachments.size(), ColorAttachments.data());

			ColorAttachments.clear();
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool isMultisampled = m_Specification.Samples > 1;

		// Attachments
		if (colorAttachmentSpecs.size())
		{
			ColorAttachments.resize(colorAttachmentSpecs.size());
			Utils::CreateTextures(isMultisampled, ColorAttachments.data(), ColorAttachments.size());

			for (size_t i = 0; i < colorAttachmentSpecs.size(); i++)
			{
				Utils::BindTexture(isMultisampled, ColorAttachments[i]);
				switch (colorAttachmentSpecs[i].TextureFormat)
				{
				case Odysseus::FramebufferTextureFormat::None:
					break;
				case Odysseus::FramebufferTextureFormat::RBGA8:
					Utils::AttachColorTexture(ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case Odysseus::FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}

		}

		if (depthAttachmentSpecs.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(isMultisampled, &m_DepthAttachment, 1);
			Utils::BindTexture(isMultisampled, m_DepthAttachment);
			switch (depthAttachmentSpecs.TextureFormat)
			{
			case Odysseus::FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}

		}

		if (ColorAttachments.size() > 1)
		{
			ODC_CORE_ASSERT(ColorAttachments.size() <= 4, "4 attachment is currently supported");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(ColorAttachments.size(), buffers);
		}
		else if (ColorAttachments.empty())
		{
			// Depth pass only
			glDrawBuffer(GL_NONE);
		}

		bool framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		ODC_CORE_ASSERT(framebufferStatus, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > MaxFrameBufferSize || height > MaxFrameBufferSize)
		{
			ODC_CORE_ERROR("Invalid framebuffer size: Size should be greater than 0 and less than {0} (Size is {1},{2})", MaxFrameBufferSize, width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		ODC_CORE_ASSERT(attachmentIndex < ColorAttachments.size(), "");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		ODC_CORE_ASSERT(attachmentIndex < ColorAttachments.size(), "");

		auto& spec = colorAttachmentSpecs[attachmentIndex];
		glClearTexImage(ColorAttachments[attachmentIndex], 0, Utils::OdysseusTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

}