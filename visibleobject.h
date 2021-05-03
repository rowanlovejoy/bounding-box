#pragma once

#include "gameobject.h"
#include "model.h"
#include "shader.h"
#include <glm/vec3.hpp>

// Class representing a GameObject that is represented visually with 3D model. Includes functionality for drawing the model and settings it scale and offset relative to its containing GameObject instance.
class VisibleObject : public GameObject
{
public:
	VisibleObject(Model model, Shader shader, const glm::vec3& position, const glm::vec3& size, const glm::vec3& offset = glm::vec3{0.0}, const glm::vec3& scale = glm::vec3{1.0});

	virtual void draw() const override;

private:
	Model model_;
	glm::vec3 scale_;
	glm::vec3 offset_;
	Shader shader_;
};
