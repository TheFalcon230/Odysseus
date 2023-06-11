#pragma once
#include "Texture.h"
#include <glm/glm.hpp>

namespace Odysseus
{
	class ODYSSEUS_API Sprite
	{
	public:
		Sprite(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		
		const Ref<Texture2D> GetTexture() { return m_Texture; }
		const glm::vec2* GetTexCoord()const { return m_TexCoords; }

		static Ref<Sprite> CreateFromCoords(const Ref<Texture2D>& texture,float x, float y, float spriteWidth, float spriteHeight, glm::vec2 spriteSize = { 1,1 });
		static Ref<Sprite> CreateFromCoords(const Ref<Texture2D>& texture, glm::vec2& coords, glm::vec2 cellSize , glm::vec2 spriteSize = {1,1});

	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_TexCoords[4];
	};
}