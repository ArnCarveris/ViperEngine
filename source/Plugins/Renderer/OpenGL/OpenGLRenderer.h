#pragma once

#include "Service/RendererSystem.h"
#include "glad/glad.h"
#include "Window/WindowContext.h"


struct GLFWwindow;

namespace Viper
{
	namespace Renderer
	{
		class OpenGLRenderer final : public RendererSystem
		{
		public:
			OpenGLRenderer() = default;
			~OpenGLRenderer() = default;

			void Initialize() override;
			void SetViewport(const WindowContext& windowContext) override;
			void Update() override;
			void Shutdown() override;

		private:
			GLuint shaderProgram;
			GLuint VAO;
		};
	}
}
