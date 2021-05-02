#include "platform.h"
#include <GLFW/glfw3.h>
#include "effolkronium/random.hpp"

Platform::Platform(const Model& model, const Shader& shader, const glm::vec3& position, const glm::vec3& size, const glm::vec3& velocity, const glm::vec3& offset, const glm::vec3& scale)
	: VisibleObject(model, shader, position, size, velocity, offset, scale),
	  offset_{effolkronium::random_thread_local::get<float>(1, 10)}
{
}

void Platform::init()
{
}

void Platform::tick(float deltaTime)
{
	const auto y{sin(glfwGetTime()) * 0.0025f * offset_ * deltaTime};

	addVelocity(glm::vec3{0.0f, y, 0.0f});

	move();
}
