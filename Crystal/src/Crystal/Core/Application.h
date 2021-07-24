#pragma once

#include "Core.h"

#include "Window.h"
#include "Crystal/Core/LayerStack.h"
#include "Crystal/Events/Event.h"
#include "Crystal/Events/ApplicationEvent.h"

#include "Crystal/ImGui/ImGuiLayer.h"

#include "Crystal/Core/Timestep.h"

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

		void Close();

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
