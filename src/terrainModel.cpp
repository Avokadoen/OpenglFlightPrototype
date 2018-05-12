#include "terrainModel.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>



Terrain::Terrain(float maxHeight, float blockScale) {
	this->maxHeight = maxHeight;
	this->blockScale = blockScale;
	yOffset = 0.0f;
	highestPoint = 0;
	yScale = 1.0f;
	lerpRange = 0.05f;
	seasonModifier = 0.0f;
	seasonDirection = 1;
	runThroughSeason = true;
}

void Terrain::update(float deltaTime) {
	if (runThroughSeason == true) {
		if (seasonModifier >= 1.0f) {
			seasonDirection = -1;
		}

		seasonModifier += (deltaTime / (float)SecondsForAYear) * seasonDirection;

		if (seasonModifier <= 0.00f) {
			seasonDirection = 1;
		}
	}
}

bool Terrain::generateHeightValues() {

	int index = 0;
	if (imageData) {

		// While imagedata is pointing on data
		// iterate through image
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				heightValues.push_back(GetHeightValue(imageData + (x + (imageWidth * y)) * nrComponents));
			}
		}
		return true;
	}
	else {
		return false;
	}
}

float Terrain::GetHeightValue(const unsigned char* data)
{
	
	switch (nrComponents)
	{
	case 1:
	{
		return (unsigned char)(data[0]) / (float)0xff;
	}
	break;
	case 2:
	{
		return (unsigned short)(data[1] << 8 | data[0]) / (float)0xffff;
	}
	break;
	case 3:
	{
		return (unsigned int)(data[2] << 16 | data[1] << 8 | data[0]) / (float)0xffffff;
	}
	break;
	case 4:
	{
		return (unsigned int)(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]) / (float)0xffffffff;
	}
	break;
	default:
	{
		assert(false);  // Height field with non standard pixel sizes
	}
	break;
	}

	return 0.0f;
	

}


bool Terrain::loadHeightMapData(const char *path)
{

	unsigned int textureID;
	glGenTextures(1, &textureID);

	imageData = stbi_load(path, &imageWidth, &imageHeight, &nrComponents, 0);


	if(!imageData)
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(imageData);
		return false;
	}
	if (!generateHeightValues()) {
		std::cout << "failed to generate height map" << std::endl;
		return false;
	}
	//imageData = data;
	for (auto point : heightValues) {
		if (highestPoint < point) {
			highestPoint = point;
		}
	}
	
	return true;
}

void Terrain::freeImageData() {
	stbi_image_free(imageData);
}

void Terrain::createTerrainMesh(float meshMaxHeight, float meshMinHeight, glm::vec3 primaryColor) {
	
	auto iter = heightValues.begin();
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {

			glm::vec3 data(0.0f);
			data.x = x * blockScale;
			data.y = maxHeight * (*iter++);
			data.z = y * blockScale;

			Vertex vboData;
			vboData.Position	= data;
			vboData.Normal		= glm::vec3(0.0f);
			vboData.TexCoords	= glm::vec2(0.0f);

			vertices.push_back(vboData);
		}
	}
	generateIndices();
	for (int i = 0; i < 6; i++) {
		generateNormals(i);
	}

	for (int i = 0; i < indices.size(); i++) {
		vertices[indices[i]].Normal = glm::normalize(vertices[indices[i]].Normal);
	}

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}

	Mesh mesh(vertices, indices);
	meshes.push_back(mesh);
}

void Terrain::generateIndices() {
	
	// 2 triangles for every quad of the terrain mesh
	const unsigned int numTriangles = (imageWidth - 1) * (imageHeight - 1) * 2;

	// 3 indices for each triangle in the terrain mesh

	unsigned int index = 0; // Index in the index buffer
	for (unsigned int y = 0; y < (imageHeight - 1); y++)
	{
		for (unsigned int x = 0; x < (imageWidth - 1); x++)
		{
			int vertexIndex = (y * imageWidth) + x;
			// Top triangle (T0)
			indices.push_back(vertexIndex);                           // V0
			indices.push_back(vertexIndex + imageWidth);		// V3
			indices.push_back(vertexIndex + 1);                      // V1
																	// Bottom triangle (T1)
			indices.push_back(vertexIndex + 1);                           // V0
			indices.push_back(vertexIndex + imageWidth + 1);            // V2
			indices.push_back(vertexIndex + imageWidth);        // V3
		}
	}

}

void Terrain::generateNormals(int offset) {
	for (unsigned int i = 0 + offset; i < indices.size() - 3; i += 3)
	{
		glm::vec3 v0 = vertices[indices[i + 0]].Position;
		glm::vec3 v1 = vertices[indices[i + 1]].Position;
		glm::vec3 v2 = vertices[indices[i + 2]].Position;

		glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);

		vertices[indices[i + 0]].Normal += normal;
		vertices[indices[i + 1]].Normal += normal;
		vertices[indices[i + 2]].Normal += normal;
	}
}

void Terrain::bindMaterialsToShader(Shader shader) {
	shader.setFloat("maxHeight", maxHeight);
	shader.setFloat("seasonModifier", seasonModifier);

	shader.setVec3("snow.ambient", snow.ambient);
	shader.setVec3("snow.diffuse", snow.diffuse);
	shader.setVec3("snow.specular", snow.specular);
	shader.setFloat("snow.shininess", snow.shininess);
	shader.setFloat("SummerSnowBottom", SummerSnowBottom );
	shader.setFloat("WinterSnowBottom", WinterSnowBottom);

	shader.setVec3("stone.ambient", stone.ambient);
	shader.setVec3("stone.diffuse", stone.diffuse);
	shader.setVec3("stone.specular", stone.specular);
	shader.setFloat("stone.shininess", stone.shininess);
	shader.setFloat("SummerStoneBottom", SummerStoneBottom);
	shader.setFloat("WinterStoneBottom", WinterStoneBottom);

	shader.setVec3("grass.ambient", grass.ambient);
	shader.setVec3("grass.diffuse", grass.diffuse);
	shader.setVec3("grass.specular", grass.specular);
	shader.setFloat("grass.shininess", grass.shininess);
	shader.setFloat("SummerGrassBottom", SummerGrassBottom);
	shader.setFloat("WinterGrassBottom", WinterGrassBottom);

	shader.setVec3("mud.ambient", mud.ambient);
	shader.setVec3("mud.diffuse", mud.diffuse);
	shader.setVec3("mud.specular", mud.specular);
	shader.setFloat("mud.shininess", mud.shininess);
	shader.setFloat("SummerMudBottom", SummerMudBottom);
	shader.setFloat("WinterMudBottom", WinterMudBottom);

	shader.setVec3("water.ambient", water.ambient);
	shader.setVec3("water.diffuse", water.diffuse);
	shader.setVec3("water.specular", water.specular);
	shader.setFloat("water.shininess", water.shininess);

}

void Terrain::setSeason(float seasonValue) {
	seasonModifier = seasonValue;
}

void Terrain::toggleRunThrough() {
	runThroughSeason = !runThroughSeason;
}