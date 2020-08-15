#pragma once

#include "Hazel.h"
class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D() : Layer("Sandbox"), m_CameraController(1280.0f / 720.0f) {}
	virtual ~Sandbox2D() = default;
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
};

