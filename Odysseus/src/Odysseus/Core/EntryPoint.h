#pragma once
#ifdef ODC_PLATFORM_WINDOWS

extern Odysseus::Application* Odysseus::CreateApplication();

int main(int argc, char** argv)
{
	Odysseus::Log::Init();
	ODC_PROFILE_BEGIN_SESSION("Startup", "OdysseusProfile-Startup.json");
	auto app = Odysseus::CreateApplication();
	ODC_PROFILE_END_SESSION();
	ODC_PROFILE_BEGIN_SESSION("Runtime", "OdysseusProfile-Runtime.json");
	app->Run();
	ODC_PROFILE_END_SESSION();
	ODC_PROFILE_BEGIN_SESSION("Shutdown", "OdysseusProfile-Shutdown.json");
	delete app;
	ODC_PROFILE_END_SESSION();
}

#endif // ODC_PLATFORM_WINDOWS
