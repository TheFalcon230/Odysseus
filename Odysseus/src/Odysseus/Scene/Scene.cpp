#include "odcpch.h"
#include "Scene.h"

#include "Odysseus/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Odysseus
{

	Scene::Scene()
	{
		entt::entity entity = registry.create();
		registry.emplace<TransformComponent>(entity);
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(std::string name)
	{
		Entity entity = { registry.create() , this };
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::Update(Timestep time)
	{
		//Render Sprites
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = registry.group<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.bIsMainCamera)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto view = registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto& [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				QuadProperties quad;
				quad.baseColor = sprite.Color;
				quad.position = transform.Position;
				quad.rotation = transform.Rotation;
				quad.scale = transform.Scale;
				Renderer2D::DrawQuad(quad);
			}
			Renderer2D::EndScene();
		}
	}
}
