#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

struct Transform3D
{
	glm::vec3 pos;
	float radius = 0.f;
	glm::vec3 size = glm::vec3(0.f);
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	Vertex(glm::vec2 pos, glm::vec3 color) : pos(glm::vec3(pos,0.f)), color(color), normal(1.f) {}
	Vertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal = glm::vec3(1.f)) : pos(pos), color(color),normal(normal) {}
};

struct BufferData
{
	unsigned VAO = 0, VBO = 0, EBO = 0;
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	void EmplaceSquareIndices(int index1, int index2, int index3, int index4);
	void BindBuffers();
};

class Object
{
public:
	virtual void Render(unsigned program);
	virtual ~Object() = default;
protected:
	BufferData mBufferData;
	Transform3D mTransform;
	glm::vec3 color = glm::vec3(0.f);
	Object(glm::vec2 pos, float radius = 0.f, glm::vec2 size = glm::vec2(0.f));
	Object(glm::vec3 pos, float radius = 0.f, glm::vec3 size = glm::vec3(0.f));
};
