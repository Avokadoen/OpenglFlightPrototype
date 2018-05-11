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
		glm::vec3(0.6, 0.6, 0.67),
		glm::vec3(0.9, 0.9, 1.0),
		glm::vec3(0.5, 0.5, 0.5),
		32.0f
	};
	const float SnowStart = 0.85f;
	const float SnowEnd = 1.0f;

	const Material stone = {
		glm::vec3(0.2, 0.2, 0.2),
		glm::vec3(0.4, 0.4, 0.4),
		glm::vec3(0.05, 0.05, 0.05),
		16.0f
	};
	const float StoneStart = 0.5f;
	const float StoneEnd = 0.85f;

	const Material grass = {
		glm::vec3(0.1, 0.3, 0.1),
		glm::vec3(0.2, 0.6, 0.2),
		glm::vec3(0.05, 0.05, 0.05),
		16.0f
	};
	const float GrassStart = 0.1f;
	const float GrassEnd = 0.5f;


	const Material mud = {
		glm::vec3(0.4, 0.2, 0.05),
		glm::vec3(0.8, 0.4, 0.1),
		glm::vec3(0.2, 0.2, 0.2),
		8.0f
	};
	const float MudStart = 0.05f;
	const float MudEnd = 0.1f;

	const Material water = {
		glm::vec3(0.1, 0.1, 0.3),
		glm::vec3(0.2, 0.2, 0.6),
		glm::vec3(0.5, 0.5, 0.5),
		64.0f
	};
	const float WaterEnd = 0.05f;

private:
	std::vector<unsigned int> generateIndices();
	std::vector<glm::vec3>	generateNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	std::vector<float>	heightValues;

	float yOffset;
	float yScale;
	float maxHeight;
	float blockScale;
	unsigned int heightoffset;
};