#pragma once
#include "entt.hpp"
#include <Odysseus/Core/Timestep.h>



namespace Odysseus
{
	class Object;
	class ODYSSEUS_API Scene
	{
	public:
		Scene();
		~Scene();

		Object CreateObject(std::string name = "");
		void DestroyObject(Object object);

		void Update(Timestep time);
		void OnViewportResize(uint32_t newWidth, uint32_t newHeight);
	private:
		template<typename T>
		void OnComponentAdded(Object object, T& component);
	private:
		uint32_t viewportWidth = 0, viewportHeight = 0;

		entt::registry registry;

		friend class Object;
		friend class HierarchyPanel;
	};
}