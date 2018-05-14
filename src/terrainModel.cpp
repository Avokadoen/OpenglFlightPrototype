#include "terrainModel.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>


Terrain::Terrain(float maxHeight, float blockScale) {
	this->maxHeight		= maxHeight;
	this->blockScale	= blockScale;

	seasonModifier		= 0.0f;
	seasonDirection		= 1;
	runThroughSeason	= true;
	activeContour		= 0.0f;

}

void Terrain::update(float deltaTime) {
	if (runThroughSeason == true) {
		if (seasonModifier >= 1.0f) {
			goTowardsSummerSeason();
		}

		seasonModifier += (deltaTime / (float)SecondsForAYear) * seasonDirection;

		if (seasonModifier <= 0.00f) {
			goTowardsWinterSeason();
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

	// load the image data from file
	imageData = stbi_load(path, &imageWidth, &imageHeight, &nrComponents, 0);
	if(!imageData)
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(imageData);
		return false;
	}

	// generate height values from texture
	if (!generateHeightValues()) {
		std::cout << "failed to generate height map" << std::endl;
		return false;
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

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}

	Mesh mesh(vertices, indices);
	meshes.push_back(mesh);
}

void Terrain::generateIndices() {	
	
	const unsigned int numTriangles = (imageWidth - 1) * (imageHeight - 1) * 2;

	// 3 indices for each triangle in the terrain mesh
	unsigned int index = 0; // Index in the index buffer
	for (unsigned int y = 0; y < (imageHeight - 1); y++)
	{
		for (unsigned int x = 0; x < (imageWidth - 1); x++)
		{
			int vertexIndex = (y * imageWidth) + x;

			// Top triangle
			indices.push_back(vertexIndex);                          
			indices.push_back(vertexIndex + imageWidth);		
			indices.push_back(vertexIndex + 1);                     
				
			// bottom triangle
			indices.push_back(vertexIndex + 1);                         
			indices.push_back(vertexIndex + imageWidth + 1);          
			indices.push_back(vertexIndex + imageWidth);       
		}
	}

}

void Terrain::generateNormals(int offset) {
	for (unsigned int i = 0 + offset; i < indices.size() - 3; i += 3)
	{
		glm::vec3 v0 = vertices[indices[i + 0]].Position;
		glm::vec3 v1 = vertices[indices[i + 1]].Position;
		glm::vec3 v2 = vertices[indices[i + 2]].Position;

		// we find v2 and v1 vector from face's space instead of global space
		// and calculate the normal from v0 
		glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);

		// apply v0 normal onto all other vertices so that we can find average normal later
		vertices[indices[i + 0]].Normal += normal;
		vertices[indices[i + 1]].Normal += normal;
		vertices[indices[i + 2]].Normal += normal;
	}
}

// send data to shader
void Terrain::bindMaterialsToShader(Shader shader) {
	
	shader.setFloat("maxHeight", maxHeight);
	shader.setFloat("seasonModifier", seasonModifier);
	shader.setFloat("contourStroke", activeContour);

	shader.setVec3("snow.ambient", snow.ambient);
	shader.setVec3("snow.diffuse", snow.diffuse);
	shader.setVec3("snow.specular", snow.specular);
	shader.setFloat("snow.shininess", snow.shininess);
	shader.setFloat("SummerSnowBottom", SummerSnowBottom );
	shader.setFloat("WinterSnowBottom", WinterSnowBottom);

	shader.setVec3("ice.ambient", ice.ambient);
	shader.setVec3("ice.diffuse", ice.diffuse);
	shader.setVec3("ice.specular", ice.specular);
	shader.setFloat("ice.shininess", ice.shininess);
	shader.setFloat("SummerIceBottom", SummerIceBottom);
	shader.setFloat("WinterIceBottom", WinterIceBottom);

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

void Terrain::toggleRunThroughSeason() {
	runThroughSeason = !runThroughSeason;
}

void Terrain::toggleContourStroke() {
	if (activeContour > 0) {
		activeContour = 0;
	}
	else activeContour = contourStroke;
}

void Terrain::goTowardsSummerSeason() {
	seasonDirection = -1;
}

void Terrain::goTowardsWinterSeason() {
	seasonDirection = 1;
}

std::string Terrain::getSeasonString() {
	if (seasonModifier >= 0.65)
		return "Winter";
	else if (seasonModifier <= 0.35) {
		return "Summer";
	}
	else {
		if (seasonDirection > 0) {
			return "Autumn";
		}
		else return "Spring";
	}
}

// find the position of the middle vertice in the terrain
glm::vec3 Terrain::getActualPos() {
	auto middle = vertices.begin() + ((imageHeight / 2) * imageWidth) + (imageWidth / 2);
	return middle->Position;
}

// choose a random vertice within given(hardcoded) bounds
glm::vec3 Terrain::getRandomValidPos() {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6((int)(imageWidth * (imageHeight / 5)), 
																	(int)(imageWidth * (imageHeight / 1)));
	auto point = vertices.begin() + dist6(rng);
	return point->Position;
}

glm::vec3 Terrain::getRandomTreeValidPos() {

	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6((int)(imageWidth * (imageHeight / 5)),
		(int)(imageWidth * (imageHeight / 1)));

	while (true) {
		auto point = vertices.begin() + dist6(rng);
		if (point->Position.y/maxHeight >= SummerGrassBottom && point->Position.y / maxHeight <= SummerMudBottom * 0.8f)
			return point->Position + glm::vec3(0, 2, 0);
	}
	

}

// generates transforms that can be used to place objects on the surface of the map
std::vector<glm::mat4> Terrain::generateTreeTransforms(int amount) {
	std::vector<glm::mat4> transforms;

	for (int i = 0; i < amount; i++) {
		transforms.push_back(glm::translate(glm::mat4(1),getRandomTreeValidPos()));
	}
	return transforms;
}