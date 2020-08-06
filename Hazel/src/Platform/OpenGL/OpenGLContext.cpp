#include "hzpch.h"

#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Hazel {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		: m_WindowHandle(windowHandle) {
		HZ_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initiate Glad!");

		//glGetString(GL_VENDOR);
		HZ_CORE_INFO("OpenGL Info: \nRenderer: {0} \nVendor: {1} \nVersion: {2}", glGetString(GL_RENDERER), glGetString(GL_VENDOR), glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {

		glfwSwapBuffers(m_WindowHandle);
	}
}