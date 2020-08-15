#include "hzpch.h"
#include "Renderer2D.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Renderer/RenderCommand.h"

namespace Hazel {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> flatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		
		s_Data = new Renderer2DStorage();
		//*
		s_Data->QuadVertexArray = VertexArray::Create();

		//index buffer
		float squarVerticies[4 * 3] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareBF = VertexBuffer::Create(squarVerticies, sizeof(squarVerticies));
		squareBF->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareBF);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t));

		s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->flatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		//*/
	}

	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->flatColorShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::mat4& transform)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, transform);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::mat4& transform)
	{
		s_Data->flatColorShader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(s_Data->flatColorShader)->UploadUniformFloat4("u_Color", color);
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColorShader)->UploadUniformMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}