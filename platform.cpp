#include "platform.h"
#include <GLFW/glfw3.h>
#include "effolkronium/random.hpp"

Platform::Platform(const Model& model, const Shader& shader, const glm::vec3& position, const glm::vec3& size, bool oscillate, const glm::vec3& offset, const glm::vec3& scale)
	: VisibleObject{model, shader, position, size, offset, scale},
	offset_{effolkronium::random_thread_local::get<float>(1, 10)},
	oscillate_{oscillate}
{
}

void Platform::init()
{
}

void Platform::tick(float deltaTime)
{
	if (oscillate_)
	{
		const auto y{sin(static_cast<float>(glfwGetTime())) * 0.0025f * offset_ * deltaTime};

		addVelocity(glm::vec3{0.0f, y, 0.0f});

		move();
	}
}

void Platform::setOscillate(bool oscillate)
{
	oscillate_ = oscillate;
}

bool Platform::getOscillate() const
{
	return oscillate_;
}
