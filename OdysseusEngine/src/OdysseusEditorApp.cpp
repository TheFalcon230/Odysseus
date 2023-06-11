#include <Odysseus.h>
// -------ENTRY POINT-------------
#include <Odysseus/Core/EntryPoint.h>
// -------------------------------

#include "EditorLayer.h"

//
//#include <Platform/OpenGL/OpenGLShader.h>
//
//#include "../imgui/imgui.h"
//
//#include <glm/ext/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//


namespace Odysseus
{
	class OdysseusEditor : public Application
	{
	public:
		OdysseusEditor()
			: Application()
		{
			PushLayer(new EditorLayer());
		}

		~OdysseusEditor()
		{
		}

	private:

	};

	Application* CreateApplication()
	{
		return new OdysseusEditor();
	}
}