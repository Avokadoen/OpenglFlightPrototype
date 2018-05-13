#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "loadedModel.hpp"
#include "gtx/matrix_decompose.hpp"

struct PlaneInput {
	bool pitchKey;
	bool yawKey;
	bool leftBarrelRollLey;
	bool rightBarrelRollKey;
	bool breakKey;
	bool accelKey;

	void reset();
};

class Plane : public LoadedModel {
public:
	Plane(char* path, glm::vec3 velocity = glm::vec3(-20.0f, 0, 0),
		float yawSpeed = 35.0f, float rollSpeed = 50.0f, float acceleration = 10.0f);
	void update(float deltaTime);
	void inputHandler(PlaneInput frameInput);
	glm::vec3 getPostion();
private:
	glm::vec3 velocity;
	float acceleration;
	float yawSpeed;
	float rollSpeed;
	PlaneInput inputBuffer;
};

