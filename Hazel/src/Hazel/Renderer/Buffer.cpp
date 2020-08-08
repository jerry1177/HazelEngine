#include "hzpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: 
			HZ_CORE_ASSERT(false, "RendererAPI::None is not superted!");
			return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(verticies, size);

		default:
			break;
		}

		HZ_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* verticies, uint32_t size) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is not superted!");
			return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(verticies, size);

		default:
			break;
		}

		HZ_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}


}