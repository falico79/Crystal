#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Crystal::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Crystal::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Crystal::RenderCommand::Clear();

	Crystal::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Crystal::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
	Crystal::Renderer2D::EndScene();

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

}

void Sandbox2D::OnImGuiRenderer()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Crystal::Event& e)
{
	m_CameraController.OnEvent(e);
}
