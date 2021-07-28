#include "crystalpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Crystal {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CRYSTAL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		CRYSTAL_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CRYSTAL_CORE_ASSERT(status, "Failed to initialize Glad!");

		CRYSTAL_CORE_INFO("OpenGL Info:");
		CRYSTAL_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CRYSTAL_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CRYSTAL_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		CRYSTAL_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Crystal requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		CRYSTAL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);

	}

}
