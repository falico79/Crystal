#include "crystalpch.h"
#include "Crystal/Core/Application.h"

#include "Crystal/Core/Log.h"

#include "Crystal/Renderer/Renderer.h"

#include "Crystal/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Crystal {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
		
	{
		CRYSTAL_PROFILE_FUNCTION();

		CRYSTAL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(CRYSTAL_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		CRYSTAL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		CRYSTAL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		CRYSTAL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CRYSTAL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CRYSTAL_BIND_EVENT_FN(Application::OnWindowResize));

		CRYSTAL_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		CRYSTAL_PROFILE_FUNCTION();

		while (m_Running)
		{
			CRYSTAL_PROFILE_SCOPE("RunLoop");

			float time = glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			if (!m_Minimized)
			{
				{
					CRYSTAL_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					CRYSTAL_PROFILE_SCOPE("LayerStack OnImGuiRenderer");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRenderer();
				}
				m_ImGuiLayer->End();
			}


			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		CRYSTAL_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());


		return false;
	}

}
