#pragma once

#ifdef ODC_PLATFORM_WINDOWS

extern Odysseus::Application* Oddyseus::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Odysseus::CreateApplication();
	app->Run();
	delete app;
}

#endif // ODC_PLATFORM_WINDOWS
