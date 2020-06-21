#pragma once
#include "MeshReader.h"
class MeshReaderObj :
	public MeshReader
{
public:
	std::unique_ptr<Mesh> loadMesh(std::string fileName) override;


};

