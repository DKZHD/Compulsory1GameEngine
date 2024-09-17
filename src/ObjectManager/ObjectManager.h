#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class QuadTree;
class BoxObject;
class BallObject;
class Object;

class ObjectManager
{
public:
	void AddBall(glm::vec2 pos, float radius, float resolution, glm::vec3 color);
	void Add3DBall(glm::vec3 pos, float radius, int numSubDivs, glm::vec3 color);
	void AddXAmountofBalls(int amount, float resolution, glm::vec3 color);
	void AddBox(glm::vec2 pos, glm::vec3 color, glm::vec2 size);
	void Add3DBox(glm::vec3 pos, glm::vec3 color, glm::vec3 size);

	void Render(unsigned program);
	void Update(float deltaTime, QuadTree& quad_tree);
	void CheckCollisions();
	void CheckCollisionSection(std::vector<BallObject*> ball_objects);

	BallObject* GetBallObjectAtIndex(int index);
private:
	friend QuadTree;
	friend BallObject;
	std::vector<BallObject> mBallObjects;
	std::vector<BoxObject> mBoxObjects;
};

