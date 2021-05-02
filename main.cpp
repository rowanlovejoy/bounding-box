#include "main.h"
#include "game.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include <random>

// Constants defining viewport horizontal and vertical resolution
constexpr auto SCREEN_WIDTH{1280};
constexpr auto SCREEN_HEIGHT{720};

// Game instance is global to enable access by GLFW callbacks
Game gameInstance{SCREEN_WIDTH, SCREEN_HEIGHT};

// Initialise GLFW and run game loop
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	const auto window{glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bounding Box", nullptr, nullptr)};
	if (!window)
	{
		std::cout << "Failed to create GLFW window.\n";

		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);

	// Capture and hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Register callback to call on mouse input
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	// Register callback to call on keyboard input
	glfwSetKeyCallback(window, keyInputCallback);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	// Initialise GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD\n";

		return -1;
	}

	// Viewport
	stbi_set_flip_vertically_on_load(true);

	// OpenGL config
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	gameInstance.init();

	// Update rate limit, sixty times per second
	constexpr auto fpsLimit{1.0 / 60.0};
	auto frames{0};
	auto updates{0};

	auto lastTime{glfwGetTime()};
	auto timer{lastTime};
	double deltaTime{0};
	double nowTime{0};

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / fpsLimit;
		lastTime = nowTime;

		// Every sixty iterations delta time will be greater than or equal to one, so updates occur every sixty iterations
		while (deltaTime >= 1.0)
		{
			// Process input and update game state at a fixed rate
			glfwPollEvents();
			gameInstance.processInput();
			gameInstance.update(deltaTime);

			++updates;
			--deltaTime;
		}

		// Don't limit render rate
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gameInstance.render();
		glfwSwapBuffers(window);
		++frames;

		// Print rates and reset counters
		if (glfwGetTime() - timer > 1.0)
		{
			++timer;
			std::cout << "FPS: " << frames << ", Updates: " << updates << "\n";
			updates = 0;
			frames = 0;
		}
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

// Resize viewport on window resize
void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Callback on mouse cursor input
void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	static auto firstMouse{true};
	static auto lastX{static_cast<float>(SCREEN_WIDTH) / 2.0f};
	static auto lastY{static_cast<float>(SCREEN_HEIGHT) / 2.0f};

	// Handle first mouse input upon capture
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;

		firstMouse = false;
	}

	// Calculate mouse offset since last frame
	const auto xOffset{xPos - lastX};
	const auto yOffset{lastY - yPos}; // Y coordinates go from bottom top so operands are reverse from xOffset
	lastX = xPos;
	lastY = yPos;

	gameInstance.setMouseInput(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	gameInstance.setScrollInput(xOffset, yOffset);
}

// Process latest input received
void keyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	// Close window when escape is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			gameInstance.setKeyState(key, true);
		else if (action == GLFW_RELEASE)
			gameInstance.setKeyState(key, false);
	}
}
