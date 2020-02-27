#pragma once
#include "Terminology.h"
#include "Vertex.h"
#include "Shader.h"
class Mesh
{
public:
	Mesh(Vertex* vertList, int vertCount, Index* indList, int indCount);
	~Mesh();
	void Draw(Shader shader) const;
private:
	void primeMesh();
	
	Vertex* vertexList;
	Index* indexList;

	GLID vertexArray;
	GLID vertexBuffer;
	GLID indexBuffer;
	
	int vertexCount;
	int indexCount;
	
};

