#include "Object.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void BufferData::EmplaceSquareIndices(int index1, int index2, int index3, int index4)
{
	indices.emplace_back(index1);
	indices.emplace_back(index2);
	indices.emplace_back(index3);
	indices.emplace_back(index3);
	indices.emplace_back(index4);
	indices.emplace_back(index1);
}

void BufferData::BindBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned)), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
}

void Object::Render(unsigned program)
{
	glBindVertexArray(mBufferData.VAO);
	glDrawElements(GL_TRIANGLES, mBufferData.indices.size(), GL_UNSIGNED_INT, nullptr);
}

Object::Object(glm::vec2 pos, float radius, glm::vec2 size) : mTransform(glm::vec3(pos,0.f),radius,glm::vec3(size,0.f))
{

}

Object::Object(glm::vec3 pos, float radius, glm::vec3 size) : mTransform(pos,radius,size)
{

}
