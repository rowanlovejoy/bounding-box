#pragma once

#include "gameobject.h"
#include "model.h"
#include "shader.h"
#include <glm/vec3.hpp>

// Class representing a GameObject that is represented visually with 3D model. Includes functionality for drawing the model and settings it scale and offset relative to its containing GameObject instance.
class VisibleObject : public GameObject
{
public:
	VisibleObject(const glm::vec3& pos, const glm::vec3& siz, const Model& mod, const glm::vec3& scale = glm::vec3{1.0f}, const glm::vec3& offset = glm::vec3{0.0f});

	virtual void draw(const Shader& shader) const override;

private:
	Model ObjectModel;
	glm::vec3 Scale;
	glm::vec3 Offset;
};
