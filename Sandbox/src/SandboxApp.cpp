#include <Odysseus.h>

class Sandbox : public Odysseus::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}

private:

};

Odysseus::Application* Odysseus::CreateApplication()
{
	return new Sandbox();
}