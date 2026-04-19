#include <Odysseus.h>
// -------ENTRY POINT-------------
#include <Odysseus/Core/EntryPoint.h>
// -------------------------------

#include "EditorLayer.h"

namespace Odysseus
{
	class OdysseusEditor : public Application
	{
	public:
		OdysseusEditor(ApplicationCommandLineArgs args)
			: Application("Odysseus Engine 0.0.2", args)
		{
			PushLayer(new EditorLayer());
		}

		~OdysseusEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new OdysseusEditor(args);
	}
}