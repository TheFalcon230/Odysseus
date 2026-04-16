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
		OdysseusEditor()
			: Application("Odysseus Engine 0.0.1")
		{
			PushLayer(new EditorLayer());
		}

		~OdysseusEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new OdysseusEditor();
	}
}