#pragma once

#include "shader.h"
#include <glm/vec3.hpp>

// Class representing an in-game entity with a position, velocity, collision size. Contains an overridable method for drawing an associated model, which is to be implemented in derived classes.
class GameObject
{
public:
	virtual ~GameObject() = default;
	GameObject(const glm::vec3& pos, const glm::vec3& siz, const glm::vec3& vel = glm::vec3{0.0f});
	virtual void draw(const Shader& shader) const;
	virtual void move();
	
	void setPosition(const glm::vec3& newPos)
	{
		Position = newPos;
	}
	
	const glm::vec3& getPosition() const
	{
		return Position;
	}

	const glm::vec3& getSize() const
	{
		return Size;
	}

	void addVelocity(const glm::vec3& direction)
	{
		Velocity += direction;
	}

	const glm::vec3& getVelocity() const
	{
		return Velocity;
	}

protected:
	glm::vec3 Position;
	glm::vec3 Size;
	glm::vec3 Velocity;
};
