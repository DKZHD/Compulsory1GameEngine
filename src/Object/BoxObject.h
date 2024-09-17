#pragma once
#include "Object.h"

class BallObject;

class BoxObject : public Object
{
public:
	BoxObject(glm::vec2 pos, glm::vec3 color, glm::vec2 size);
	BoxObject(glm::vec3 pos, glm::vec3 color, glm::vec3 size);
	void Render(unsigned program) override;
	bool checkCollision(glm::vec2& pos, float radius, glm::vec2& vel, glm::vec2 prevPos);
private:
	void CreateBox2D(glm::vec2 pos, glm::vec3 color, glm::vec2 size);
	void CreateBox3D(glm::vec3 pos, glm::vec3 color, glm::vec3 size);
};
