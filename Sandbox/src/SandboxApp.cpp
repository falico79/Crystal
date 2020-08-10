
#include <Crystal.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <imgui/imgui.h>
#include <glm\glm\gtc\type_ptr.hpp>

class ExampleLayer : public Crystal::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Crystal::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Crystal::Ref<Crystal::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Crystal::VertexBuffer::Create(vertices, sizeof(vertices)));

		Crystal::BufferLayout layout = {
			{ Crystal::ShaderDataType::Float3, "a_Position" },
			{ Crystal::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		Crystal::Ref<Crystal::IndexBuffer> indexBuffer;
		indexBuffer.reset(Crystal::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Crystal::VertexArray::Create());

		float square[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Crystal::Ref<Crystal::VertexBuffer> squareVB;
		squareVB.reset(Crystal::VertexBuffer::Create(square, sizeof(square)));

		squareVB->SetLayout({
			{ Crystal::ShaderDataType::Float3, "a_Position" },
			{ Crystal::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Crystal::Ref<Crystal::IndexBuffer> squareIB;
		squareIB.reset(Crystal::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(Crystal::Shader::Create(vertexSrc, fragmentSrc));

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

		m_FlatColorShader.reset(Crystal::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		m_TextureShader.reset(Crystal::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Crystal::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Crystal::Timestep ts) override
	{
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_A))
		{
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_D))
		{
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		Crystal::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Crystal::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Crystal::Renderer::BeginScene(m_Camera);

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
		
		m_Texture->Bind();
		Crystal::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Crystal::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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

	void OnEvent(Crystal::Event& event) override
	{

	}

private:
	Crystal::Ref<Crystal::Shader> m_Shader;
	Crystal::Ref<Crystal::VertexArray> m_VertexArray;

	Crystal::Ref<Crystal::Shader> m_FlatColorShader, m_TextureShader;
	Crystal::Ref<Crystal::VertexArray> m_SquareVA;

	Crystal::Ref<Crystal::Texture2D> m_Texture;
	Crystal::Ref<Crystal::Texture2D> m_ChernoLogoTexture;

	Crystal::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


class Sandbox : public Crystal::Application {
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() 
	{

	}
	
};

Crystal::Application* Crystal::CreateApplication()
{
	return new Sandbox();
}

