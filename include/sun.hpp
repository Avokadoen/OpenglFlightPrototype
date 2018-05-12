#pragma once
#include "loadedModel.hpp"
#include "shader.hpp"
struct DirLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

enum TimeOfTheDay {DAY, DUSK, NIGHT, DAWN};

class Sun {
public:
	Sun();
	void syncWithShader(Shader shader);
	void update(float deltaTime);
	DirLight lerpDirLight(DirLight v0, DirLight v1, float t);
	glm::vec3 lerpVec(glm::vec3 v0, glm::vec3 v1, float t);
	float lerp(float v0, float v1, float t);

private:

	TimeOfTheDay time;
	float lerpPos;
	float secondsInADay;
	DirLight currentLight;


	const DirLight Day{
		glm::vec3(0.0, -0.9, 0.1),
		glm::vec3(0.5, 0.5, 0.3),
		glm::vec3(0.6, 0.6, 0.4),
		glm::vec3(0.5, 0.5, 0.5) 
	};

	const DirLight Dusk{
		glm::vec3(-0.5, -0.6, 0.1),
		glm::vec3(0.3, 0.3, 0.1),
		glm::vec3(0.4, 0.4, 0.4),
		glm::vec3(0.3, 0.3, 0.3)
	};

	const DirLight Night{
		glm::vec3(0.0, -0.1, 0.2),
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.4, 0.4, 0.4)
	};

		const DirLight Dawn{
		glm::vec3(0.5, -0.6, 0.1),
		glm::vec3(0.45, 0.2, 0.2),
		glm::vec3(0.55, 0.34, 0.33),
		glm::vec3(0.5, 0.5, 0.5) 
	};
};w