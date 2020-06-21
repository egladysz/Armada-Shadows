#include "Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>



Mesh::Mesh(std::unique_ptr<Vertex[]> vertList, int vertCount, std::unique_ptr<Index[]> indList, int indCount)
	:vertexList{ std::move(vertList) }, vertexCount{ vertCount },
	indexList{ std::move(indList) }, indexCount{ indCount }
{
	primeMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader shader) const
{
	glBindVertexArray(vertexArray);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::primeMesh()
{
	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(vertexArray);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount* sizeof(Vertex), vertexList.get(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount* sizeof(Index), indexList.get(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);

}
