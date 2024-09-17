#include "BoxObject.h"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "BallObject.h"

BoxObject::BoxObject(glm::vec2 pos, glm::vec3 color, glm::vec2 size) : Object(pos,0.f,size)
{
	CreateBox2D(pos,color,size);
}

BoxObject::BoxObject(glm::vec3 pos, glm::vec3 color, glm::vec3 size) : Object(pos,0.f,size)
{
	CreateBox3D(pos, color, size);
}

void BoxObject::Render(unsigned program)
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(mTransform.pos));
	model = glm::scale(model, glm::vec3(mTransform.size));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	Object::Render(program);
}

bool BoxObject::checkCollision(glm::vec2& pos, float radius, glm::vec2& vel, glm::vec2 prevPos)
{
	if (pos.y - radius > mTransform.pos.y + mTransform.size.y || pos.x + radius < mTransform.pos.x || pos.x - radius > mTransform.pos.x + mTransform.size.x || pos.y + radius < mTransform.pos.y/* || pos.z - radius > mTransform.pos.z || pos.z + radius  < mTransform.pos.z - mTransform.size.z*/)
		return false;

	glm::vec2 dir = glm::normalize(pos - prevPos);
	float length = glm::length(pos-prevPos);

	if(prevPos.x + radius < mTransform.pos.x && vel.x > 0.f || prevPos.x - radius > mTransform.pos.x + mTransform.size.x && vel.x < 0.f)
	{
		pos.x = prevPos.x;
		vel.x *= -1.f;
	}
	if(prevPos.y - radius > mTransform.pos.y + mTransform.size.y && vel.y < 0.f || prevPos.y + radius < mTransform.pos.y && vel.y > 0.f)
	{
		pos.y = prevPos.y;
		vel.y *= -1.f;
		
	}
	//if(prevPos.z - radius > mTransform.pos.z && vel.z < 0.f || pos.z + radius  < mTransform.pos.z - mTransform.size.z && vel.z > 0.f)
	//{
	//	pos.z = prevPos.z;
	//	vel.z *= -1.f;
	//}

	return true;
}

void BoxObject::CreateBox2D(glm::vec2 pos, glm::vec3 color, glm::vec2 size)
{
	this->color = color;
	mBufferData.vertices.emplace_back(glm::vec2(0.f), color);
	mBufferData.vertices.emplace_back(glm::vec2(1.f, 0.f), color);
	mBufferData.vertices.emplace_back(glm::vec2(1.f), color);
	mBufferData.vertices.emplace_back(glm::vec2(0.f, 1.f), color);

	mBufferData.EmplaceSquareIndices(0,1,2,3);

	mBufferData.BindBuffers();
}

void BoxObject::CreateBox3D(glm::vec3 pos, glm::vec3 color, glm::vec3 size)
{
	this->color = color;
	mBufferData.vertices.emplace_back(glm::vec3(0.f), color);
	mBufferData.vertices.emplace_back(glm::vec3(1.f, 0.f,0.f), color);
	mBufferData.vertices.emplace_back(glm::vec3(1.f,1.f,0.f), color);
	mBufferData.vertices.emplace_back(glm::vec3(0.f,1.f,0.f), color);

	mBufferData.vertices.emplace_back(glm::vec3(0.f,0.f,-1.f), color);
	mBufferData.vertices.emplace_back(glm::vec3(1.f,0.f,-1.f), color);
	mBufferData.vertices.emplace_back(glm::vec3(1.f,1.f,-1.f), color);
	mBufferData.vertices.emplace_back(glm::vec3(0.f,1.f,-1.f), color);

	mBufferData.EmplaceSquareIndices(0, 1, 2, 3);
	mBufferData.EmplaceSquareIndices(0, 4, 5, 1);
	mBufferData.EmplaceSquareIndices(0, 3, 7, 4);
	mBufferData.EmplaceSquareIndices(6, 5, 4, 7);
	mBufferData.EmplaceSquareIndices(6, 2, 1, 5);
	mBufferData.EmplaceSquareIndices(6, 7, 3, 2);

	mBufferData.BindBuffers();
}
