#pragma once

#include "shader.h"
#include <glm/vec3.hpp>

// Class representing an in-game entity with a position, velocity, collision size. Contains an overridable method for drawing an associated model, which is to be implemented in derived classes.
class GameObject
{
public:
	virtual ~GameObject() = default;
	GameObject(const glm::vec3& position, const glm::vec3& size);

	virtual void draw() const;
	virtual void move();
	virtual void init();
	virtual void tick(float deltaTime);

	void setPosition(const glm::vec3& newPos);
	const glm::vec3& getPosition() const;

	void setSize(const glm::vec3& size);
	const glm::vec3& getSize() const;

	void addVelocity(const glm::vec3& direction);
	const glm::vec3& getVelocity() const;

protected:
	glm::vec3 position_;
	glm::vec3 size_;
	glm::vec3 velocity_;
};
