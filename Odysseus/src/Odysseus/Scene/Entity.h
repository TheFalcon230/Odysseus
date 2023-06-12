#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace Odysseus
{
	class ODYSSEUS_API Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* pScene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ODC_CORE_ASSERT(!HasComponent<T>(), "Entity already has this type of component.");
			return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			ODC_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this type of component.");
			return scene->registry.get<T>(entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return scene->registry.all_of<T>(entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			ODC_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this type of component.");
			scene->registry.remove<T>(entityHandle);
		}

		operator bool() const { return entityHandle != entt::null; }
	private:
		entt::entity entityHandle = entt::null;
		Scene* scene = nullptr;
	};
}