#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Odysseus/Renderer/Camera.h"
#include "SceneCamera.h"
#include "ScriptableObject.h"
#include <glm/gtx/quaternion.hpp>
#include "Odysseus/Renderer/Texture.h"

#define GLM_ENABLE_EXPERIMENTAL


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
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position) 
				* rotation 
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);
		Ref<Texture2D> Albedo, NormalMap, ORMMap;
		float TilingFactor = 1.0f;
		float Roughness = 0.5f;
		float Metallic = 0.5f;
		float AO = 0.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
		SpriteRendererComponent(const glm::vec4& color, const Ref<Texture2D>& albedo, const Ref<Texture2D>& normalMap = nullptr, const Ref<Texture2D>& ormMap = nullptr, float tilingFactor = 1.0f, float roughness = 0.5f, float metallic = 0.5f, float ao = 0.0f)
			: Color(color), Albedo(albedo), NormalMap(normalMap), ORMMap(ormMap), TilingFactor(tilingFactor), Roughness(roughness), Metallic(metallic), AO(ao) {}
	};

	struct CubeRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		CubeRendererComponent() = default;
		CubeRendererComponent(const CubeRendererComponent&) = default;
		CubeRendererComponent(const glm::vec4& color)
			: Color(color) {}
		CubeRendererComponent(const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor = 1.0f)
			: Color(color), Texture(texture), TilingFactor(tilingFactor) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool bIsMainCamera = true;
		bool bHasFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct PointLightComponent
	{
		float Intensity = 1.0f;
		glm::vec4 Color = glm::vec4(1.0f);

		PointLightComponent() = default;
		PointLightComponent(const float& intensity, const glm::vec4& color = glm::vec4(1.0f))
			: Intensity(intensity), Color(color) {}
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