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
	void bindMaterialsToShader(Shader shader);

	virtual void translate(glm::vec3 offset);
	virtual void scale(glm::vec3 scale);
	virtual void scale(float scale);

protected:

	const Material snow = {
		glm::vec3(0.7, 0.7, 0.7),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(0.5, 0.5, 0.5),
		32.0f
	};
	const float SnowBottom = 0.80f;
	const float SnowTop = 1.0f;

	const Material stone = {
		glm::vec3(0.2, 0.2, 0.2),
		glm::vec3(0.4, 0.4, 0.4),
		glm::vec3(0.05, 0.05, 0.05),
		16.0f
	};
	const float StoneBottom = 0.5f;
	const float StoneTop = 0.85f;

	const Material grass = {
		glm::vec3(0.1, 0.3, 0.1),
		glm::vec3(0.2, 0.6, 0.2),
		glm::vec3(0.05, 0.05, 0.05),
		16.0f
	};
	const float GrassBottom = 0.1f;
	const float GrassTop = 0.45f;


	const Material mud = {
		glm::vec3(0.4, 0.2, 0.05),
		glm::vec3(0.8, 0.4, 0.1),
		glm::vec3(0.2, 0.2, 0.2),
		8.0f
	};
	const float MudBottom = 0.05f;
	const float MudTop = 0.2f;

	const Material water = {
		glm::vec3(0.1, 0.1, 0.3),
		glm::vec3(0.2, 0.2, 0.6),
		glm::vec3(0.5, 0.5, 0.5),
		64.0f
	};
	const float WaterTop = 0.05f;

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
	unsigned int heightoffset;
};