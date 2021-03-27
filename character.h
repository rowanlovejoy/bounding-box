#pragma once

#include "gameobject.h"
#include <glm/glm.hpp>

enum class PlayerMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	JUMP_PRESSED,
	JUMP_RELEASED
};

// Class representing a player controlled character, with input handling and a first person camera
class Character : public GameObject
{
public:
	Character(const glm::vec3& pos, const glm::vec3& siz, float radius);

	// Process input received from the keyboard
	void processKeyboard(PlayerMovement direction);

	// Process input received from a mouse
	void processMouseMovement(float xOffset, float yOffset);

	// Get view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() const;

	float getFov() const
	{
		return Fov;
	}

	glm::vec3 getFront() const
	{
		return Front;
	}

	float getRadius() const
	{
		return Radius;
	}

	float getMovementSpeed() const
	{
		return MovementSpeed;
	}

	void setMovementSpeed(float newSpeed)
	{
		if (newSpeed < 0.0f)
			newSpeed = 0.0f;

		MovementSpeed = newSpeed;
	}

	bool getGrounded() const
	{
		return Grounded;
	}
	
	void setGrounded(bool grounded)
	{
		Grounded = grounded;
	}

private:

	glm::vec3 Front;
	glm::vec3 Forward;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Fov;
	float Radius;

	bool Grounded;
	
	// Calculates the front vector from the camera's current Euler Angles
	void updateDirectionVectors();
};
