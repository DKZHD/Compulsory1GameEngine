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
	if (object.GetPosition().x - object.GetRadius() < mTransform.pos.x && object.GetPosition().x + object.GetRadius() > mTransform.pos.x ||
		object.GetPosition().x - object.GetRadius() < mTransform.pos.x + mTransform.size.x && object.GetPosition().x + object.GetRadius() > mTransform.pos.x + mTransform.size.x ||
		object.GetPosition().y - object.GetRadius() < mTransform.pos.y && object.GetPosition().y + object.GetRadius() > mTransform.pos.y ||
		object.GetPosition().y - object.GetRadius() < mTransform.pos.y + mTransform.size.y && object.GetPosition().y + object.GetRadius() > mTransform.pos.y + mTransform.size.y)
	{

	}
		
	if (object.GetPosition().x > mTransform.pos.x + mTransform.size.x || object.GetPosition().x<mTransform.pos.x || object.GetPosition().y > mTransform.pos.y + mTransform.size.y || object.GetPosition().y < mTransform.pos.y)
	{
		std::erase(containedBallObjects, &object);
		if (parent)
			std::erase(parent->containedBallObjects, &object);
		return nullptr;
	}
	if(!leftBot)
	{
		if (std::ranges::find(containedBallObjects.begin(), containedBallObjects.end(), &object) == containedBallObjects.end())
			containedBallObjects.push_back(&object);
		return this;
	}
	QuadTree* ptr = nullptr;
	if (ptr = leftBot->FindQuad(object))
		return ptr;
	if(rightBot->FindQuad(object))
		return ptr;
	if(leftTop->FindQuad(object))
		return ptr;
	if(rightTop->FindQuad(object))
		return ptr;
	return nullptr;
}

void QuadTree::Render(unsigned program)
{
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

void QuadTree::Init(ObjectManager& obj_manager, bool subDivide)
{
	if (!objManagerRef)
		objManagerRef = &obj_manager;
	for(BallObject& obj : obj_manager.mBallObjects)
	{
		containedBallObjects.push_back(&obj);
	}
	if(subDivide)
	{
		SubDivide();
		leftBot->SubDivide();
		rightBot->SubDivide();
		leftTop->SubDivide();
		rightTop->SubDivide();
	}
	for(BallObject* obj : containedBallObjects)
	{
		FindQuad(*obj);
	}
}

bool QuadTree::CheckAllCollisions(QuadTree* q_t)
{
	objManagerRef->CheckCollisionSection(q_t->containedBallObjects);
	if(q_t->leftBot)
	{
		CheckAllCollisions(q_t->leftBot.get());
		CheckAllCollisions(q_t->rightBot.get());
		CheckAllCollisions(q_t->leftTop.get());
		CheckAllCollisions(q_t->rightTop.get());
	}
	return false;
}


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

void QuadTree::UnSubDivide()
{
	if (parent == nullptr)
		return;
	leftBot.reset();
	rightBot.reset();
	rightTop.reset();
	leftTop.reset();
	numSubDivs--;
}
