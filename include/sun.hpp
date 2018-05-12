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
	void update(float deltaTime);
	void syncWithShader(Shader shader);
	glm::vec3 getSkyColor();
	void toggleTime();
	void setTime(TimeOfTheDay t, float lerpPos);
	DirLight lerpDirLight(DirLight v0, DirLight v1, float t);
	glm::vec3 lerpVec(glm::vec3 v0, glm::vec3 v1, float t);
	float lerp(float v0, float v1, float t);
	
private:

	TimeOfTheDay time;
	float lerpPos;
	float secondsInADay;
	DirLight currentLight;
	bool timeRunning;
	glm::vec3 skyColor;

	const glm::vec3 DaySkyColor = glm::vec3(0.3, 0.3, 1.0);
	const glm::vec3 DuskSkyColor = glm::vec3(0.3, 0.3, 0.5);
	const glm::vec3 NightSkyColor = glm::vec3(0.0, 0.0, 0.05);
	const glm::vec3 DawnSkyColor = glm::vec3(0.3, 0.1, 0.01);

	const DirLight Day{
		glm::vec3(0.0, -0.9, 0.1),
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.5, 0.5, 0.5),
		glm::vec3(0.5, 0.5, 0.5) 
	};

	const DirLight Dusk{
		glm::vec3(-0.5, -0.6, 0.1),
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.4, 0.4, 0.4),
		glm::vec3(0.3, 0.3, 0.3)
	};

	const DirLight Night{
		glm::vec3(0.0, -0.1, 0.2),
		glm::vec3(0),
		glm::vec3(0.05, 0.05, 0.05),
		glm::vec3(0.4, 0.4, 0.4)
	};

		const DirLight Dawn{
		glm::vec3(0.5, -0.6, 0.1),
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.4, 0.2, 0.2),
		glm::vec3(0.5, 0.5, 0.5) 
	};
};