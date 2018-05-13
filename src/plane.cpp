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
	float yaw = 0.0f;
	float pitch = 0.0f;

}

void Plane::update(float deltaTime) {

	if (inputBuffer.pitchKey) {
		rotate(yawSpeed * deltaTime, glm::vec3(0, 0, 1));
		yaw += yawSpeed * deltaTime;
	}
	if (inputBuffer.yawKey) {
		rotate(-yawSpeed * deltaTime, glm::vec3(0, 0, 1));
		yaw -= yawSpeed * deltaTime;
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
	/*if (yaw < 0) {
		velocity.x -= acceleration * deltaTime; //* glm::radians(yaw);
	}
	if (yaw > 0) {
		velocity.x += acceleration * deltaTime; //* glm::radians(yaw);
	}*/

	translate(velocity * deltaTime);

	inputBuffer.reset();
}

void Plane::Draw(Shader shader){
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	Model::Draw(shader);
	//glDisable(GL_BLEND);
}

void Plane::inputHandler(PlaneInput frameInput) {

	inputBuffer.pitchKey			= (inputBuffer.pitchKey				|| frameInput.pitchKey);
	inputBuffer.yawKey				= (inputBuffer.yawKey				|| frameInput.yawKey);
	inputBuffer.leftBarrelRollLey	= (inputBuffer.leftBarrelRollLey	|| frameInput.leftBarrelRollLey);
	inputBuffer.rightBarrelRollKey	= (inputBuffer.rightBarrelRollKey	|| frameInput.rightBarrelRollKey);
	inputBuffer.breakKey			= (inputBuffer.breakKey				|| frameInput.breakKey);
	inputBuffer.accelKey			= (inputBuffer.accelKey				|| frameInput.accelKey);

}

glm::vec3 Plane::getPostion() {
	glm::vec3 position;
	glm::decompose(transform, glm::vec3(), glm::quat(), position, glm::vec3(), glm::vec4());
	return position;
}

void Plane::setPosition(glm::vec3 position, glm::vec3 lookAtPos) {
	transform = glm::mat4(1);
	translate(position);
	
	if (position.z > lookAtPos.z) {
		rotate(180, glm::vec3(0, 1, 0));
	}
	else{
		rotate(-180, glm::vec3(0, 1, 0));
	}

}

float Plane::cheatGetSpeed() {
	return velocity.x * -1 * 10;
}

