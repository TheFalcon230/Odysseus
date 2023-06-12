#pragma once
#include "entt.hpp"
#include <Odysseus/Core/Timestep.h>

#include "Components.h"

namespace Odysseus
{
	class Entity;
	class ODYSSEUS_API Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(std::string name = "");
		//Temp function
		entt::registry& Reg() { return registry; }

		void Update(Timestep time);
	private:
		entt::registry registry;

		friend class Entity;
	};
}