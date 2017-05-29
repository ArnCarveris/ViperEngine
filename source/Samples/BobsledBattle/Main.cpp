#include "Pch.h"
#include <windows.h>
#include "Core/ServiceLocator.h"
#include "Core/ModuleLoader.h"
#include "Graphics/Mesh.h"
#include "Memory/MemoryManager.h"
#include "Service/AudioManager.h"
#include "Service/RendererSystem.h"


#ifndef UNICODE
#define UNICODE
#endif


#define WIDTH		1280
#define HEIGHT		720
#define TITLE		"Bobsled Battle"
#define CONFIG_FILE "Config.ini"

using namespace Viper;


void Initialize()
{
	MemoryManager* allocator = new MemoryManager();
	assert(allocator != nullptr);
	Core::ModuleLoader::CreateInstance(*allocator);
	Core::ModuleLoader::GetInstance().LoadModules(CONFIG_FILE);
}

void ShutDown()
{
	MemoryAllocator& allocator = ServiceLocator::GetInstance().GetMemoryAllocator();
	Core::ModuleLoader::GetInstance().UnloadModules();
	Core::ModuleLoader::Destroy();
	delete &allocator;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	Initialize();

	WindowManager& windowManager = ServiceLocator::GetInstance().GetWindowManager();
	windowManager.Initialize();
	const WindowContext& windowContext = windowManager.CreateGameWindow(WIDTH, HEIGHT, TITLE);

	InputManager& inputManager = ServiceLocator::GetInstance().GetInputManager();
	inputManager.Initialize(windowContext);

	RendererSystem& rendererSystem = ServiceLocator::GetInstance().GetRendererSystem();
	rendererSystem.Initialize();
	rendererSystem.SetViewport(windowContext);

	TextureLoader& loader = ServiceLocator::GetInstance().GetTextureLoader();
	Graphics::Texture textureObj1 = loader.LoadTexture("Content/Textures/wall.jpg");
	Graphics::Texture textureObj2 = loader.LoadTexture("Content/Textures/decal.jpg");

	Graphics::Shader vertexShader = rendererSystem.LoadShaderFile("Content/Shaders/default.vert", Graphics::ShaderType::VERTEX_SHADER);
	Graphics::Shader fragmentShader = rendererSystem.LoadShaderFile("Content/Shaders/default.frag", Graphics::ShaderType::FRAGMENT_SHADER);
	rendererSystem.UseShaders({vertexShader, fragmentShader});

	// vertices and indices to vertices for a tringle
	float vertices[] = {
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
		0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
	};
	std::uint32_t elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	Graphics::Mesh mesh(4, vertices, 2, elements);
	rendererSystem.LoadMesh(mesh);

	// Test Audio
	AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
	audioManager.SetListener3dAttributes(Viper::Vector3(0, 0, 0), Viper::Vector3(0, 0, 1), Viper::Vector3(0, 1, 0));
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.bank");
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.strings.bank");
	audioManager.LoadSoundBankEvents("Content/Sounds/ZombieWars.bank");

	Viper::Vector3 position(0, 0, 0);
	audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
	audioManager.PlayEvent("event:/GattlingGun-Fire");

	while (windowManager.BeginUpdate(windowContext))
	{
		inputManager.Update();
		if (inputManager.GetButtonState(Button::Key_Escape) == ButtonState::Pressed)
		{
			break;
		}
		rendererSystem.Update();

		if (inputManager.GetButtonState(Button::Key_Up) == ButtonState::Pressed)
		{
			position.z += 0.01f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		}
		if (inputManager.GetButtonState(Button::Key_Down) == ButtonState::Pressed)
		{
			position.z -= 0.01f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		}
		if (inputManager.GetButtonState(Button::Key_Space) == ButtonState::Pressed && inputManager.IsModifierActive(ModifierKey::Control))
		{
			position.z = 0.0f;
			audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		}
		audioManager.Update();

		windowManager.EndUpdate(windowContext);
	}

	rendererSystem.Shutdown();
	windowManager.Shutdown();
	ShutDown();
	return 0;
}
