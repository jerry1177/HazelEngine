#include "hzpch.h"
#include "Application.h"
#include "Hazel/Input.h"
#include <glad/glad.h>
namespace Hazel {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		m_Running = true;
	}

	Application::~Application() {
	
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLay(Layer* layer) {
		m_LayerStack.PushOverLay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));


		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	

	void Application::Run() {
		WindowResizeEvent e(1200, 720);
		if (e.IsInCategory(EventCategoryApplication)) HZ_INFO(e);
		if (!e.IsInCategory(EventCategoryKeyboard)) HZ_ERROR(e);
		while (m_Running) { 
			glClearColor(0, .5, 1, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//bool x = Input::IsKeyPressed()

			//HZ_CORE_TRACE("{0}", x);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

}