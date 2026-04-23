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
		entity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	Object Scene::CreateCube(std::string name)
	{
		Object entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>(glm::vec3(0.0f));
		auto& tag = entity.AddComponent<TagComponent>(name);
		entity.AddComponent<CubeRendererComponent>(glm::vec4(1.0f));
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	Object Scene::CreatePointLight(std::string name)
	{
		Object entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>(glm::vec3(0.0f));
		auto& tag = entity.AddComponent<TagComponent>(name);
		entity.AddComponent<PointLightComponent>(1.0f, glm::vec4(1.0f));
		tag.Tag = name.empty() ? "Point Light" : name;
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
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto SpriteView = registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : SpriteView)
			{
				auto [transform, sprite] = SpriteView.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.Transform(), sprite, (int)entity);
			}

			auto CubeView = registry.view<TransformComponent, CubeRendererComponent>();
			for (auto entity : CubeView)
			{
				auto [transform, cube] = CubeView.get<TransformComponent, CubeRendererComponent>(entity);
				Renderer2D::DrawCube(transform.Transform(), cube.Color, (int)entity);
			}

			auto LightView = registry.view<TransformComponent, PointLightComponent>();
			for (auto entity : LightView)
			{
				auto [transform, light] = LightView.get<TransformComponent, PointLightComponent>(entity);
				Renderer2D::DrawPointLight(transform.Position, light.Color, light.Intensity);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::UpdateEditor(Timestep time, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto SpriteView = registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : SpriteView)
		{
			auto [transform, sprite] = SpriteView.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawSprite(transform.Transform(), sprite, (int)entity);
		}

		auto CubeView = registry.view<TransformComponent, CubeRendererComponent>();
		for (auto entity : CubeView)
		{
			auto [transform, cube] = CubeView.get<TransformComponent, CubeRendererComponent>(entity);
			Renderer2D::DrawCube(transform.Transform(), cube.Color, (int)entity);
		}

		auto LightView = registry.view<TransformComponent, PointLightComponent>();
		for (auto entity : LightView)
		{
			auto [transform, light] = LightView.get<TransformComponent, PointLightComponent>(entity);
			Renderer2D::DrawPointLight(transform.Position, light.Color, light.Intensity);
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
		if (viewportWidth > 0 && viewportHeight > 0)
			component.Camera.SetViewportSize(viewportWidth, viewportHeight);
	}
}
