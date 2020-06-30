#include "MeshReaderObj.h"
#include "Vertex.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
//parseLine
//parseLineV
//parseLineVt
//parseLineVn
//parseLineVp
//parseLineF
//parseLineComment

struct FaceVertex
{
	unsigned int v_pos;
	unsigned int vt_uv;
	unsigned int vn_norm;

	bool operator==(const FaceVertex& other) const {
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
	{
		return nullptr;
	}
	
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;

	std::vector<FaceVertex> faceVertList;
	std::vector<Index> indexList;

	std::string line;
	while (std::getline(objFile,line)) {
		//split line into tokens
		//obj already deliminated by spaces
		std::istringstream ss(line);
		std::istream_iterator<std::string> start(ss);
		std::istream_iterator<std::string> end;

		std::vector<std::string> lineTokens(start, end);
		if (!lineTokens.size()) {
			continue;
		}
		std::string lineType = lineTokens.at(0);
		if (lineType == "v") {
			position.push_back(glm::vec3(
				std::stof(lineTokens.at(1)),
				std::stof(lineTokens.at(2)),
				std::stof(lineTokens.at(3))));

		}
		else if (lineType == "vt") {
			uv.push_back(glm::vec2(
				std::stof(lineTokens.at(1)),
				std::stof(lineTokens.at(2))));
		}
		else if (lineType == "vn") {
			normal.push_back(glm::vec3(
				std::stof(lineTokens.at(1)),
				std::stof(lineTokens.at(2)),
				std::stof(lineTokens.at(3))));
		}
		else if (lineType == "f") {

			//collect all face vertices used
			std::vector<FaceVertex> faceVertexLine;
			for (int i = 1; i < lineTokens.size(); i++) {
				auto currentToken = lineTokens.at(i);
				std::stringstream vs(currentToken);
				FaceVertex fv;
				std::string token;
				std::getline(vs, token, '/');
				fv.v_pos = std::stoi(token) - 1;
				std::getline(vs, token, '/');
				fv.vt_uv = std::stoi(token) - 1;
				std::getline(vs, token, ' ');
				fv.vn_norm = std::stoi(token) - 1;

				faceVertexLine.push_back(fv);
			}

			//add all faceVertices in groups of 3 to make each face.
			//if more than 3, use the previous 2 with the nth;
			for (int i = 2; i < faceVertexLine.size(); i++) {
				
				int fvIndex = std::find(faceVertList.begin(), faceVertList.end(), faceVertexLine.at(0)) - faceVertList.begin();
				indexList.push_back(fvIndex);
				if (fvIndex == faceVertList.end() - faceVertList.begin()) {
					faceVertList.push_back(faceVertexLine.at(0));
				}
				
				fvIndex = std::find(faceVertList.begin(), faceVertList.end(), faceVertexLine.at(i-1)) - faceVertList.begin();
				indexList.push_back(fvIndex);
				if (fvIndex == faceVertList.end() - faceVertList.begin()) {
					faceVertList.push_back(faceVertexLine.at(i-1));
				}

				fvIndex = std::find(faceVertList.begin(), faceVertList.end(), faceVertexLine.at(i)) - faceVertList.begin();
				indexList.push_back(fvIndex);
				if (fvIndex == faceVertList.end() - faceVertList.begin()) {
					faceVertList.push_back(faceVertexLine.at(i));
				}

				
				for (int j = i; j < i + 3; j++) {
					
				}
			}
		}
		else {
			continue;
		}
	}

	unsigned int vertexCount = faceVertList.size();
	auto vertexData = std::make_unique<Vertex[]>(vertexCount);

	for (unsigned int i = 0; i < vertexCount; i++) {
		vertexData[i].position = position.at(faceVertList.at(i).v_pos);
		vertexData[i].normal = normal.at(faceVertList.at(i).vn_norm);
		vertexData[i].uv = uv.at(faceVertList.at(i).vt_uv);
	}
	unsigned int indexCount = indexList.size();

	auto indexData = std::make_unique<Index[]>(indexCount);
	for (unsigned int i = 0; i < indexCount; i++) {
		indexData[i] = indexList.at(i);
	}
	auto mesh = std::make_unique<Mesh>(std::move(vertexData), vertexCount, std::move(indexData), indexCount);
	return mesh;

}
