#pragma once;

#include <vector>
#include "model.hpp"
#include "stb_image.h"

class Terrain : public Model
{
public:

	Terrain(float maxHeight = 500.0f, float blockScale = 2.0f);
	void update(float deltaTime);
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
	void bindMaterialsToShader(Shader shader);
	void setSeason(float seasonValue);

protected:
	const int SecondsForAYear = 200;

	const Material snow = {
		glm::vec3(0.7, 0.7, 0.7),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(0.8, 0.8, 0.8),
		32.0f
	};
	const float SummerSnowBottom = 0.50f;
	const float WinterSnowBottom = 0.1f;

	const Material stone = {
		glm::vec3(0.2, 0.2, 0.2),
		glm::vec3(0.4, 0.4, 0.4),
		glm::vec3(0.05, 0.05, 0.05),
		16.0f
	};
	const float SummerStoneBottom = 2.0f;
	const float WinterStoneBottom = 2.0f;

	const Material mud = {
		glm::vec3(0.4, 0.2, 0.05),
		glm::vec3(0.8, 0.4, 0.1),
		glm::vec3(0.2, 0.2, 0.2),
		8.0f
	};
	const float SummerMudBottom = 0.4f;
	const float WinterMudBottom = 0.9;

	const Material grass = {
		glm::vec3(0.1, 0.3, 0.1),
		glm::vec3(0.2, 0.6, 0.2),
		glm::vec3(0.05, 0.05, 0.05),
		16.0f
	};
	const float SummerGrassBottom = 0.15f;
	const float WinterGrassBottom = 0.08f;


	const Material water = {
		glm::vec3(0.1, 0.1, 0.3),
		glm::vec3(0.2, 0.2, 0.6),
		glm::vec3(0.9, 0.9, 0.9),
		64.0f
	};
	const float SummerWaterBottom = 0.00f;
	const float WinterWaterBottom = 0.00f;


	
private:
	void generateIndices();
	void generateNormals(int offset);
	std::vector<float>	heightValues;

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;

	float highestPoint;
	float yOffset;
	float yScale;
	float maxHeight;
	float blockScale;
	float lerpRange;
	float seasonModifier;
};