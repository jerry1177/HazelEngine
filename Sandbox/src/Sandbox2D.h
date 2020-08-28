#pragma once

#include "Hazel.h"
#include "ParticleSystem.h"
class EditorLayer : public Hazel::Layer
{
public:
	EditorLayer() : Layer("Sandbox"), m_CameraController(1280.0f / 720.0f) {}
	virtual ~EditorLayer() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Hazel::TimeStep ts) override;
	void OnImGuiRender() override;

	void OnEvent(Hazel::Event& e) override;

private:

	Hazel::OrthographicCameraController m_CameraController;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::Shader> m_Shader;
	glm::vec4 m_SuareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
	Hazel::Ref<Hazel::SubTexture2D> m_TextureBarrel;
	Hazel::Ref<Hazel::Framebuffer> m_FrameBuffer;
	float m_Rotation = 0.0f;
	float m_RotationSpeed = 10.0f;
	ParticleSystem m_ParticleSystem;

	ParticleProps m_Particle;
	

	std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> s_TextureMap;
	uint32_t m_MapWidth, m_MapHeight;
};

