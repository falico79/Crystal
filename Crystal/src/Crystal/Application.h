#pragma once

#include "Core.h"

#include "Window.h"
#include "Crystal/LayerStack.h"
#include "Events/Event.h"
#include "Crystal/Events/ApplicationEvent.h"

#include "Crystal/ImGui/ImGuiLayer.h"

#include "Crystal/Renderer/Shader.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
