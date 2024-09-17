#pragma once
#include "Object.h"
#include "../Physics/Physics.h"


class ObjectManager;

class BallObject : public Object
{
public:
	BallObject(glm::vec2 pos, float radius, float resolution, glm::vec3 color);
	BallObject(glm::vec3 pos, float radius, int numSubDivs, glm::vec3 color);

	//Setters
	void SetPosition(glm::vec2 newPos);
	void SetVelocity(glm::vec2 newVel);

	//Update
	void UpdatePosition(glm::vec2 pos);
	void Update(float deltaTime);
	void UpdateVelocity(glm::vec2 vel);

	//Getters
	glm::vec2& GetVelocity();
	float GetRadius();
	glm::vec2& GetPosition();

	//Misc
	float mass = 0.2f;
	void Render(unsigned program) override;
	bool checkCollision(BallObject& object);

private:
	friend class ObjectManager;
	//Creation Functions
	void Create2D(glm::vec2 pos, float radius, float resolution, glm::vec3 color);
	void subDivide(int index1, int index2, int index3, int i);
	void Create3D(glm::vec3 pos, float radius, int subdivs, glm::vec3 color);

	Physics mPhysics {mTransform.pos,glm::vec2(0.f)};
	glm::vec2 prevPos = glm::vec2(0.f);
	glm::vec3 mVelocity = {0.f,0.f,0.f};
	float maxSpeed = 3.f;
	
};

