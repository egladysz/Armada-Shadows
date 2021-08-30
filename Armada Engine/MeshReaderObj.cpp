#include "MeshReaderObj.h"
#include "Vertex.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>


struct UnparsedVertex
{
	unsigned int v_pos;
	unsigned int vt_uv;
	unsigned int vn_norm;

	bool operator==(const UnparsedVertex& other) const {
		return 
			v_pos == other.v_pos && 
			vt_uv == other.vt_uv && 
			vn_norm == other.vn_norm;
	}
};


std::unique_ptr<Mesh> MeshReaderObj::loadMesh(std::string fileName)
{
	std::ifstream objFile;
	objFile.open(fileName);

	if (!objFile.is_open())
		return nullptr;
	
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;

	std::vector<UnparsedVertex> unparsedVertices;
	std::vector<Index> indexList;

	std::string line;
	while (std::getline(objFile,line)) {
		//split line into tokens
		//obj already deliminated by spaces
		std::istringstream ss(line);
		std::istream_iterator<std::string> start(ss);
		std::istream_iterator<std::string> end;

		std::vector<std::string> lineTokens(start, end);
		if (!lineTokens.size())
			continue;

		std::string lineType = lineTokens.at(0);
		if (lineType == "v") 
			position.push_back(glm::vec3(	std::stof(lineTokens.at(1)),
											std::stof(lineTokens.at(2)),
											std::stof(lineTokens.at(3))));
		else if (lineType == "vt")
			uv.push_back(glm::vec2(	std::stof(lineTokens.at(1)),
									std::stof(lineTokens.at(2))));
		else if (lineType == "vn")
			normal.push_back(glm::vec3( std::stof(lineTokens.at(1)),
										std::stof(lineTokens.at(2)),
										std::stof(lineTokens.at(3))));
		else if (lineType == "f") {

			auto extractVertexFromLine = [](std::string token) {
				std::stringstream vs(token);
				
				std::string positionToken;
				std::string uvToken;
				std::string normalToken;
				std::getline(vs, positionToken, '/');
				std::getline(vs, uvToken, '/');
				std::getline(vs, normalToken, ' ');

				UnparsedVertex vertex;
				vertex.v_pos = std::stoi(positionToken) - 1;
				vertex.vt_uv = std::stoi(uvToken) - 1;
				vertex.vn_norm = std::stoi(normalToken) - 1;

				return vertex;
			};
			std::vector<UnparsedVertex> lineVertices(lineTokens.size() - 1);
			std::transform(lineTokens.begin() + 1, lineTokens.end(), lineVertices.begin(),extractVertexFromLine);
			

			auto& firstVertex = lineVertices.at(0);
			auto& secondVertex = lineVertices.at(1);

			auto firstIndex = std::find(unparsedVertices.begin(), unparsedVertices.end(), firstVertex) - unparsedVertices.begin();
			if (firstIndex == unparsedVertices.end() - unparsedVertices.begin())
				unparsedVertices.push_back(firstVertex);
			auto previousIndex = std::find(unparsedVertices.begin(), unparsedVertices.end(), secondVertex) - unparsedVertices.begin();
			if (previousIndex == unparsedVertices.end() - unparsedVertices.begin())
				unparsedVertices.push_back(secondVertex);

			for (int i = 2; i < lineVertices.size(); i++) {
				indexList.push_back(firstIndex);
				indexList.push_back(previousIndex);

				auto& currentVertex = lineVertices.at(i);
				auto currentIndex = std::find(unparsedVertices.begin(), unparsedVertices.end(), currentVertex) - unparsedVertices.begin();

				if (currentIndex == unparsedVertices.end() - unparsedVertices.begin())
					unparsedVertices.push_back(currentVertex);
				indexList.push_back(currentIndex);

				previousIndex = currentIndex;
			}
		}
		else
			continue;
	}

	size_t vertexCount = unparsedVertices.size();
	auto vertexData = std::make_unique<Vertex[]>(vertexCount);

	for (size_t i = 0; i < vertexCount; i++) {
		vertexData[i].position = position.at(unparsedVertices.at(i).v_pos);
		vertexData[i].normal = normal.at(unparsedVertices.at(i).vn_norm);
		vertexData[i].uv = uv.at(unparsedVertices.at(i).vt_uv);
	}

	size_t indexCount = indexList.size();
	auto indexData = std::make_unique<Index[]>(indexCount);
	for (size_t i = 0; i < indexCount; i++)
		indexData[i] = indexList.at(i);

	auto mesh = std::make_unique<Mesh>(std::move(vertexData), vertexCount, std::move(indexData), indexCount);
	return mesh;

}
