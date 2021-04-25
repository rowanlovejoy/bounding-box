#pragma once

#include "shader.h"
#include <glm/vec3.hpp>

// Class representing an in-game entity with a position, velocity, collision size. Contains an overridable method for drawing an associated model, which is to be implemented in derived classes.
class GameObject
{
public:
	virtual ~GameObject() = default;
	GameObject(const glm::vec3& position, const glm::vec3& size, const glm::vec3& velocity = glm::vec3{0.0});
	
	virtual void draw() const;
	virtual void move();
	
	void setPosition(const glm::vec3& newPos);
	const glm::vec3& getPosition() const;
	const glm::vec3& getSize() const;
	
	void addVelocity(const glm::vec3& direction);
	const glm::vec3& getVelocity() const;

protected:
	glm::vec3 position_;
	glm::vec3 size_;
	glm::vec3 velocity_;
};
