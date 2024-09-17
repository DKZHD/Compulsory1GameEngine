#include "QuadTree.h"

#include <iostream>
#include <glad/glad.h>

#include "ObjectManager.h"
#include "../Object/BallObject.h"

QuadTree::QuadTree(glm::vec2 bottomLeft, glm::vec2 topRight) : BoxObject(bottomLeft,glm::vec3(1.f), glm::vec2(topRight.x - bottomLeft.x, topRight.y - bottomLeft.y))
{
}

QuadTree* QuadTree::FindQuad(BallObject& object)
{
	if (object.GetPosition().x > mTransform.pos.x + mTransform.size.x || object.GetPosition().x<mTransform.pos.x || object.GetPosition().y > mTransform.pos.y + mTransform.size.y || object.GetPosition().y < mTransform.pos.y)
		return nullptr;
	if(!leftBot || !leftTop || !rightTop || !rightBot)
	{
		if(parent!=nullptr)
		{
			containedBallObjects.push_back(&object);
			for(auto it = parent->containedBallObjects.begin(); it!=parent->containedBallObjects.end();++it)
			{
				if (*it == &object)
				{
					parent->containedBallObjects.erase(it);
					break;
				}
			}

		}
		return this;
	}
	if (containedBallObjects.size() > 15)
		SubDivide();
	//if (leftBot->containedBallObjects.size() + leftTop->containedBallObjects.size() +
	//	rightBot->containedBallObjects.size() + rightTop->containedBallObjects.size() + containedBallObjects.size() < 15)
	//{
	//	containedBallObjects.insert(containedBallObjects.begin(), leftBot->containedBallObjects.begin(), leftBot->containedBallObjects.end());
	//	containedBallObjects.insert(containedBallObjects.begin() + containedBallObjects.size(), leftTop->containedBallObjects.begin(), leftTop->containedBallObjects.end());
	//	containedBallObjects.insert(containedBallObjects.begin() + containedBallObjects.size(), rightBot->containedBallObjects.begin(), rightBot->containedBallObjects.end());
	//	containedBallObjects.insert(containedBallObjects.begin() + containedBallObjects.size(), rightTop->containedBallObjects.begin(), rightTop->containedBallObjects.end());
	//	leftBot.reset();
	//	rightBot.reset();
	//	leftTop.reset();
	//	rightTop.reset();
	//	return this;
	//}
	
	QuadTree* ptr = nullptr;
	if(leftBot)
	{
		if (ptr = leftBot->FindQuad(object))
			return ptr;
		if (ptr = leftTop->FindQuad(object))
			return ptr;
		if (ptr = rightBot->FindQuad(object))
			return ptr;
		if (ptr = rightTop->FindQuad(object))
			return ptr;
	}
	return ptr;

}

void QuadTree::Render(unsigned program)
{
	if(objManagerRef)
	{
		objManagerRef->CheckCollisionSection(containedBallObjects);
		if(leftBot)
		{
			objManagerRef->CheckCollisionSection(leftBot->containedBallObjects);
			objManagerRef->CheckCollisionSection(rightBot->containedBallObjects);
			objManagerRef->CheckCollisionSection(leftTop->containedBallObjects);
			objManagerRef->CheckCollisionSection(rightTop->containedBallObjects);
		}

	}
	glPolygonMode(GL_FRONT, GL_LINE);
	BoxObject::Render(program);
	if(leftBot)
	{
		leftBot->Render(program);
		leftTop->Render(program);
		rightBot->Render(program);
		rightTop->Render(program);
	}
}

void QuadTree::Init(ObjectManager& obj_manager)
{
	if (!objManagerRef)
		objManagerRef = &obj_manager;
	for(BallObject& obj : obj_manager.mBallObjects)
	{
		containedBallObjects.push_back(&obj);
	}
	for(BallObject* obj : containedBallObjects)
	{
		FindQuad(*obj);
	}
}
int numSubDivs = 0;
void QuadTree::SubDivide()
{
	if (numSubDivs > 3)
		return;
	leftBot = std::make_unique<QuadTree>(mTransform.pos, mTransform.pos + mTransform.size / 2.f);
	leftTop = std::make_unique<QuadTree>(glm::vec2(mTransform.pos.x, mTransform.pos.y + mTransform.size.y / 2.f), glm::vec2(mTransform.pos.x + mTransform.size.x / 2.f, mTransform.pos.y + mTransform.size.y));
	rightTop = std::make_unique<QuadTree>((mTransform.pos + mTransform.size /2.f), mTransform.pos + mTransform.size);
	rightBot = std::make_unique<QuadTree>(glm::vec2(mTransform.pos.x + mTransform.size.x / 2.f, mTransform.pos.y), glm::vec2(mTransform.pos.x + mTransform.size.x, mTransform.pos.y + mTransform.size.y/2.f));

	leftBot->parent = this;
	leftTop->parent = this;
	rightTop->parent = this;
	rightBot->parent = this;
	numSubDivs++;
}
