#include "EditorLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace Hazel {

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
		m_Texture = Texture2D::Create("assets/textures/icon.png");
		m_SpriteSheet = Texture2D::Create("assets/game/Textures/RPGpack_sheet_2X.png");

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / m_MapWidth;
		m_TextureBarrel = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 9 }, { 128, 128 });
		s_TextureMap['d'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
		s_TextureMap['w'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });


		m_CameraController.SetZoomLevel(4.0f);

		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = Hazel::Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		auto square = m_ActiveScene->CreateEntity("Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		m_SquareEntity = square;
		
		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		m_CameraEntity2 = m_ActiveScene->CreateEntity("Camera Entity2");
		m_CameraEntity2.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
		auto& cc = m_CameraEntity2.GetComponent<CameraComponent>();
		cc.FixedAspectRatio = true;
		cc.Primary = false;
		class CameraController : public ScriptableEntity {
		public:
			void OnCreate() {
				auto& transform = GetComponent<TransformComponent>().Transform;
				std::cout << "Camera Controller::OnCreate!" << std::endl;
				transform[3][0] = rand() % 10 - 5.0f;
			}

			void OnUpdate(TimeStep ts) {
				std::cout << "Camera Controller::OnUpdate: " << ts << std::endl;
				glm::mat4& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;
				if (Input::IsKeyPressed(Key::A)) {
					transform[3][0] -= speed * ts;
				}
				if (Input::IsKeyPressed(Key::D)) {
					transform[3][0] += speed * ts;
				}
				if (Input::IsKeyPressed(Key::W)) {
					transform[3][1] += speed * ts;
				}
				if (Input::IsKeyPressed(Key::S)) {
					transform[3][1] -= speed * ts;
				}
			}

			void OnDestroy() {

			}
		};
		m_CameraEntity.AddNativeScriptComponent<CameraController>();
		m_CameraEntity2.AddNativeScriptComponent<CameraController>();
	}
	void EditorLayer::OnDetach() {

	}

	float position = 0.5f;
	float positionSpeed = 0.1f;
	void EditorLayer::OnUpdate(TimeStep ts) {
		HZ_PROFILE_FUNCTION();
		//HZ_CORE_INFO("ts: {0}", ts.GetMiliSeconds());

		m_FrameBuffer->Bind();

		// On Update 
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);


		Renderer2D::ResetStats();

		// On Render
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);
		
		m_FrameBuffer->Unbind();
	}
	void EditorLayer::OnImGuiRender() {
		HZ_PROFILE_FUNCTION();

		//*
		static bool docSpaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &docSpaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Hazel::Application::Get().Close();
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", NULL, false))
					docSpaceOpen = false;
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		auto stats = Hazel::Renderer2D::GetStats();
		ImGui::Text("Render2D Stats: ");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndicies());

		ImGui::Separator();
		if (m_SquareEntity) {
			ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());

			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;

			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		}
		
		ImGui::Separator();
		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
		ImGui::DragFloat3("Camera2 Transform", glm::value_ptr(m_CameraEntity2.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Camera A", &m_PrimaryCamera)) {
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			HZ_INFO(m_CameraEntity.GetComponent<CameraComponent>().Primary);
			m_CameraEntity2.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
			HZ_INFO(m_CameraEntity2.GetComponent<CameraComponent>().Primary);
		}

		ImGui::End();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewPanelSize.x != viewportPanelSize.x || m_ViewPanelSize.y != viewportPanelSize.y) {

			m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewPanelSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController.Resize(viewportPanelSize.x, viewportPanelSize.y);

			m_ActiveScene->SetViewportSize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		}

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();

		ImGui::Image((void*)textureID, ImVec2{ m_ViewPanelSize.x, m_ViewPanelSize.y }, { 0, 1 }, { 1, 0 });
		
		ImGui::End();
		ImGui::PopStyleVar();
		
		ImGui::End();
		//*/
	}

	void EditorLayer::OnEvent(Event& e) {
		HZ_PROFILE_FUNCTION();
		m_CameraController.OnEvent(e);
	}
}