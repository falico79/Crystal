#pragma once

#include "Crystal.h"

class Sandbox2D : public Crystal::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Crystal::Timestep ts) override;
	virtual void OnImGuiRenderer() override;
	void OnEvent(Crystal::Event& e) override;
private:
	Crystal::OrthographicCameraController m_CameraController;

	// Temp
	Crystal::Ref<Crystal::VertexArray> m_SquareVA;
	Crystal::Ref<Crystal::Shader> m_FlatColorShader;

	Crystal::Ref<Crystal::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor;
};