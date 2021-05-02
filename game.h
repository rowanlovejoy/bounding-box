#pragma once

#include "character.h"
#include <memory>
#include <vector>

enum class GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_DEBUG
};

enum class Direction
{
	X_POS,
	X_NEG,
	Y_POS,
	Y_NEG,
	Z_POS,
	Z_NEG
};

// Type to contain data about collisions between the player and other objects
using Collision = std::tuple<bool, Direction, glm::vec3>;

// Class representing an instance of the game, defining its initialisation, update, and render behaviour. Also receives inputs from external window system, and maintains references to in-game objects and the player character.
class Game
{
public:
	Game(int width, int height);
	void init();
	void processInput();
	void update(float deltaTime);
	void render();
	void setKeyState(int key, bool pressed);
	void setMouseInput(float xOffset, float yOffset);
	void setScrollInput(float xOffset, float yOffset);

private:
	GameState State;
	bool Keys[1024];
	int ScreenWidth;
	int ScreenHeight;

	std::vector<std::unique_ptr<GameObject>> GameObjects;
	std::vector<Shader> Shaders;
	Character PlayerCharacter;

	void doCollisions();
	void applyGravity();
	void checkGameOver();
	static Collision checkCollision(const Character& camera, const GameObject& object);
	static Direction getVectorDirection(const glm::vec3& target);
};
