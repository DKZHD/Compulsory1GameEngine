#pragma once

#include <memory>
#include "../Object/BoxObject.h"

class ObjectManager;

class QuadTree : public BoxObject
{
public:
	QuadTree(glm::vec2 bottomLeft, glm::vec2 topRight);
	QuadTree* FindQuad(BallObject& object);
	void Render(unsigned program) override;
	void Init(ObjectManager& obj_manager);
	void SubDivide();
private:

	QuadTree* parent = nullptr;
	std::unique_ptr<QuadTree> leftTop = nullptr;
	std::unique_ptr<QuadTree> rightTop = nullptr;
	std::unique_ptr<QuadTree> leftBot = nullptr;
	std::unique_ptr<QuadTree> rightBot = nullptr;
	ObjectManager* objManagerRef = nullptr;
	std::vector<BallObject*> containedBallObjects;
};
