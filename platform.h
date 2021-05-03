#pragma once

#include "visibleobject.h"

class Platform : public VisibleObject
{
public:
	Platform(const Model& model, const Shader& shader, const glm::vec3& position, const glm::vec3& size, bool oscillate = true, const glm::vec3& offset = glm::vec3{0.0}, const glm::vec3& scale = glm::vec3{1.0});

	virtual void init() override;

	virtual void tick(float deltaTime) override;

	void setOscillate(bool oscillate);
	bool getOscillate() const;

private:
	float offset_;
	bool oscillate_;
};
