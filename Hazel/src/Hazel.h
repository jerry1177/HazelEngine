#pragma once

// ------- Core Application --------
#include "Hazel/Core/Application.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/OrthoCameraController.h"

// ------- UI --------
#include "Hazel/ImGui/ImGuiLayer.h"

// ------- Renderer --------
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/SubTexture2D.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/FrameBuffer.h"

#include "Hazel/Renderer/OrthographicCamera.h"

// -------Entry Point--------
// Must be implemented on client side!
//#include "Hazel/Core/EntryPoint.h"
// --------------------------