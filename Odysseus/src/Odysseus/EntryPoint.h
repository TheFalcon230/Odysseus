#pragma once
#include "Log.h"
#ifdef ODC_PLATFORM_WINDOWS

extern Odysseus::Application* Odysseus::CreateApplication();

int main(int argc, char** argv)
{
	Odysseus::Log::Init();
	auto app = Odysseus::CreateApplication();
	app->Run();
	delete app;
}

#endif // ODC_PLATFORM_WINDOWS
