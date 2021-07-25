#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Crystal/Debug/Instrumentor.h"

namespace Crystal {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		CRYSTAL_PROFILE_FUNCTION();

		m_CheckerboardTexture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");

		Crystal::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Crystal::Framebuffer::Create(fbSpec);


	}

	void EditorLayer::OnDetach()
	{
		CRYSTAL_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Crystal::Timestep ts)
	{
		CRYSTAL_PROFILE_FUNCTION();

		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Crystal::Renderer2D::ResetStats();
		{
			CRYSTAL_PROFILE_SCOPE("Render Prep");
			m_Framebuffer->Bind();
			Crystal::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Crystal::RenderCommand::Clear();
		}

		{
			static float rotation = 0.0f;
			rotation += ts * 50.0f;

			CRYSTAL_PROFILE_SCOPE("Render Draw");
			Crystal::Renderer2D::BeginScene(m_CameraController.GetCamera());
			Crystal::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), m_SquareColor);
			Crystal::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
			Crystal::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { m_SquareColor.b, m_SquareColor.g, m_SquareColor.r, 1.0f });
			Crystal::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
			Crystal::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);

			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
					Crystal::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}
			Crystal::Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRenderer()
	{
		CRYSTAL_PROFILE_FUNCTION();

		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit")) Crystal::Application::Get().Close();
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Begin("Settings");

			auto stats = Crystal::Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());


			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("Viewport");
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
			{
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::End();

			ImGui::PopStyleVar();

			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Crystal::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}