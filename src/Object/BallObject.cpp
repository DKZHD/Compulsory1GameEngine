#include "BallObject.h"
#include "glad/glad.h"
#include "glm/trigonometric.hpp"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Source.h"
#include "../ObjectManager/ObjectManager.h"
#include "../Physics/Physics.h"
#include "glm/geometric.hpp"

BallObject::BallObject(glm::vec2 pos, float radius, float resolution, glm::vec3 color) : Object(pos,radius)
{
	Create2D(pos,radius,resolution,color);
}

BallObject::BallObject(glm::vec3 pos, float radius, int numSubDivs, glm::vec3 color) : Object(pos,radius)
{
	Create3D(pos, radius, numSubDivs, color);
}

void BallObject::SetPosition(glm::vec2 newPos)
{
	mPhysics.pos = newPos;
}

void BallObject::UpdatePosition(glm::vec2 pos)
{
	mPhysics.pos += pos;
}

void BallObject::Update(float deltaTime)
{
	prevPos = GetPosition();
	mPhysics.vel = glm::clamp(mPhysics.vel, -glm::vec2(maxSpeed), glm::vec2(maxSpeed));
	if (std::abs(mVelocity.x) < 0.0001f)
		mVelocity.x = 0.f;
	if (std::abs(mVelocity.y) < 0.0001f)
		mVelocity.y = 0.f;
	mPhysics.pos += mPhysics.vel * deltaTime;
}

void BallObject::UpdateVelocity(glm::vec2 vel)
{
	mPhysics.vel = glm::min(mPhysics.vel += vel, glm::vec2(maxSpeed));
}

void BallObject::SetVelocity(glm::vec2 newVel)
{
	mPhysics.vel = glm::min(newVel, glm::vec2(maxSpeed));
}

glm::vec2& BallObject::GetVelocity()
{
	return mPhysics.vel;
}

float BallObject::GetRadius()
{
	return mTransform.radius;
}

glm::vec2& BallObject::GetPosition()
{
	return mPhysics.pos;
}

void BallObject::Render(unsigned program)
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(mPhysics.pos,0.f));
	model = glm::scale(model, glm::vec3((mTransform.radius)));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	Object::Render(program);
}

bool BallObject::checkCollision(BallObject& object)
{
	float dx = object.GetPosition().x - GetPosition().x;
	float dy = object.GetPosition().y - GetPosition().y;
	float l = sqrtf(dx * dx + dy * dy);
	float d = GetRadius() + object.GetRadius();
	if (l >= d)
		return false;

	float detM = mass + object.mass;
	float numM1 = 2 * mass;
	float numM2 = 2 * object.mass;

	float v1Num = glm::dot(GetVelocity()-object.GetVelocity(),GetPosition()-object.GetPosition());
	float v2Num = glm::dot(object.GetVelocity()-GetVelocity(), object.GetPosition() - GetPosition());

	float detV1 = glm::distance(GetPosition(), object.GetPosition());
	float detV2 = glm::distance(object.GetPosition(), GetPosition());

	glm::vec2 v1 = GetVelocity() - (numM2 / detM) * (v1Num / (detV1*detV1))*(GetPosition()-object.GetPosition());
	glm::vec2 v2 = object.GetVelocity() - (numM1 / detM) * (v2Num / (detV2*detV2))*(object.GetPosition()-GetPosition());

	SetVelocity(v1);
	object.SetVelocity(v2);

	glm::vec2 collisionV1 = glm::normalize(object.GetPosition() - GetPosition());

	SetPosition(GetPosition() + collisionV1 * ((l-d)/2.f));
	object.SetPosition(object.GetPosition() - collisionV1 * ((l - d) / 2.f));


	return true;
}

void BallObject::Create2D(glm::vec2 pos, float radius, float resolution, glm::vec3 color)
{
	assert(Core::GetGameView() == GameView::Two_Dimensional && "Not using 2D view. Switch game view to 2D to resolve issue");
	this->color = color;
	mBufferData.vertices.emplace_back(glm::vec2(0.f), color);

	for (int i = 0; i < static_cast<int>(resolution); ++i)
	{
		float x = std::cos(static_cast<float>(i) / resolution * 2.f * glm::pi<float>());
		float y = std::sin(static_cast<float>(i) / resolution * 2.f * glm::pi<float>());
		mBufferData.vertices.emplace_back(glm::normalize(glm::vec2(x, y)), color);
	}

	for (int i = 1; i < mBufferData.vertices.size(); ++i)
	{
		mBufferData.indices.emplace_back(0);
		mBufferData.indices.emplace_back(i + 1);
		mBufferData.indices.emplace_back(i);
	}
	mBufferData.indices.emplace_back(0);
	mBufferData.indices.emplace_back(mBufferData.vertices.size() - 1);
	mBufferData.indices.emplace_back(1);

	mBufferData.BindBuffers();
}

void BallObject::subDivide(int index1, int index2, int index3, int i)
{
	if(i > 0) {
		glm::vec3 pos1 = glm::normalize(mBufferData.vertices[index1].pos + mBufferData.vertices[index2].pos);
		int t_index1 = mBufferData.vertices.size();
		mBufferData.vertices.emplace_back(pos1, color, pos1);

		glm::vec3 pos2 = glm::normalize(mBufferData.vertices[index1].pos + mBufferData.vertices[index3].pos);
		int t_index2 = mBufferData.vertices.size();
		mBufferData.vertices.emplace_back(pos2, color, pos2);

		glm::vec3 pos3 = glm::normalize(mBufferData.vertices[index3].pos + mBufferData.vertices[index2].pos);
		int t_index3 = mBufferData.vertices.size();
		mBufferData.vertices.emplace_back(pos3, color, pos3);

		subDivide(index1, t_index1, t_index2, i - 1);
		subDivide(index3, t_index2, t_index3, i - 1);
		subDivide(index2, t_index3, t_index1, i - 1);
		subDivide(t_index3, t_index2, t_index1, i - 1);
	} else{
		mBufferData.indices.emplace_back(index1);
		mBufferData.indices.emplace_back(index2);
		mBufferData.indices.emplace_back(index3);
	}
}


void BallObject::Create3D(glm::vec3 pos, float radius, int subdivs, glm::vec3 color)
{
	assert(Core::GetGameView() == GameView::Three_Dimensional && "Not using 3D view. Switch game view to 3D to resolve issue");
	this->color = color;

	mBufferData.vertices.emplace_back(glm::vec3(0.f, 0.f, 1.f), color, glm::vec3(0.f, 0.f, 1.f));
	mBufferData.vertices.emplace_back(glm::vec3(1.f, 0.f, 0.f), color, glm::vec3(1.f, 0.f, 0.f));
	mBufferData.vertices.emplace_back(glm::vec3(0.f, 1.f, 0.f), color, glm::vec3(0.f, 1.f, 0.f));
	mBufferData.vertices.emplace_back(glm::vec3(-1.f, 0.f, 0.f), color, glm::vec3(-1.f, 0.f, 0.f));
	mBufferData.vertices.emplace_back(glm::vec3(0.f, -1.f, 0.f), color, glm::vec3(0.f, -1.f, 0.f));
	mBufferData.vertices.emplace_back(glm::vec3(0.f, 0.f, -1.f), color, glm::vec3(0.f, 0.f, -1.f));

	subDivide(0, 1, 2, subdivs);
	subDivide(0, 2, 3, subdivs);
	subDivide(0, 3, 4, subdivs);
	subDivide(0, 4, 1, subdivs);
	subDivide(5, 2, 1, subdivs);
	subDivide(5, 3, 2, subdivs);
	subDivide(5, 4, 3, subdivs);
	subDivide(5, 1, 4, subdivs);

	mBufferData.BindBuffers();
}
