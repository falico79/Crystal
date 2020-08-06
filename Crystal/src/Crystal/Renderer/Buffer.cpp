#include "crystalpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Crystal {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CRYSTAL_CORE_ASSERT(false, "RenderAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);

		}

		CRYSTAL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CRYSTAL_CORE_ASSERT(false, "RenderAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);

		}

		CRYSTAL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}