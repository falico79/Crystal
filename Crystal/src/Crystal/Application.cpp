#include "crystalpch.h"

#include "Application.h"



namespace Crystal {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		CRYSTAL_CORE_TRACE("{0}", e);
	}

	void Application::Run()
	{
		while (m_running)
		{
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

}
