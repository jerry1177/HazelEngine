#include <Hazel.h>
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"


class SandBoxLayer : public Hazel::Layer {
public:
	SandBoxLayer() : Layer("Sandbox"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f), cameraPosition(0.0f,0.0f,0.0f), m_SquarePosition(0.0f) {
		m_VertexArray.reset(Hazel::VertexArray::Create());

		//index buffer
		float verticies[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.3f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.3f
		};

		//vertex buffer

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(verticies, sizeof(verticies)));

		// tell opengl the layout of buffer
		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indicies[3] = { 0, 1, 2 };

		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(int32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squarVerticies[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};


		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squarVerticies, sizeof(squarVerticies)));


		Hazel::BufferLayout squareVBLayout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" }
		};

		squareVB->SetLayout(squareVBLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(int32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main() {
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec4 u_Color;

			void main() {
				color = u_Color;
			}
		)";
		m_SquareShader.reset(new Hazel::Shader(vertexSrc2, fragmentSrc2));

	}
	~SandBoxLayer() {}

	void OnUpdate(Hazel::TimeStep ts) override {

		HZ_TRACE("DeltaTime: {0}s, {1}ms", ts.GetSeconds(), ts.GetMiliSeconds());
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_Camera);

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			cameraPosition.x -= m_MoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
			cameraPosition.y += m_MoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			cameraPosition.x += m_MoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
			cameraPosition.y -= m_MoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;


		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)) {
			m_Rotation -= m_RotationSpeed * ts;
			m_Camera.SetRotaion(m_Rotation);
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)) {
			m_Rotation += m_RotationSpeed * ts;
			m_Camera.SetRotaion(m_Rotation);
		}

		//rotation += 1.0f;


		m_Camera.SetPosition(cameraPosition);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.2, 0.3, 0.8, 1.0);
		glm::vec4 blueColor(0.8, 0.3, 0.2, 1.0);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				if (x % 2 == 0)
					m_SquareShader->UploadUniformFloat4("u_Color", redColor);
				else
					m_SquareShader->UploadUniformFloat4("u_Color", blueColor);

				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos + m_SquarePosition) * scale;
				Hazel::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
			}
		}
		

		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override {

		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(SandBoxLayer::OnKeyPressedEvent));
		
	}

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event) {
		
		return false;
	}

	void OnImGuiRender() override {

	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::Shader> m_SquareShader;

	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthogrphicCamera m_Camera;

	float m_RotationSpeed = 40.0f, m_Rotation = 0.0f, m_MoveSpeed = 3.0f;

	glm::vec3 cameraPosition;

	float m_SquareMoveSpeed = 1.0f;
	glm::vec3 m_SquarePosition;
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	Sandbox* sandBox = new Sandbox();
	SandBoxLayer* sbLayer = new SandBoxLayer();
	
	
	sandBox->PushLayer(sbLayer);

	return sandBox;
}