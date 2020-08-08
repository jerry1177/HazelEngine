#include "hzpch.h"
#include "Application.h"
#include "Hazel/Input.h"
#include <glad/glad.h>
namespace Hazel {
	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDatatypeToOpenGLType(ShaderDataType type) {
		switch (type)
		{
		case Hazel::ShaderDataType::Float:	return GL_FLOAT;
		case Hazel::ShaderDataType::Float2:	return GL_FLOAT;
		case Hazel::ShaderDataType::Float3:	return GL_FLOAT;
		case Hazel::ShaderDataType::Float4:	return GL_FLOAT;
		case Hazel::ShaderDataType::Mat3:	return GL_FLOAT;
		case Hazel::ShaderDataType::Mat4:	return GL_FLOAT;
		case Hazel::ShaderDataType::Int:	return GL_INT;
		case Hazel::ShaderDataType::Int2:	return GL_INT;
		case Hazel::ShaderDataType::Int3:	return GL_INT;
		case Hazel::ShaderDataType::Int4:	return GL_INT;
		case Hazel::ShaderDataType::Bool:	return GL_BOOL;

		default:
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	}

	Application::Application() {
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
		//vertex array
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);
		

		//index buffer
		float verticies[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		//vertex buffer
		m_VertexBuffer.reset(VertexBuffer::Create(verticies, sizeof(verticies)));

		
		// tell opengl the layout of buffer
		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};
			m_VertexBuffer->SetLayout(layout);
		}
		
		const auto& layout = m_VertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), 
				ShaderDatatypeToOpenGLType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(), 
				(const void*)element.Offset);
			index++;
		}
		unsigned int indicies[3] = { 0, 1, 2 };
		
		m_IndexBuffer.reset(IndexBuffer::Create(indicies, sizeof(indicies)/sizeof(int32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position+0.5, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Position*0.5+0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(.1, .1, .1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//bool x = Input::IsKeyPressed()

			//HZ_CORE_TRACE("{0}", x);
			//*
			m_ImGuiLayer->Begin();
			// ImGui render sits between begin and end 
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			//*/
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

}