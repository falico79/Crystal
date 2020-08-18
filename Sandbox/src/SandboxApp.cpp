
#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Crystal::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray= Crystal::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Crystal::Ref<Crystal::VertexBuffer> vertexBuffer = Crystal::VertexBuffer::Create(vertices, sizeof(vertices));

		Crystal::BufferLayout layout = {
			{ Crystal::ShaderDataType::Float3, "a_Position" },
			{ Crystal::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		Crystal::Ref<Crystal::IndexBuffer> indexBuffer = Crystal::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Crystal::VertexArray::Create();

		float square[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Crystal::Ref<Crystal::VertexBuffer> squareVB = Crystal::VertexBuffer::Create(square, sizeof(square));

		squareVB->SetLayout({
			{ Crystal::ShaderDataType::Float3, "a_Position" },
			{ Crystal::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Crystal::Ref<Crystal::IndexBuffer> squareIB = Crystal::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main ()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main ()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		m_Shader = Crystal::Shader::Create("VertexColorShader", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main ()
			{
				v_Position = a_Position;

				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main ()
			{
				color = vec4(u_Color, 1.0);
			}

		)";

		m_FlatColorShader = Crystal::Shader::Create("FlatColorShader", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Crystal::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Crystal::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Crystal::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Crystal::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Crystal::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Crystal::RenderCommand::Clear();

		Crystal::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Crystal::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
		
		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Crystal::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Crystal::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// Crystal::Renderer::Submit(m_Shader, m_VertexArray);

		Crystal::Renderer::EndScene();

	}

	virtual void OnImGuiRenderer() override 
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Crystal::Event& e) override
	{
		m_CameraController.OnEvent(e);

	}

private:
	Crystal::ShaderLibrary m_ShaderLibrary;
	Crystal::Ref<Crystal::Shader> m_Shader;
	Crystal::Ref<Crystal::VertexArray> m_VertexArray;

	Crystal::Ref<Crystal::Shader> m_FlatColorShader;
	Crystal::Ref<Crystal::VertexArray> m_SquareVA;

	Crystal::Ref<Crystal::Texture2D> m_Texture;
	Crystal::Ref<Crystal::Texture2D> m_ChernoLogoTexture;

	Crystal::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


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

