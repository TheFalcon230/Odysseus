#pragma once
#include <string>
#include <glm/glm.hpp>


namespace Odysseus
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void Unbind();

		void UploadUniforMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_rendererID = 0;

	};
}