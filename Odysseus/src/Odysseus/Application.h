#pragma once
#include "Core.h"

namespace Odysseus
{
	class ODYSSEUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
