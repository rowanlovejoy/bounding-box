#include "visibleobject.h"
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <utility>

VisibleObject::VisibleObject
(
	Model model,
	Shader shader,
	const glm::vec3& position,
	const glm::vec3& size,
	const glm::vec3& offset,
	const glm::vec3& scale
)
	: GameObject{position, size},
	model_{std::move(model)},
	scale_{scale},
	offset_{offset},
	shader_{shader}
{
}

// Calculate transform for the model and then render it using the provided shader
void VisibleObject::draw() const
{
	auto transform{glm::mat4{1.0f}};

	transform = translate(transform, position_);

	// Offset models slightly on X and Y to line up with collision box -- may be necessary if model is scaled
	transform = translate(transform, offset_);

	transform = scale(transform, scale_);

	model_.draw(shader_, transform);
}
