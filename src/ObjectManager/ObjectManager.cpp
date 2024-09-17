#include "ObjectManager.h"
#include <ctime>
#include "../Object/BallObject.h"
#include "../Object/BoxObject.h"
#include "QuadTree.h"

void ObjectManager::AddBall(glm::vec2 pos, float radius, float resolution, glm::vec3 color)
{
	mBallObjects.emplace_back(pos,radius,resolution,color);
}

void ObjectManager::Add3DBall(glm::vec3 pos, float radius, int numSubDivs, glm::vec3 color)
{
	mBallObjects.emplace_back(pos,radius,numSubDivs,color);
}

void ObjectManager::Add3DBox(glm::vec3 pos, glm::vec3 color, glm::vec3 size)
{
	mBoxObjects.emplace_back(pos, color, size);
}

void ObjectManager::AddXAmountofBalls(int amount, float resolution, glm::vec3 color)
{
	std::srand(time(nullptr));
	int y = 0;
	int counter = 0;
	for(int i = 0; counter<amount;i++)
	{
		mBallObjects.emplace_back(glm::vec2(-1.6+i*0.06f,-0.7f + y*0.06f), 0.02f, resolution, color);
		mBallObjects.back().mass = 0.05f;
		if (i % 50 == 0)
		{
			y++;
			i = 0;
		}
			
		counter++;
	}
}

void ObjectManager::AddBox(glm::vec2 pos, glm::vec3 color, glm::vec2 size)
{
	mBoxObjects.emplace_back(pos,color,size);
}

void ObjectManager::Render(unsigned program)
{
	for(auto& element : mBoxObjects)
	{
		element.Render(program);
	}
	for (auto& element : mBallObjects)
	{
		element.Render(program);
	}
}

void ObjectManager::Update(float deltaTime, QuadTree& quad_tree)
{
	for (auto& element : mBallObjects)
	{
		element.Update(deltaTime);
		quad_tree.FindQuad(element);
	}
}

void ObjectManager::CheckCollisions()
{
	for (int i = 0; i < mBoxObjects.size(); i++)
	{
		for (int j = 0; j < mBallObjects.size(); ++j)
		{
			mBoxObjects[i].checkCollision(mBallObjects[j].GetPosition(), mBallObjects[j].GetRadius(), mBallObjects[j].GetVelocity(),mBallObjects[j].prevPos);
		}
	}
	//for (int i = 0; i < mBallObjects.size(); i++)
	//{
	//	for (int j = i + 1; j < mBallObjects.size(); j++)
	//	{
	//		if (mBallObjects[i].checkCollision(mBallObjects[j]))
	//		{
	//			break;
	//		}
	//	}
	//}
}

void ObjectManager::CheckCollisionSection(std::vector<BallObject*> ball_objects)
{
	for (int i = 0; i < ball_objects.size(); i++)
	{
		for (int j = i + 1; j < ball_objects.size(); j++)
		{
			if (ball_objects[i]->checkCollision(*ball_objects[j]))
			{
				break;
			}
		}
	}
}

BallObject* ObjectManager::GetBallObjectAtIndex(int index)
{
	if (mBallObjects.size() - 1 >= index)
		return &mBallObjects[index];
	return nullptr;
}
