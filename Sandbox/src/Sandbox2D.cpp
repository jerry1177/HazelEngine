#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void Sandbox2D::OnAttach() {
	
}
void Sandbox2D::OnDetach() {

}
void Sandbox2D::OnUpdate(Hazel::TimeStep ts) {
	// On Update 
	m_CameraController.OnUpdate(ts);

	// On Render
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, {1.0f, 1.0f}, m_SuareColor);

	Hazel::Renderer2D::EndScene();
	
}
void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SuareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e) {
	m_CameraController.OnEvent(e);
}