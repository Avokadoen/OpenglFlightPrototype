#include "plane.hpp"

void PlaneInput::reset() {

	pitchKey			= false;
	yawKey				= false;
	leftBarrelRollLey	= false;
	rightBarrelRollKey	= false;
	breakKey			= false;
	accelKey			= false;

}

Plane::Plane(char* path, glm::vec3 velocity,
			float yawSpeed, float rollSpeed, float acceleration) : LoadedModel(path) {

	this->velocity		= velocity;
	this->yawSpeed		= yawSpeed;
	this->rollSpeed		= rollSpeed;
	this->acceleration	= acceleration;

}

void Plane::update(float deltaTime) {

	if (inputBuffer.pitchKey) {
		rotate(yawSpeed * deltaTime, glm::vec3(0, 0, 1));
	}
	if (inputBuffer.yawKey) {
		rotate(-yawSpeed * deltaTime, glm::vec3(0, 0, 1));
	}
	if (inputBuffer.leftBarrelRollLey) {
		rotate(-rollSpeed * deltaTime, glm::vec3(1, 0, 0));
	}
	if (inputBuffer.rightBarrelRollKey) {
		rotate(rollSpeed * deltaTime, glm::vec3(1, 0, 0));
	}
	if (inputBuffer.accelKey) {
		velocity.x -= acceleration * deltaTime;
	}
	if (inputBuffer.breakKey) {
		velocity.x += acceleration * deltaTime;
	}

	translate(velocity * deltaTime);

	inputBuffer.reset();
}

void Plane::inputHandler(PlaneInput frameInput) {

	inputBuffer.pitchKey			= (inputBuffer.pitchKey				|| frameInput.pitchKey);
	inputBuffer.yawKey				= (inputBuffer.yawKey				|| frameInput.yawKey);
	inputBuffer.leftBarrelRollLey	= (inputBuffer.leftBarrelRollLey	|| frameInput.leftBarrelRollLey);
	inputBuffer.rightBarrelRollKey	= (inputBuffer.rightBarrelRollKey	|| frameInput.rightBarrelRollKey);
	inputBuffer.breakKey			= (inputBuffer.breakKey				|| frameInput.breakKey);
	inputBuffer.accelKey			= (inputBuffer.accelKey				|| frameInput.accelKey);

}