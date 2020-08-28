#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static const uint32_t s_MapWidth = 24;

static const char* s_MapTiles = 
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwddddddddddddwwwwwww"
"wwwwddwwwwwwwwwwwddwwwww"
"wwwddwwwdddddddddwddwwww"
"wwwdwwwwdddddddddwwwdwww"
"wwdwwwwwdddddddddwwwdwww"
"wdwwwwwwdddddddddwwwdwww"
"wdwwwwwwdddddddddwwwdwww"
"wwddwwwwdddddddddwwwdwww"
"wwwddwwwddddddkddwwwdwww"
"wwwwddwwdddddddddwwdwwww"
"wwwwwwdwwwwwwwwwwwdwwwww"
"wwwwwwwdddddddddddwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwww";

void EditorLayer::OnAttach() {
	HZ_PROFILE_FUNCTION();
	m_Texture = Hazel::Texture2D::Create("assets/textures/icon.png");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/Textures/RPGpack_sheet_2X.png");

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / m_MapWidth;
	m_TextureBarrel = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 9 }, { 128, 128 });
	s_TextureMap['d'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
	s_TextureMap['w'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });


	m_CameraController.SetZoomLevel(4.0f);

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	Hazel::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_FrameBuffer = Hazel::Framebuffer::Create(fbSpec);
}
void EditorLayer::OnDetach() {

}

float position = 0.5f;
float positionSpeed = 0.1f;
void EditorLayer::OnUpdate(Hazel::TimeStep ts) {
	HZ_PROFILE_FUNCTION();
	//HZ_CORE_INFO("ts: {0}", ts.GetMiliSeconds());
	

	// On Update 
	m_CameraController.OnUpdate(ts);

	Hazel::Renderer2D::ResetStats();
	
	// On Render
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.0f });
	Hazel::RenderCommand::Clear();

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}


	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	m_Rotation += m_RotationSpeed * ts;
	if (position > 3.0f || position < -3.0f)
		positionSpeed = -positionSpeed;
	position += positionSpeed;
	//*
	for (uint32_t y = 0; y < m_MapHeight; y++) {
		for (uint32_t x = 0; x < m_MapWidth; x++) {
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Hazel::Ref<Hazel::SubTexture2D> texture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
				texture = s_TextureMap[tileType];
			else
				texture = m_TextureBarrel;
			Hazel::Renderer2D::DrawQuad({ x - m_MapWidth/2.0f, y-m_MapHeight/2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
		}
	}
	//Hazel::Renderer2D::DrawRotatedQuad({ 0.5f, 0.5f, 0.0f }, m_Rotation, { 3.0f, 3.0f }, m_Texture, 2.0f, glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
	//Hazel::Renderer2D::DrawRotatedQuad({ 0.5f, 0.5f, 0.1f }, m_Rotation, { 1.0f, 1.0f }, m_TextureStairs);
	//Hazel::Renderer2D::DrawRotatedQuad({ 0.5f, 0.5f, 0.0f }, m_Rotation, { 3.0f, 3.0f }, m_Texture, 2.0f, glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
	Hazel::Renderer2D::EndScene();
	
}
void EditorLayer::OnImGuiRender() {
	HZ_PROFILE_FUNCTION();
	
	//*

	ImGui::Begin("Settings");
	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Render2D Stats: ");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndicies());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SuareColor));

	

	ImGui::End();
	//*/
}

void EditorLayer::OnEvent(Hazel::Event& e) {
	HZ_PROFILE_FUNCTION();
	m_CameraController.OnEvent(e);
}