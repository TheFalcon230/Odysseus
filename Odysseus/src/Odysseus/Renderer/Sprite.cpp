#include "odcpch.h"
#include "Sprite.h"

namespace Odysseus
{
	Sprite::Sprite(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		:m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<Sprite> Sprite::CreateFromCoords(const Ref<Texture2D>& texture, float x, float y, float spriteWidth, float spriteHeight,glm::vec2 spriteSize)
	{
		return CreateFromCoords(texture, glm::vec2{ x, y }, glm::vec2{ spriteWidth, spriteHeight }, spriteSize);
	}

	Ref<Sprite> Sprite::CreateFromCoords(const Ref<Texture2D>& texture, glm::vec2& coords, glm::vec2 cellSize, glm::vec2 spriteSize)
	{
		
		glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.y) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
		return CreateRef<Sprite>(texture, min, max);
	}

}