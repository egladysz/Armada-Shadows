#pragma once
#include <string>
#include <memory>
#include "Mesh.h"

class MeshReader{
	virtual std::unique_ptr<Mesh> loadMesh(std::string fileName) = 0;
};