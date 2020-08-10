#pragma once

#include <string>


namespace Crystal {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fagmentSrc);
		static Shader* Create(const std::string& path);
	};
}