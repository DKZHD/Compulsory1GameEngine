#pragma once
#include <glm/vec2.hpp>

class Physics
{
public:
	void UpdatePos(glm::vec2 velChange);

	glm::vec2 pos = glm::vec2(0.f);
	glm::vec2 vel = glm::vec2(0.f);
};

