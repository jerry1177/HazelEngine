#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is not superted!");
			return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();

		default:
			break;
		}

		HZ_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}