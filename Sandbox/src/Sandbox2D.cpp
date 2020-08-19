#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
/*
#include <chrono>
template<typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& func) : m_Name(name), m_Stopped(false), m_Func(func) {
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	void Stop() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		m_Stopped = true;
		float duration = (end - start) * 0.001f;

		m_Func({m_Name, duration});

	}
	~Timer() {
		if (!m_Stopped) Stop();
	}
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
*/
void Sandbox2D::OnAttach() {
	HZ_PROFILE_FUNCTION();
	m_Texture = Hazel::Texture2D::Create("assets/textures/icon.png");
}
void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts) {
	HZ_PROFILE_FUNCTION();
	
	// On Update 
	m_CameraController.OnUpdate(ts);
	
	// On Render
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	m_Rotation += m_RotationSpeed * ts;
	Hazel::Renderer2D::DrawQuad({ 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f }, m_SuareColor);
	Hazel::Renderer2D::DrawRotatedQuad({ 0.5f, 0.5f, 1.0 }, m_Rotation, { 0.5f, 1.0f }, {1.0f, 0.0f, 0.0f, 1.0f});
	
	Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, m_Rotation, { 3.0f, 3.0f }, m_Texture, 2.0f, glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));
	Hazel::Renderer2D::EndScene();
	
}
void Sandbox2D::OnImGuiRender() {
	HZ_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SuareColor));

	
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e) {
	HZ_PROFILE_FUNCTION();
	m_CameraController.OnEvent(e);
}