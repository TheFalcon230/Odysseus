#pragma once

#include <glm/glm.hpp>
#include "Odysseus/Renderer/Shader.h"

//TODO: REMOVE
typedef unsigned int GLenum;

namespace Odysseus
{
	class ODYSSEUS_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& path);
		virtual ~OpenGLShader();

		virtual void Bind() const  override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& name, const glm::mat4& value)  override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value)  override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value)  override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat(const std::string& name,  float value) override;
		virtual void SetInt(const std::string& name,  int value)  override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count)  override;

		virtual const std::string& GetName() const override { return m_Name; }

		/// <summary>
		/// Upload a 4x4 matrix uniform to the shader.
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		/// <summary>
		/// Upload a 3x3 matrix uniform to the shader.
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);

		/// <summary>
		/// Upload an integer to a given uniform to the shader
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformInt(const std::string& name, const int value);

		/// <summary>
		/// Upload an integer to a given uniform to the shader
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		/// <summary>
		/// Upload a float to a given uniform to the shader
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformFloat(const std::string& name, const float value);

		/// <summary>
		/// Upload a 2D Vector (or float2) to a given uniform to the shader
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);

		/// <summary>
		/// Upload a 3D Vector (or float3) to a given uniform to the shader
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);

		/// <summary>
		/// Upload a 4D Vector (or float4) to a given uniform to the shader
		/// </summary>
		/// <param name="name">The uniform to upload.</param>
		/// <param name="value">The value to assign to the uniform.</param>
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_rendererID = 0;
		std::string m_Name;
	};
}
