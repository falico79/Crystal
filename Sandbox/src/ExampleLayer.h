#pragma once

#include "Crystal.h"

class ExampleLayer : public Crystal::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Crystal::Timestep ts) override;
	virtual void OnImGuiRenderer() override;
	void OnEvent(Crystal::Event& e) override;

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

