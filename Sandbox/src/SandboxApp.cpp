
#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>


#include "Sandbox2D.h"
#include "ExampleLayer.h"


class Sandbox : public Crystal::Application {
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() 
	{

	}
	
};

Crystal::Application* Crystal::CreateApplication()
{
	return new Sandbox();
}

