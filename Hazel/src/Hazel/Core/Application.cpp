#include "hzpch.h"
#include "Application.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "GLFW/glfw3.h"

namespace Hazel {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		HZ_PROFILE_FUNCTION();
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
		
		
		m_Running = true;
	}

	Application::~Application() {
		HZ_PROFILE_FUNCTION();
	}

	void Application::PushLayer(Layer* layer) {
		HZ_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLay(Layer* layer) {
		HZ_PROFILE_FUNCTION();
		m_LayerStack.PushOverLay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		HZ_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWIndowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	

	void Application::Run() {
		HZ_PROFILE_FUNCTION();
		while (m_Running) { 
			HZ_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime();
			TimeStep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimize) {
				{
					HZ_PROFILE_SCOPE("Layers: OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
			}
			
			m_ImGuiLayer->Begin();
			{
				HZ_PROFILE_SCOPE("Layers: For ImGui Rendering");
				// ImGui render sits between begin and end 
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
	bool Application::OnWIndowResize(WindowResizeEvent& e) {
		HZ_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimize = true;
			return false;
		}
		m_Minimize = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

}