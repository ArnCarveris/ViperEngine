#include "Pch.h"
#include "Shader.h"

namespace Viper
{
	namespace Graphics
	{
		Shader::Shader(std::uint32_t shaderId, ShaderType shaderType) :
			id(shaderId), type(shaderType)
		{
		}

		std::uint32_t Shader::Id() const
		{
			return id;
		}

		ShaderType Shader::Type() const
		{
			return type;
		}
	}
}
