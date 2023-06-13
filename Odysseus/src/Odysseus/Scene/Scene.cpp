#include "odcpch.h"
#include "Scene.h"

#include "Odysseus/Renderer/Renderer2D.h"
#include "Components.h"
#include "Entity.h"

namespace Odysseus
{

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	Object Scene::CreateEntity(std::string name)
	{
		Object entity = { registry.create() , this };
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::Update(Timestep time)
	{
		// Update scripts
		{
			registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
			{
					//TODO: Move to Scene::OnPlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstanciateScript();
					nsc.Instance->object = Object{ entity, this };
					nsc.Instance->Start();
				}
			nsc.Instance->Update(time);
			});
		}


		//Get all the cameras in the scene
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = registry.group<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.bIsMainCamera)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}


		//Render sprites
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto view = registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
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

	void Scene::OnViewportResize(uint32_t newWidth, uint32_t newHeight)
	{
		viewportWidth = newWidth;
		viewportHeight = newHeight;
		auto view = registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.bHasFixedAspectRatio)
			{
				camera.Camera.SetViewportSize(newWidth, newHeight);
			}
		}
	}
}
