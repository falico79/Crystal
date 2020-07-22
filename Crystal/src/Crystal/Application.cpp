#include "crystalpch.h"

#include "Application.h"

#include "Crystal/Events/ApplicationEvent.h"

namespace Crystal {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_running)
		{
			m_Window->OnUpdate();
		}
	}

}
