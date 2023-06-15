#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Odysseus/Renderer/Camera.h"
#include "SceneCamera.h"
#include "ScriptableObject.h"

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
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position) : Position(position) {}
		/*TransformComponent(const glm::vec3& pPosition, float pRotation, glm::vec2 pScale)
			:Position(pPosition), Rotation(pRotation), Scale(pScale)
		{
			glm::mat4 location = glm::translate(glm::mat4(1.0f), pPosition);
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(pRotation), { 0.f, 0.f, 1.0f });
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), { pScale.x, pScale.y, 1.0f });
			Transform = pRotation == 0.0f ? location * scale : location * rotation * scale;
		}*/

		glm::mat4 Transform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0,0,1 });

			return glm::translate(glm::mat4(1.0f), Position) 
				* rotation 
				* glm::scale(glm::mat4(1.0f), Scale);
		}
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

	struct NativeScriptComponent
	{
		ScriptableObject* Instance = nullptr;

		ScriptableObject* (*InstanciateScript)();
		void (*DestroyScript)(NativeScriptComponent*);


		template<typename T>
		void Bind()
		{
			InstanciateScript = []() { return static_cast<ScriptableObject*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}