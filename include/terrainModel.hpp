#pragma once;

#include <vector>
#include "model.hpp"
#include "stb_image.h"

class Terrain : public Model
{
public:

	Terrain(float maxHeight = 500.0f, float blockScale = 2.0f);

	int nrComponents;
	int imageWidth;
	int imageHeight;

	unsigned int fileSize;
	unsigned char* imageData;

	float GetHeightValue(const unsigned char* data);
	bool loadHeightMapData(const char *path);
	void freeImageData();
	bool generateHeightValues();
	void createTerrainMesh(float maxHeight, float minHeight, glm::vec3 primaryColor);
private:
	std::vector<unsigned int> generateIndices();
	std::vector<glm::vec3>	generateNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	std::vector<float>	heightValues;

	unsigned int maxHeight;
	float blockScale;
	unsigned int heightoffset;

};