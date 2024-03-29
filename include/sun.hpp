#pragma once
#include "loadedModel.hpp"
#include "shader.hpp"
struct DirLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

enum TimeOfTheDay {DAY, NOON, NIGHT, MORNING};

// encapsulation of dirlight to simulate the sun.
class Sun {
public:
	Sun();
	void update(float deltaTime);
	void syncWithShader(Shader shader);
	glm::vec3 getSkyColor();
	void toggleTime();
	void setTime(TimeOfTheDay t, float lerpPos);
	std::string getTimeString();
	DirLight lerpDirLight(DirLight v0, DirLight v1, float t);
	glm::vec3 lerpVec(glm::vec3 v0, glm::vec3 v1, float t);
	float lerp(float v0, float v1, float t);
	void getTimeFromMinuteInString(float minutes, std::string& time);
private:

	TimeOfTheDay time;
	float lerpPos;
	float secondsInADay;
	DirLight currentLight;
	bool timeRunning;
	glm::vec3 skyColor;

	const glm::vec3 DaySkyColor = glm::vec3(0.3, 0.3, 1.0);
	const glm::vec3 NoonSkyColor = glm::vec3(0.3, 0.3, 0.5);
	const glm::vec3 NightSkyColor = glm::vec3(0.0, 0.0, 0.05);
	const glm::vec3 MorningSkyColor = glm::vec3(0.3, 0.1, 0.01);

	const DirLight Day{
		glm::vec3(0.2, -0.7, 0.1),
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.5, 0.5, 0.5),
		glm::vec3(0.5, 0.5, 0.5) 
	};

	const DirLight Noon{
		glm::vec3(-0.5, -0.6, 0.1),
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.4, 0.4, 0.4),
		glm::vec3(0.3, 0.3, 0.3)
	};

	const DirLight Night{
		glm::vec3(0.0, -0.1, 0.2),
		glm::vec3(0),
		glm::vec3(0.05, 0.05, 0.05),
		glm::vec3(0.1, 0.1, 0.1)
	};

		const DirLight Morning{
		glm::vec3(0.5, -0.6, 0.1),
		glm::vec3(0.1, 0.1, 0.1),
		glm::vec3(0.4, 0.2, 0.2),
		glm::vec3(0.5, 0.5, 0.5) 
	};
};