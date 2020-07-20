#pragma once
#include "Window.h"
#include "Core.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/LayerStack.h"

namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void OnEvent(Event& e);
		void Run();

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}

