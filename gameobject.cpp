#include "gameobject.h"

GameObject::GameObject
(
	const glm::vec3& position,
	const glm::vec3& size,
	const glm::vec3& velocity
)
	: position_{position}, size_{size}, velocity_{velocity}
{
}

void GameObject::draw() const
{
}

// Consume accumulated velocity to move the object in the game world
void GameObject::move()
{
	position_ += velocity_;

	// Reset velocity to prevent constantly increasing speed
	velocity_ = glm::vec3{0.0};
}

void GameObject::init()
{
}

void GameObject::setPosition(const glm::vec3& newPos)
{
	position_ = newPos;
}

const glm::vec3& GameObject::getPosition() const
{
	return position_;
}

void GameObject::setSize(const glm::vec3& size)
{
	size_ = size;
}

const glm::vec3& GameObject::getSize() const
{
	return size_;
}

void GameObject::addVelocity(const glm::vec3& direction)
{
	velocity_ += direction;
}

const glm::vec3& GameObject::getVelocity() const
{
	return velocity_;
}

void GameObject::tick(float deltaTime)
{
	move();
}
