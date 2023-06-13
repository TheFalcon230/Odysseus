#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace Odysseus
{
	class ODYSSEUS_API Object
	{
	public:
		Object() = default;
		Object(entt::entity handle, Scene* pScene);
		Object(const Object& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ODC_CORE_ASSERT(!HasComponent<T>(), "Entity already has this type of component.");
			return scene->registry.emplace<T>(objectID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			ODC_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this type of component.");
			return scene->registry.get<T>(objectID);
		}

		template<typename T>
		bool HasComponent()
		{
			return scene->registry.all_of<T>(objectID);
		}

		template<typename T>
		void RemoveComponent()
		{
			ODC_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this type of component.");
			scene->registry.remove<T>(objectID);
		}

		operator bool() const { return objectID != entt::null; }
		operator uint32_t() const { return (uint32_t)objectID; }

		bool operator==(const Object& other) const { return objectID == other.objectID && scene == other.scene; }
		bool operator!=(const Object& other) const { return !(*this == other); }
	private:
		entt::entity objectID = entt::null;
		Scene* scene = nullptr;
	};
}