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

	Object Scene::CreateObject(std::string name)
	{
		Object entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>(glm::vec3(0.0f));
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	Object Scene::CreateSquare(std::string name)
	{
		Object entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>(glm::vec3(0.0f));
		auto& tag = entity.AddComponent<TagComponent>(name);
		entity.AddComponent<SpriteRendererComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyObject(Object object)
	{
		registry.destroy(object);
	}

	Object Scene::GetMainCamera()
	{
		auto view = registry.view<CameraComponent>();
		for (auto object : view)
		{
			const 	auto& cameraComponent = view.get<CameraComponent>(object);
			if (cameraComponent.bIsMainCamera)
			{
				return Object{ object, this };
			}
		}
		ODC_CORE_ERROR("Error: no main camera found!");
		return {};
	}

	void Scene::UpdateRuntime(Timestep time)
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
		glm::mat4 cameraTransform;
		{
			auto group = registry.group<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.bIsMainCamera)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.Transform();
					break;
				}
			}
		}


		//Render sprites
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			auto view = registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				QuadProperties quad;
				quad.baseColor = sprite.Color;
				quad.position = transform.Position;
				quad.rotation = transform.Rotation;
				quad.scale = glm::vec2(transform.Scale.x, transform.Scale.y);
				Renderer2D::DrawQuad(quad, (int)entity);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::UpdateEditor(Timestep time, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto view = registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
			QuadProperties quad;
			quad.baseColor = sprite.Color;
			quad.position = transform.Position;
			quad.rotation = transform.Rotation;
			quad.scale = glm::vec2(transform.Scale.x, transform.Scale.y);
			Renderer2D::DrawQuad(quad, (int)entity);
		}
		Renderer2D::EndScene();
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

	template<typename T>
	void Scene::OnComponentAdded(Object object, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Object object, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Object object, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Object object, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Object object, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Object object, CameraComponent& component)
	{
		component.Camera.SetViewportSize(viewportWidth, viewportHeight);
	}
}
