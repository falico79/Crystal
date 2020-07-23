
#include <Crystal.h>

class ExampleLayer : public Crystal::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate() override
	{
		CRYSTAL_CLIENT_INFO("ExampleLayer::Update");
	}

	void OnEvent(Crystal::Event& event) override
	{
		CRYSTAL_CLIENT_TRACE("{0}", event);
	}
};


class Sandbox : public Crystal::Application {
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() 
	{

	}
	
};

Crystal::Application* Crystal::CreateApplication()
{
	return new Sandbox();
}

