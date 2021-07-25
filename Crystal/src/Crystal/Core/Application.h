#pragma once

#include "Crystal/Core/Core.h"

#include "Crystal/Core/Window.h"
#include "Crystal/Core/LayerStack.h"
#include "Crystal/Events/Event.h"
#include "Crystal/Events/ApplicationEvent.h"

#include "Crystal/Core/Timestep.h"

#include "Crystal/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Crystal {

	class CRYSTAL_API Application
	{
	public:
		Application(const std::string& name = "Crystal App");
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }
	private:
		void Run();
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
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}
