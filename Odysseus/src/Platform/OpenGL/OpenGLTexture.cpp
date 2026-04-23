#include "odcpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

//#define ODC_PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name, [&](ProfileResult profileResult) {})
//#define ODC_PROFILE_FUNCTION() ODC_PROFILE_SCOPE(__FUNCSIG__);

namespace Odysseus
{
	namespace Utils {

		static GLenum OdysseusImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			}

			ODC_CORE_ASSERT(false);
			return 0;
		}

		static GLenum OdysseusImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}

			ODC_CORE_ASSERT(false);
			return 0;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		ODC_CORE_INFO("Start loading texture at path: '{0}'", m_Path);

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;	
		{
			//ODC_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}		

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

				m_InternalFormat = internalFormat;
				m_DataFormat = dataFormat;

			ODC_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported.");
			ODC_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported.");

				glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
				glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
				ODC_CORE_INFO("Texture loaded successfully!");

				stbi_image_free(data);
		}
		else
		{
			ODC_CORE_ASSERT(data, "Failed to load image!");
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
	{
		m_InternalFormat = Odysseus::Utils::OdysseusImageFormatToGLInternalFormat(specification.Format);
		m_DataFormat = Odysseus::Utils::OdysseusImageFormatToGLDataFormat(specification.Format);
		m_InternalFormat = Odysseus::Utils::OdysseusImageFormatToGLInternalFormat(specification.Format);
		m_DataFormat = Odysseus::Utils::OdysseusImageFormatToGLDataFormat(specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bytePerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		ODC_CORE_ASSERT(size == m_Width * m_Height * bytePerPixel, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		//ODC_PROFILE_SCOPE(__FUN);
		glBindTextureUnit(slot, m_RendererID);
	}

	bool OpenGLTexture2D::IsLoaded() const
	{
		return m_IsLoaded;
	}
}