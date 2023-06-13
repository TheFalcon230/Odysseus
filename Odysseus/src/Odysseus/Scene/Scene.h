#pragma once
#include "entt.hpp"
#include <Odysseus/Core/Timestep.h>

#include "Components.h"

namespace Odysseus
{
	class Object;
	class ODYSSEUS_API Scene
	{
	public:
		Scene();
		~Scene();

		Object CreateEntity(std::string name = "");

		void Update(Timestep time);
		void OnViewportResize(uint32_t newWidth, uint32_t newHeight);
	private:
		uint32_t viewportWidth = 0, viewportHeight = 0;

		entt::registry registry;

		friend class Object;
	};
}