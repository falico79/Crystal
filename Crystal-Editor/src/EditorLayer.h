#pragma once

#include "Crystal.h"

namespace Crystal {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Crystal::Timestep ts) override;
		virtual void OnImGuiRenderer() override;
		void OnEvent(Crystal::Event& e) override;
	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<Crystal::VertexArray> m_SquareVA;
		Ref<Crystal::Shader> m_FlatColorShader;
		Ref<Crystal::Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		entt::entity m_SquareEntity;

		Ref<Crystal::Texture2D> m_CheckerboardTexture;
		Ref<Crystal::Texture2D> m_SpriteSheet;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize;
		glm::vec4 m_SquareColor;

	};
}