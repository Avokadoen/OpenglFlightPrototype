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

Terrain::Terrain(float heightScale, float blockScale) {

}

bool Terrain::generateHeightValue() {

	int index = 0;
	if (imageData) {

		// While imagedata is pointing on data
		// iterate through image
		while ((index * nrComponents) < fileSize) {
			heightValues.push_back(GetHeightValue(&imageData[index * nrComponents]));
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


bool Terrain::loadHeightMap(const char *path)
{
	std::ifstream ifs;
	ifs.open(path, std::ifstream::binary);
	if (ifs.fail())
	{
		std::cerr << "Failed to open file: " << path << std::endl;
		return false;
	}
	fileSize = GetFileLength(ifs);
	ifs.close();

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int height;
	imageData = new unsigned char[fileSize];
	imageData = stbi_load(path, &imageWidth, &height, &nrComponents, 0);
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
		glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, height, 0, format, GL_UNSIGNED_BYTE, imageData);
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
	generateHeightValue();
	//imageData = data;
	return true;
}

void Terrain::freeImageData() {
	stbi_image_free(imageData);
}