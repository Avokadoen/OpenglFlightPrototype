
#include "sun.hpp"

Sun::Sun() {
	time			= DAY;
	skyColor		= DaySkyColor;
	lerpPos			= 0;
	secondsInADay	= 20.0f;
	currentLight	= Day;
	timeRunning		= true;
}

// rotate through time states and lerp between pre-set dirlights
// the resulting lerped dirlight is saved and synced to shader later
void Sun::update(float deltaTime) {
	if (!timeRunning) deltaTime = 0;

	if (time == DAY) {
		if (lerpPos < 1.0f) {
			currentLight = lerpDirLight(Day, Noon, lerpPos);
			skyColor = lerpVec(DaySkyColor, NoonSkyColor, lerpPos);
			lerpPos += deltaTime / secondsInADay;
		}
		else {
			time = NOON;
			lerpPos = 0;
		}
	}

	if (time == NOON) {
		if (lerpPos < 1.0f) {
			currentLight = lerpDirLight(Noon, Night, lerpPos);
			skyColor = lerpVec(NoonSkyColor, NightSkyColor, lerpPos);
			lerpPos += deltaTime / secondsInADay;
		}
		else {
			time = NIGHT;
			lerpPos = 0;
		}
	}

	if (time == NIGHT) {
		if (lerpPos < 1.0f) {
			currentLight = lerpDirLight(Night, Morning, lerpPos);
			skyColor = lerpVec(NightSkyColor, MorningSkyColor, lerpPos);
			lerpPos += deltaTime / secondsInADay;
		}
		else {
			time = MORNING;
			lerpPos = 0;
		}
	}

	if (time == MORNING) {
		if (lerpPos < 1.0f) {
			currentLight = lerpDirLight(Morning, Day, lerpPos);
			skyColor = lerpVec(MorningSkyColor, DaySkyColor, lerpPos);
			lerpPos += deltaTime / secondsInADay;
		}
		else {
			time = DAY;
			lerpPos = 0;
		}
	}
	
}

void Sun::syncWithShader(Shader shader) {
	shader.use();
	shader.setBool("dirSet", true);
	shader.setVec3("dirLight.direction", currentLight.direction);
	shader.setVec3("dirLight.ambient", currentLight.ambient);
	shader.setVec3("dirLight.diffuse", currentLight.diffuse);
	shader.setVec3("dirLight.specular", currentLight.specular);
}

// get skycolor can be use as a skybox in combination with glClearColor
glm::vec3 Sun::getSkyColor() {
	return skyColor;
}

void Sun::toggleTime() {
	timeRunning = !timeRunning;
}

void Sun::setTime(TimeOfTheDay time, float lerpPos) {
	this->time = time;
	this->lerpPos = lerpPos;
}

std::string Sun::getTimeString() {
	std::string rtrString;
	float minutes;
	switch (time) {
	case DAY:
		// get time of the day in minutes
		minutes = lerp(720, 1080, lerpPos);
		rtrString = "Day, ";
		// convert minutes to clock format
		getTimeFromMinuteInString(minutes, rtrString);
		break;
	case NOON:
		minutes = lerp(1080, 1440, lerpPos);
		rtrString = "Noon, ";
		getTimeFromMinuteInString(minutes, rtrString);
		break;
	case NIGHT:
		minutes = lerp(0, 360, lerpPos);
		rtrString = "Night, ";
		getTimeFromMinuteInString(minutes, rtrString);
		break;
	case MORNING:
		minutes = lerp(360, 720, lerpPos);
		rtrString = "Morning, ";
		getTimeFromMinuteInString(minutes, rtrString);
		break;
	}
	return rtrString;
}

DirLight Sun::lerpDirLight(DirLight v0, DirLight v1, float t) {
	DirLight outPut;
	outPut.direction	= lerpVec(v0.direction, v1.direction, t);
	outPut.ambient		= lerpVec(v0.ambient, v1.ambient, t);
	outPut.diffuse		= lerpVec(v0.diffuse, v1.diffuse, t);
	outPut.specular		= lerpVec(v0.specular, v1.specular, t);
	return outPut;
}

glm::vec3 Sun::lerpVec(glm::vec3 v0, glm::vec3 v1, float t) {
	glm::vec3 outPut;
	outPut.x = lerp(v0.x, v1.x, t);
	outPut.y = lerp(v0.y, v1.y, t);
	outPut.z = lerp(v0.z, v1.z, t);
	return outPut;
}

float Sun::lerp(float v0, float v1, float t){
	return (1 - t) * v0 + t * v1;
}

void Sun::getTimeFromMinuteInString(float minutes, std::string& time) {
	int hours = minutes / 60;
	int clockMinutes = (int)minutes % 60;


	time.append(std::to_string(hours));
	time.append(":");
	if(clockMinutes < 10) time.append("0");
	time.append(std::to_string(clockMinutes));
}

