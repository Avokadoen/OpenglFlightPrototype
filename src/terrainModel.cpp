#include "terrainModel.hpp"

#include <iostream>
#include <fstream>

inline float GetPercentage(float value, const float min, const float max)
{
	value = glm::clamp(value, min, max);
	return (value - min) / (max - min);
}

inline int GetFileLength(std::istream& file)
{
	int pos = file.tellg();
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	// Restore the position of the get pointer
	file.seekg(pos);

	return length;
}

Terrain::Terrain(float maxHeight, float blockScale) {
	this->maxHeight = maxHeight;
	this->blockScale = blockScale;
}

bool Terrain::generateHeightValues() {

	int index = 0;
	if (imageData) {

		// While imagedata is pointing on data
		// iterate through image
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				heightValues.push_back(GetHeightValue(imageData +(x + imageWidth * y)));
			}
		}

		while ((index * nrComponents) < fileSize) {
			
			index++;
		}

		if (heightValues.size() == fileSize) {
			return true;
		}else return false;
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


	if (imageData)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 2)
			format = GL_RG;
		//else if (nrComponents == 3)
			//format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(imageData);
		return false;
	}
	generateHeightValues();
	//imageData = data;
	return true;
}

void Terrain::freeImageData() {
	stbi_image_free(imageData);
}

void Terrain::createTerrainMesh(float maxHeight, float minHeight, glm::vec3 primaryColor) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {

			glm::vec3 data(0);
			data.x = j * blockScale;
			data.y = maxHeight * heightValues[(i*j)+j];
			data.z = i * blockScale;

			Vertex vboData;
			vboData.Position = data;
			vboData.Normal = glm::vec3(0);
			vboData.TexCoords = glm::vec2(0);

			vertices.push_back(vboData);
		}
	}


	indices = generateIndices();

	std::vector<Texture> t;
	Material mat;
	Mesh mesh(vertices, indices, t, mat);
	meshes.push_back(mesh);
}

std::vector<unsigned int> Terrain::generateIndices() {
	/*if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2)
	{
		// Terrain hasn't been loaded, or is of an incorrect size
		return;
	}*/

	// 2 triangles for every quad of the terrain mesh
	const unsigned int numTriangles = (imageWidth - 1) * (imageHeight - 1) * 2;

	// 3 indices for each triangle in the terrain mesh
	std::vector<unsigned int> indices;
	indices.resize(numTriangles * 3);

	unsigned int index = 0; // Index in the index buffer
	for (unsigned int j = 0; j < (imageWidth - 1); ++j)
	{
		for (unsigned int i = 0; i < (imageHeight - 1); ++i)
		{
			int vertexIndex = (j * imageHeight) + i;
			// Top triangle (T0)
			indices[index++] = vertexIndex;                           // V0
			indices[index++] = vertexIndex + imageWidth + 1;		// V3
			indices[index++] = vertexIndex + 1;                      // V1
																	// Bottom triangle (T1)
			indices[index++] = vertexIndex;                           // V0
			indices[index++] = vertexIndex + imageWidth;            // V2
			indices[index++] = vertexIndex + imageWidth + 1;        // V3
		}
	}
	return indices;
}