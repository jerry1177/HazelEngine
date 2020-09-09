#pragma once

#include "Hazel.h"
#include "ParticleSystem.h"
namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer() : Layer("Sandbox"), m_CameraController(1280.0f / 720.0f) {}
		virtual ~EditorLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(TimeStep ts) override;
		void OnImGuiRender() override;

		void OnEvent(Event& e) override;

	private:

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity, m_CameraEntity, m_CameraEntity2;
		bool m_PrimaryCamera = true;

		OrthographicCameraController m_CameraController;
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		glm::vec4 m_SuareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_TextureBarrel;
		Ref<Framebuffer> m_FrameBuffer;
		float m_Rotation = 0.0f;
		float m_RotationSpeed = 10.0f;
		ParticleSystem m_ParticleSystem;

		ParticleProps m_Particle;


		std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> s_TextureMap;
		uint32_t m_MapWidth, m_MapHeight;

		glm::vec2 m_ViewPanelSize;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};
}

