#include "crystalpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Crystal {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Crystal::ShaderDataType::Float:
			case Crystal::ShaderDataType::Float2:
			case Crystal::ShaderDataType::Float3:
			case Crystal::ShaderDataType::Float4:
			case Crystal::ShaderDataType::Mat3:
			case Crystal::ShaderDataType::Mat4:  return GL_FLOAT;
			case Crystal::ShaderDataType::Int:
			case Crystal::ShaderDataType::Int2:
			case Crystal::ShaderDataType::Int3:
			case Crystal::ShaderDataType::Int4:  return GL_INT;
			case Crystal::ShaderDataType::Bool:  return GL_BOOL;
		}

		CRYSTAL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		CRYSTAL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}