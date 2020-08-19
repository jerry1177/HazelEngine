#include "hzpch.h"
#include "Renderer2D.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();
		s_Data = new Renderer2DStorage();
		//*
		s_Data->QuadVertexArray = VertexArray::Create();

		//index buffer
		float squarVerticies[4 * 5] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareBF = VertexBuffer::Create(squarVerticies, sizeof(squarVerticies));
		squareBF->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TextCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareBF);

		uint32_t squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t));

		s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->textureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->textureShader->Bind();
		s_Data->textureShader->SetInt("u_Texture", 0);
		//*/
	}

	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();
		s_Data->textureShader->Bind();
		s_Data->textureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		HZ_PROFILE_FUNCTION();
		// Bind White texture here
		s_Data->whiteTexture->Bind();
		s_Data->textureShader->SetFloat4("u_Color", color);
		s_Data->textureShader->SetFloat("u_TilingFactor", 1.0f);

		glm::mat4 tranform1 = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->textureShader->SetMat4("u_Transform", tranform1);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();
		// Bind White texture here
		s_Data->whiteTexture->Bind();
		s_Data->textureShader->SetFloat4("u_Color", color);
		s_Data->textureShader->SetFloat("u_TilingFactor", 1.0f);

		glm::mat4 tranform1 = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0,0,1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->textureShader->SetMat4("u_Transform", tranform1);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const glm::vec4& tintColor) {
		HZ_PROFILE_FUNCTION();
		s_Data->textureShader->SetFloat4("u_Color", tintColor);
		s_Data->textureShader->SetFloat("u_TilingFactor", tileFactor);
		texture->Bind();

		glm::mat4 tranform1 = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->textureShader->SetMat4("u_Transform", tranform1);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const glm::vec4& tintColor) {
		DrawQuad({position.x, position.y, 0.0f}, size, texture, tileFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const glm::vec4& tintColor) {
		HZ_PROFILE_FUNCTION();
		s_Data->textureShader->SetFloat4("u_Color", tintColor);
		s_Data->textureShader->SetFloat("u_TilingFactor", tileFactor);
		texture->Bind();

		glm::mat4 tranform1 = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->textureShader->SetMat4("u_Transform", tranform1);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tileFactor, const glm::vec4& tintColor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tileFactor, tintColor);
	}
}