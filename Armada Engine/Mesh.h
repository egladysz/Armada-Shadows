#pragma once
#include "Terminology.h"
#include "Vertex.h"
#include "Shader.h"
#include <utility>
class Mesh
{
public:
	Mesh(std::unique_ptr <Vertex[]> vertList, int vertCount, std::unique_ptr<Index[]> indList, int indCount);

	~Mesh();
	void Draw(Shader shader) const;
private:
	void primeMesh();
	
	std::unique_ptr<Vertex[]> vertexList;
	std::unique_ptr<Index[]> indexList;

	GLID vertexArray;
	GLID vertexBuffer;
	GLID indexBuffer;
	
	int vertexCount;
	int indexCount;
	
};

