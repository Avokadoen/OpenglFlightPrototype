#pragma once

#include "Global.hpp"
#include "GL/glew.h"
#include "shader.hpp"
#include <vector>
#include <string>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float opacity;
	Material();
	Material(glm::vec3 ambient, glm::vec3 diffuse, 
			glm::vec3 specular,float shininess, float opacity = 1.0f);
};

struct Vertex {

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh {
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Material material;
	/*  Functions  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void Draw(Shader shader);
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};