#include "game.h"
#include "model.h"
#include "visibleobject.h"
#include "platform.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <tuple>

Game::Game(int width, int height) : State{GameState::GAME_ACTIVE}, Keys{}, ScreenWidth{width}, ScreenHeight{height}, GameObjects{}, Shaders{}, PlayerCharacter{glm::vec3{1.0f, 1.5f, 1.0f}, glm::vec3{3.0f}, 0.85f}
{
}

// Initialise the game, including creating game objects and setting their initial positions, and setting other unchanging values
void Game::init()
{
	// Initialise shaders
	Shaders.emplace_back(Shader{"shaders/shader.vert", "shaders/shader.frag"});
	Shaders.emplace_back(Shader{"shaders/skybox.vert", "shaders/skybox.frag"});

	// Create sky cube
	const auto skyboxModel{Model{"media/skycube/skycube.obj"}};
	GameObjects.emplace_back(std::make_unique<VisibleObject>(skyboxModel, Shaders[1], glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{80.0f}));


	// PLATFORMS START
	const auto platformModel{Model{"media/platform/platform.obj"}};
	constexpr auto platformSize{glm::vec3{2.0f, 1.0f, 2.0f}};

	GameObjects.emplace_back
	(
		std::make_unique<Platform>
		(
			platformModel,
			Shaders[0],
			glm::vec3{0.0f},
			glm::vec3{4.0f, 1.0f, 4.0f},
			false,
			glm::vec3{1.0f, 0.0f, 1.0f},
			glm::vec3{2.0f, 1.0f, 2.0f}
		)
	);

	GameObjects.emplace_back(std::make_unique<Platform>(platformModel, Shaders[0], glm::vec3{0.0f, 0.0f, -5.5f}, platformSize));
	GameObjects.emplace_back(std::make_unique<Platform>(platformModel, Shaders[0], glm::vec3{0.0f, 1.5f, -10.0f}, platformSize));
	GameObjects.emplace_back(std::make_unique<Platform>(platformModel, Shaders[0], glm::vec3{6.5f, -1.0f, -10.0f}, platformSize));
	GameObjects.emplace_back(std::make_unique<Platform>(platformModel, Shaders[0], glm::vec3{12.5f, -5.0f, -5.0f}, platformSize));
	GameObjects.emplace_back(std::make_unique<Platform>(platformModel, Shaders[0], glm::vec3{17.5f, -4.0f, -5.0f}, platformSize));
	GameObjects.emplace_back(std::make_unique<Platform>(platformModel, Shaders[0], glm::vec3{17.5f, -2.5f, -9.0f}, platformSize));
	GameObjects.emplace_back(std::make_unique<Platform>(platformModel, Shaders[0], glm::vec3{22.0f, -1.0f, -8.5f}, platformSize));

	GameObjects.emplace_back
	(
		std::make_unique<Platform>
		(
			platformModel,
			Shaders[0],
			glm::vec3{30.0f, -4.5f, -8.5f},
			glm::vec3{5.0f, 1.0f, 5.0f}, 
			false,
			glm::vec3{1.75f, 0.0f, 1.75f},
			glm::vec3{3.0f, 1.0f, 3.0f}
		)
	);

	// PLATFORMS END

	// Initialise all game objects
	for (auto& obj : GameObjects)
	{
		obj->init();
	}

	// Projection matrix doesn't change so can be initialised here
	const auto projection{glm::perspective(glm::radians(PlayerCharacter.getFov()), static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight), 0.1f, 1000.0f)};
	for (const auto& shader : Shaders)
	{
		shader.use();
		shader.setUniform("projection", projection);
	}
}

// Handle received keyboard input by triggering functionality in controllable GameObjects
void Game::processInput()
{
	// Send keyboard movement input to the player character
	if (Keys[GLFW_KEY_W])
		PlayerCharacter.processKeyboard(PlayerMovement::FORWARD);
	if (Keys[GLFW_KEY_S])
		PlayerCharacter.processKeyboard(PlayerMovement::BACKWARD);
	if (Keys[GLFW_KEY_A])
		PlayerCharacter.processKeyboard(PlayerMovement::LEFT);
	if (Keys[GLFW_KEY_D])
		PlayerCharacter.processKeyboard(PlayerMovement::RIGHT);
	if (Keys[GLFW_KEY_SPACE])
		PlayerCharacter.processKeyboard(PlayerMovement::JUMP_PRESSED);
	if (!Keys[GLFW_KEY_SPACE])
		PlayerCharacter.processKeyboard(PlayerMovement::JUMP_RELEASED);
}

// Update the positions of GameObjects, apply forces, check collisions, and perform other relevant per-tick checks (e.g., game over)
void Game::update(float deltaTime)
{
	// Tick player character separately due to architectural constraints
	PlayerCharacter.tick(deltaTime);

	// Move any objects that have velocity
	for (auto& obj : GameObjects)
	{
		obj->tick(deltaTime);
	}

	// After do object movement, check apply gravity and do collisions
	applyGravity();
	doCollisions();
	checkGameOver();

	// Set shader values
	const auto view{PlayerCharacter.getViewMatrix()}; // View matrix based on the player's view
	const auto lightPos{view * glm::vec4{-0.75, -0.5, -0.3, 0.0}}; // Light position in view space
	constexpr auto lightColor{glm::vec3{1.0}}; // Light colour
	for (const auto& shader : Shaders)
	{
		shader.use();
		shader.setUniform("view", view);
		shader.setUniform("light.position", lightPos);
		shader.setUniform("light.color", lightColor);
	}
}

// Render all GameObjects and get the latest view matrix (which defines the player's view) from player character
void Game::render()
{
	// Don't render anything without shaders
	if (Shaders.empty())
	{
		return;
	}

	for (const auto& obj : GameObjects)
	{
		obj->draw();
	}
}

// Check for and resolve collisions between the player character and game objects. Also set the grounded state of the player (used for jumping logic)
void Game::doCollisions()
{
	static auto groundColCount{0};

	for (const auto& obj : GameObjects)
	{
		const auto playerPos{PlayerCharacter.getPosition()};
		const auto playerRad{PlayerCharacter.getRadius()};

		const auto collision{checkCollision(PlayerCharacter, *obj)};

		// If collision occurred...
		if (std::get<0>(collision))
		{
			const auto dir{std::get<1>(collision)};
			const auto diffVector{std::get<2>(collision)};

			if (dir == Direction::X_POS || dir == Direction::X_NEG)
			{
				const auto penetration{playerRad - std::abs(diffVector.x)};

				if (dir == Direction::X_POS)
				{
					const auto newX{playerPos.x - penetration};
					PlayerCharacter.setPosition(glm::vec3{newX, playerPos.y, playerPos.z});
				}

				if (dir == Direction::X_NEG)
				{
					const auto newX{playerPos.x + penetration};
					PlayerCharacter.setPosition(glm::vec3{newX, playerPos.y, playerPos.z});
				}
			}
			else if (dir == Direction::Y_POS || dir == Direction::Y_NEG)
			{
				const auto penetration{playerRad - std::abs(diffVector.y)};

				if (dir == Direction::Y_POS)
				{
					const auto newY{playerPos.y - penetration};
					PlayerCharacter.setPosition(glm::vec3{playerPos.x, newY, playerPos.z});
				}
				else if (dir == Direction::Y_NEG) // Top surface of obstacle
				{
					++groundColCount;

					const auto newY{playerPos.y + penetration};
					PlayerCharacter.setPosition(glm::vec3{playerPos.x, newY, playerPos.z});
				}
			}
			else
			{
				const auto penetration{playerRad - std::abs(diffVector.z)};

				if (dir == Direction::Z_POS)
				{
					const auto newZ{playerPos.z - penetration};
					PlayerCharacter.setPosition(glm::vec3{playerPos.x, playerPos.y, newZ});
				}
				else if (dir == Direction::Z_NEG)
				{
					const auto newZ{playerPos.z + penetration};
					PlayerCharacter.setPosition(glm::vec3{playerPos.x, playerPos.y, newZ});
				}
			}
		}
	}

	// Collision with ground (top surface of objects) must occur for a minimum period before player is considered grounded
	if (groundColCount >= 1)
	{
		PlayerCharacter.setGrounded(true);
		groundColCount = 0;
	}
}

// Apply the force of gravity to the player, adding negative Y-axis velocity
void Game::applyGravity()
{
	constexpr auto gravity{0.05f};

	// Apply force of gravity
	PlayerCharacter.addVelocity(glm::vec3{0.0f, -gravity, 0.0f});
}

// Check if the player is has fallen too far and if so reset their position
void Game::checkGameOver()
{
	constexpr auto minHeight{-10.0f};
	constexpr auto startPos{glm::vec3{1.0f, 1.5f, 1.0f}};

	if (PlayerCharacter.getPosition().y <= minHeight)
		PlayerCharacter.setPosition(startPos);
}

// Get the direction the player is a colliding with an object from
Direction Game::getVectorDirection(const glm::vec3& target)
{
	constexpr glm::vec3 directions[]
	{
		glm::vec3{1.0f, 0.0f, 0.0f}, // Positive X
		glm::vec3{-1.0f, 0.0f, 0.0f}, // Negative X
		glm::vec3{0.0f, 1.0f, 0.0f}, // Positive Y
		glm::vec3{0.0f, -1.0f, 0.0f}, // Negative Y
		glm::vec3{0.0f, 0.0f, 1.0f}, // Positive Z
		glm::vec3{0.0f, 0.0f, -1.0f} // Negative Z
	};

	auto max{0.0f};

	auto bestMatch{-1};

	for (auto i{0}; i < 6; ++i)
	{
		const auto dotProduct{dot(normalize(target), directions[i])};

		if (dotProduct > max)
		{
			max = dotProduct;

			bestMatch = i;
		}
	}

	return static_cast<Direction>(bestMatch);
}

// Set the pressed state of a particular key
void Game::setKeyState(int key, bool pressed)
{
	Keys[key] = pressed;
}

// Get input (cursor X and Y offsets since last update) from a mouse cursor
void Game::setMouseInput(float xOffset, float yOffset)
{
	PlayerCharacter.processMouseMovement(xOffset, yOffset);
}

// Get input (scroll wheel X and Y offset) from a mouse scroll wheel
void Game::setScrollInput(float xOffset, float yOffset)
{
}

// Check for collision between AABB objects and a player character defined by a sphere
Collision Game::checkCollision(const Character& camera, const GameObject& object)
{
	const auto cameraPos{camera.getPosition()};
	const auto cameraRad{camera.getRadius()};

	// Find camera centre by adding radius to position
	const auto cameraCentre{cameraPos + cameraRad};

	// Find ABB centre and half-extents
	const auto objectSize{object.getSize()};
	const auto objectPos{object.getPosition()};
	const auto aabbHalfExtents
	{
		glm::vec3
		{
			objectSize.x / 2.0f,
			objectSize.y / 2.0f,
			objectSize.z / 2.0f
		}
	};
	const auto aabbCentre
	{
		glm::vec3
		{
			objectPos.x + aabbHalfExtents.x,
			objectPos.y + aabbHalfExtents.y,
			objectPos.z + aabbHalfExtents.z
		}
	};

	// Find difference between both centres
	const auto difference{cameraCentre - aabbCentre};
	const auto clampedDiff{clamp(difference, -aabbHalfExtents, aabbHalfExtents)};

	// Find position closest to camera (the circle)
	const auto closestPoint{aabbCentre + clampedDiff};

	// Get vector between circle's centre and closest AABB point, and check if length is less than or equal to circle's radius
	const auto length{closestPoint - cameraCentre};

	// If collision...
	if (glm::length(length) < cameraRad)
		return std::make_tuple(true, getVectorDirection(length), length);

	return std::make_tuple(false, Direction::X_POS, glm::vec3{0.0f});
}
