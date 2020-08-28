#pragma once

#include "Hazel/Renderer/RendererAPI.h"


namespace Hazel {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		void Init() override;
		void SetClearColor(const glm::vec4& color) override;
		void Clrear() override;
		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) override;
	};
}