#include "Physics.h"
#include <glm/common.hpp>

void Physics::UpdatePos(glm::vec2 velChange)
{
	vel = glm::clamp(vel + velChange, vel, glm::vec2(4.f));
	pos += vel;
}
