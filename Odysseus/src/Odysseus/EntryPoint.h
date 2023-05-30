#pragma once
#ifdef ODC_PLATFORM_WINDOWS

extern Odysseus::Application* Odysseus::CreateApplication();

int main(int argc, char** argv)
{
	Odysseus::Log::Init();
	ODC_CORE_WARN("Initialized Log!");
	auto app = Odysseus::CreateApplication();
	app->Run();
	delete app;
}

#endif // ODC_PLATFORM_WINDOWS
