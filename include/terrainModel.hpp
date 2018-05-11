#pragma once;

#include <vector>
#include "model.hpp"
#include "stb_image.h"

class Terrain : Model
{
public:

	Terrain(float heightScale = 500.0f, float blockScale = 2.0f);

	bool loadHeightMap(const char *path);
	void freeImageData();
	bool generateHeightValue();

	int nrComponents;
	int imageWidth;
	unsigned int fileSize;
	unsigned char* imageData;
	float GetHeightValue(const unsigned char* data);

private:
	std::vector<float> heightValues;

};