#pragma once

#include "Core.h"

#include "Window.h"
#include "Crystal/LayerStack.h"
#include "Events/Event.h"
#include "Crystal/Events/ApplicationEvent.h"


namespace Crystal {

	class CRYSTAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}
