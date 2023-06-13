#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Odysseus/Renderer/Camera.h"
#include "SceneCamera.h"

namespace Odysseus
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) 
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);
		glm::vec3 Position = glm::vec3(1.0f);
		float Rotation = 0.0f;
		glm::vec2 Scale = glm::vec2(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& pPosition, float pRotation, glm::vec2 pScale)
			:Position(pPosition), Rotation(pRotation), Scale(pScale)
		{
			glm::mat4 location = glm::translate(glm::mat4(1.0f), pPosition);
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(pRotation), { 0.f, 0.f, 1.0f });
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), { pScale.x, pScale.y, 1.0f });
			Transform = pRotation == 0.0f ? location * scale : location * rotation * scale;
		}

		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool bIsMainCamera = true;
		bool bHasFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}