
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "camera.hpp"
#include "loadedModel.hpp"
#include "terrainModel.hpp"
#include "sun.hpp"
#include "freetype.hpp"


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// screen
unsigned int screenWidth = SCR_WIDTH;
unsigned int screenHeight = SCR_HEIGHT;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 30.0f));
float lastX		= SCR_WIDTH / 2.0f;
float lastY		= SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool lightToggle = false;

// The world:
Terrain terrain(70.0f, 2.0f);
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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
	

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapInterval(1);

	freeType.loadFont(SCR_WIDTH, SCR_HEIGHT);
	freeType.setOrthoRange(SCR_WIDTH, SCR_HEIGHT);
	Shader textShader("shaders/freetype.vert", "shaders/freetype.frag");

	
	terrain.loadHeightMapData("assets/heightmap/height100.png");
	terrain.createTerrainMesh(0, 0, glm::vec3(0));
	LoadedModel plane("assets/model/ask21mi.obj");

	Shader shader("shaders/loadedModel.vert", "shaders/loadedModel.frag");
	Shader terrainShader("shaders/terrainShader.vert", "shaders/terrainShader.frag");
	
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

		theSun.update(deltaTime);
		theSun.syncWithShader(shader);
		theSun.syncWithShader(terrainShader);
		// render
		// ------
		glm::vec3 skyColor = theSun.getSkyColor();
		glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		shader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
	
		plane.Draw(shader);

		terrainShader.use();
		terrainShader.setMat4("projection", projection);
		terrainShader.setMat4("view", view);
		terrainShader.setVec3("lightPos", camera.Position);
		
		terrain.Draw(terrainShader);
		terrain.update(deltaTime);
		terrain.bindMaterialsToShader(terrainShader);

		textShader.use();
		freeType.RenderText(textShader, terrain.getSeasonString(), screenWidth / 100, screenHeight * 0.92, 1.0f, glm::vec3(1.0, 1.0, 1.0));
		freeType.RenderText(textShader, terrain.getSeasonString(), screenWidth / 100 - 10, screenHeight * 0.92, 1.2f, glm::vec3(0.0, 0.0, 0.0));
		freeType.RenderText(textShader, theSun.getTimeString(), screenWidth * 0.70, screenHeight * 0.92, 1.0f, glm::vec3(1.0, 1.0, 1.0));
		freeType.RenderText(textShader, theSun.getTimeString(), screenWidth * 0.70 - 10, screenHeight * 0.92, 1.1f, glm::vec3(0.0, 0.0, 0.0));
	

		glfwSwapBuffers(window);
		glfwPollEvents();



		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		
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
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		terrain.toggleContourStroke();
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
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	screenWidth = width;
	screenHeight = height;
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