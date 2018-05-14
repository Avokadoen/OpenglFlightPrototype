
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "camera.hpp"
#include "loadedModel.hpp"
#include "terrainModel.hpp"
#include "sun.hpp"
#include "freetype.hpp"
#include "plane.hpp"
#include "tree.hpp"

#include <memory>


// callback functions for glfw
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 900;

// screen
std::unique_ptr<int> screenWidth = std::make_unique<int>(SCR_WIDTH);
std::unique_ptr<int> screenHeight = std::make_unique<int>(SCR_HEIGHT);
std::unique_ptr<int> screenPosX = std::make_unique<int>(0);
std::unique_ptr<int> screenPosY = std::make_unique<int>(SCR_HEIGHT);

// fullscreen oriente
bool isFullScreen = false;
std::unique_ptr<int> prevScreenWidth = std::make_unique<int>(SCR_WIDTH);
std::unique_ptr<int> prevScreenHeight = std::make_unique<int>(SCR_HEIGHT);
void toggleFullscreen(GLFWwindow* window);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 30.0f));
float lastX		= SCR_WIDTH / 2.0f;
float lastY		= SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool lightToggle = false;
bool writeHud = true;

// The world:
Terrain terrain(200.0f, 1.3f);
Sun theSun;

// text
FreeType freeType;




int main() {

	if (!glfwInit())
	{
		// Initialization failed
		return -4;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exam", nullptr, nullptr); // glfwGetPrimaryMonitor()
	if (!window)
	{
		// Window or OpenGL context creation failed
		return -3;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	// bind glfw events to custom functions
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	glewExperimental = GL_FALSE;
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}

	if (glewInit() != GLEW_OK) {
		return -2;
		glfwTerminate();
	}
	
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// vsync
	glfwSwapInterval(1);

	// text init
	freeType.loadFont(SCR_WIDTH, SCR_HEIGHT);
	freeType.setOrthoRange(SCR_WIDTH, SCR_HEIGHT);
	Shader textShader("shaders/freetype.vert", "shaders/freetype.frag");

	// terrain init
	terrain.loadHeightMapData("assets/heightmap/height50.png");
	terrain.createTerrainMesh(0, 0, glm::vec3(0));
	Shader terrainShader("shaders/terrainShader.vert", "shaders/terrainShader.frag");

	// plane init
	Plane plane("assets/model/ask21mi.obj");
	PlaneInput frameInput;
	Shader shader("shaders/loadedModel.vert", "shaders/loadedModel.frag");
	plane.setPosition(terrain.getActualPos() + glm::vec3(0, 50, 0), terrain.getActualPos() + glm::vec3(20, 50, 0));
	plane.rotate(90, glm::vec3(0, 1, 0));
	
	// tree init
	Tree tree("assets/model/lowpolytree.obj", terrain.generateTreeTransforms(4000));
	
	
	float lastFrame = 0;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float lightX = 0;

	terrainShader.use();
	terrain.bindMaterialsToShader(terrainShader);

	terrain.getActualPos();

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// deal with input
		glfwPollEvents();

		// plane input
		frameInput.reset();
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			frameInput.pitchKey = true;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			frameInput.yawKey = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			frameInput.rightBarrelRollKey = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			frameInput.leftBarrelRollLey = true;
		}
		if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
			frameInput.accelKey = true;
		}
		if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
			frameInput.breakKey = true;
		}	
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			plane.setPosition(terrain.getActualPos() + glm::vec3(0, 50, 0), terrain.getActualPos() + glm::vec3(20, 50, 0));
			plane.rotate(90, glm::vec3(0, 1, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			plane.setPosition(terrain.getRandomValidPos() + glm::vec3(0, 50, 0), terrain.getActualPos() + glm::vec3(0, 50, 0));
			plane.rotate(90, glm::vec3(0, 1, 0));
		}

		// camera
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			camera.ProcessKeyboard(UP, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			camera.ProcessKeyboard(DOWN, deltaTime);
		}

		theSun.update(deltaTime);
		theSun.syncWithShader(shader);
		theSun.syncWithShader(terrainShader);

		plane.inputHandler(frameInput);
		plane.update(deltaTime);
		camera.update(plane.getTransform());

		// render
		// ------
		glm::vec3 skyColor = theSun.getSkyColor();
		glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)*screenWidth / (float)*screenHeight, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		terrainShader.use();

		terrainShader.setMat4("projection", projection);
		terrainShader.setMat4("view", view);
		terrainShader.setVec3("lightPos", camera.Position);
		
		terrain.Draw(terrainShader);
		terrain.update(deltaTime);
		terrain.bindMaterialsToShader(terrainShader);
	
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		tree.Draw(shader);
		plane.Draw(shader);

		if (writeHud) {
			textShader.use();
			freeType.RenderText(textShader, terrain.getSeasonString(), (*screenWidth) / 100, (*screenHeight) * 0.92, 1.0f, glm::vec3(1.0, 1.0, 1.0));
			freeType.RenderText(textShader, terrain.getSeasonString(), (*screenWidth) / 100 - 10, (*screenHeight) * 0.92, 1.2f, glm::vec3(0.0, 0.0, 0.0));
			freeType.RenderText(textShader, theSun.getTimeString(), (*screenWidth) * 0.70, (*screenHeight) * 0.92, 1.0f, glm::vec3(1.0, 1.0, 1.0));
			freeType.RenderText(textShader, theSun.getTimeString(), (*screenWidth) * 0.70 - 10, (*screenHeight) * 0.92, 1.1f, glm::vec3(0.0, 0.0, 0.0));
			freeType.RenderText(textShader, std::to_string((int)plane.cheatGetSpeed()) + " km/h", (*screenWidth) / 100, (*screenHeight)  * 0.10, 1.0f, glm::vec3(1.0, 1.0, 1.0));
			freeType.RenderText(textShader, std::to_string((int)plane.cheatGetSpeed()) + " km/h", (*screenWidth) / 100 - 10, (*screenHeight) * 0.10, 1.1f, glm::vec3(0.0, 0.0, 0.0));
		}
		
		glfwSwapBuffers(window);	
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

void error_callback(int error, const char* description)
{
	std::cout << "Error " << error << ": " << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// misc
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		toggleFullscreen(window);
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		terrain.toggleContourStroke();

	// seasons
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		terrain.toggleRunThroughSeason();

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		terrain.setSeason(0.5f);
		terrain.goTowardsSummerSeason();
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		terrain.setSeason(0.0f);
		terrain.goTowardsWinterSeason();
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		terrain.setSeason(0.5f);
		terrain.goTowardsWinterSeason();
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		terrain.setSeason(1.0f);
		terrain.goTowardsSummerSeason();
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		terrain.setSeason(1.0f);
		terrain.goTowardsSummerSeason();
	}

	// Day and night 
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		theSun.setTime(DAY, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		theSun.setTime(NOON, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		theSun.setTime(NIGHT, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		theSun.setTime(MORNING, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		theSun.toggleTime();
	}
	// Camera
	if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS) {
		camera.rotateState();
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		writeHud = !writeHud;
	}
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	*screenWidth = width;
	*screenHeight = height;
	freeType.setOrthoRange(width, height);
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset*deltaTime, yoffset*deltaTime);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void toggleFullscreen(GLFWwindow* window) {
	if (!isFullScreen) {

		// create pointers to retrieve window data
		int* posX	= new int;
		int* posY	= new int;
		int* width	= new int;
		int* height = new int;

		// retrieve data from glfw
		glfwGetWindowPos(window, posX, posY);
		glfwGetWindowSize(window, width, height);

		// backup window position and window size
		screenPosX = std::make_unique<int>(*posX);
		screenPosY = std::make_unique<int>(*posY);
		prevScreenWidth = std::make_unique<int>(*width);
		prevScreenHeight = std::make_unique<int>(*height);

		// clean up
		delete posX;
		delete posY;
		delete width;
		delete height;

		// get reolution of monitor
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		// switch to full screen	
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
		glfwSwapInterval(1);
		isFullScreen = true;
	}
	else {
		// restore last window size and position
		glfwSetWindowMonitor(window, nullptr, *screenPosX, *screenPosY, *prevScreenWidth, *prevScreenHeight, 0);
		screenWidth	= std::make_unique<int>(*prevScreenWidth);
		screenHeight = std::make_unique<int>(*prevScreenHeight);
		isFullScreen = false;
	}


	
}