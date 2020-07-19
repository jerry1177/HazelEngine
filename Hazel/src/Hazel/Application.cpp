#include "hzpch.h"
#include "Application.h"

#include "GLFW/glfw3.h"
namespace Hazel {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Running = true;
	}

	Application::~Application() {
	
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		HZ_CORE_TRACE("{0}", e);
	}

	

	void Application::Run() {
		WindowResizeEvent e(1200, 720);
		if (e.IsInCategory(EventCategoryApplication)) HZ_INFO(e);
		if (!e.IsInCategory(EventCategoryKeyboard)) HZ_ERROR(e);
		while (m_Running) { 
			glClearColor(0, .5, 1, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

}