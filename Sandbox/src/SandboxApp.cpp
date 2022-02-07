#include <Odysseus.h>
class ExampleLayer : public Odysseus::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		ODC_INFO("ExampleLayer::Update");
	}

	void OnEvent(Odysseus::Event& event) override
	{
		ODC_TRACE("{0}", event);
	}

};

class Sandbox : public Odysseus::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
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