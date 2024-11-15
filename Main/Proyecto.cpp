#include <iostream>
#include <cmath>
#include <glm/gtc/constants.hpp> 
#ifndef M_PI			
#define M_PI 3.14159
#endif


// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "Texture.h"

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1700, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;



// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);
//Anim
//float rotBall = 0.0f;
float rotBear = 0.0f;
float rotBearX = 0.0f;
int BearAnim = 0;
float FLegs = 0.0f;
float FLegsI = 0.0f;
float FLegsD = 0.0f;
float RLegs = 0.0f;
float RLegsAux = 0.0f;
float head = 0.0f;
float circularPathSpeed = 0.001f;
float circularAngle = 0.0f;
const float angularSpeed = 0.0009f;
float valor = 0.5f;  // 'f' indica que es un float, no un double


bool moveInCircle = false; // Controla si el perro está en movimiento circular

const float RADIUS = 0.3f;  // Radio del círculo



//KeyFrames
float BearPosX, BearPosY, BearPosZ;

#define MAX_FRAMES 20
int i_max_steps = 600; //190
int i_curr_steps = 0;
typedef struct _frame {

	float rotBear;
	float rotBearInc;
	float rotBearX;
	float rotBearXInc;
	float BearPosX;
	float BearPosY;
	float BearPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float headInc;
	float tail;
	float tailInc;
	float FLegs;
	float FLegsInc;
	float RLegs;
	float RLegsInc;
	float FLegsI;
	float FLegsIInc;
	float FLegsD;
	float FLegsDInc;

	float RLegsAux;
	float RLegsAuxInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].BearPosX = BearPosX;
	KeyFrame[FrameIndex].BearPosY = BearPosY;
	KeyFrame[FrameIndex].BearPosZ = BearPosZ;

	KeyFrame[FrameIndex].rotBear = rotBear;
	KeyFrame[FrameIndex].rotBearX = rotBearX;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].FLegs = FLegs;
	KeyFrame[FrameIndex].RLegs = RLegs;

	KeyFrame[FrameIndex].FLegsI = FLegsI;
	KeyFrame[FrameIndex].FLegsD = FLegsD;
	KeyFrame[FrameIndex].RLegsAux = RLegsAux;

	FrameIndex++;
}

//////////////////////////////////////////////////////////////////				guardado y carga de la animación
void saveKeyframesToFile(const char* filename) {
	std::ofstream file(filename);
	if (file.is_open()) {
		for (int i = 0; i < FrameIndex; ++i) {
			file << KeyFrame[i].BearPosX << " " << KeyFrame[i].BearPosY << " " << KeyFrame[i].BearPosZ << " ";
			file << KeyFrame[i].rotBear << " " << KeyFrame[i].head << " " << KeyFrame[i].tail << " ";
			file << KeyFrame[i].FLegs << " " << KeyFrame[i].RLegs << " " << KeyFrame[i].FLegsI << " " << KeyFrame[i].FLegsD << " ";
			file << KeyFrame[i].RLegsAux << " " << KeyFrame[i].rotBearX << "\n"; // Agregamos rotDogX
		}
		file.close();
	}
	else {
		std::cout << "Error: Couldn't open file for saving keyframes." << std::endl;
	}
}

void loadKeyframesFromFile(const char* filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		FrameIndex = 0;
		while (!file.eof() && FrameIndex < MAX_FRAMES) {
			file >> KeyFrame[FrameIndex].BearPosX >> KeyFrame[FrameIndex].BearPosY >> KeyFrame[FrameIndex].BearPosZ;
			file >> KeyFrame[FrameIndex].rotBear >> KeyFrame[FrameIndex].head >> KeyFrame[FrameIndex].tail;
			file >> KeyFrame[FrameIndex].FLegs >> KeyFrame[FrameIndex].RLegs >> KeyFrame[FrameIndex].FLegsI >> KeyFrame[FrameIndex].FLegsD;
			file >> KeyFrame[FrameIndex].RLegsAux >> KeyFrame[FrameIndex].rotBearX; // Leemos rotDogX
			FrameIndex++;
		}
		file.close();
	}
	else {
		std::cout << "Error: Couldn't open file for loading keyframes." << std::endl;
	}
}




void resetElements(void)
{
	BearPosX = KeyFrame[0].BearPosX;
	BearPosY = KeyFrame[0].BearPosY;
	BearPosZ = KeyFrame[0].BearPosZ;
	head = KeyFrame[0].head;
	FLegs = KeyFrame[0].FLegs;
	RLegs = KeyFrame[0].RLegs;

	FLegsI = KeyFrame[0].FLegsI;
	FLegsD = KeyFrame[0].FLegsD;
	RLegsAux = KeyFrame[0].RLegsAux;


	rotBear = KeyFrame[0].rotBear;
	rotBearX = KeyFrame[0].rotBearX;

}

void interpolation() {
	for (int i = 0; i < MAX_FRAMES - 1; ++i) {
		KeyFrame[i].incX = (KeyFrame[i + 1].BearPosX - KeyFrame[i].BearPosX) / i_max_steps;
		KeyFrame[i].incY = (KeyFrame[i + 1].BearPosY - KeyFrame[i].BearPosY) / i_max_steps;
		KeyFrame[i].incZ = (KeyFrame[i + 1].BearPosZ - KeyFrame[i].BearPosZ) / i_max_steps;
		KeyFrame[i].rotBearInc = (KeyFrame[i + 1].rotBear - KeyFrame[i].rotBear) / i_max_steps;
		KeyFrame[i].headInc = (KeyFrame[i + 1].head - KeyFrame[i].head) / i_max_steps;
		KeyFrame[i].FLegsIInc = (KeyFrame[i + 1].FLegsI - KeyFrame[i].FLegsI) / i_max_steps;
		KeyFrame[i].FLegsDInc = (KeyFrame[i + 1].FLegsD - KeyFrame[i].FLegsD) / i_max_steps;
		KeyFrame[i].RLegsInc = (KeyFrame[i + 1].RLegs - KeyFrame[i].RLegs) / i_max_steps;
		KeyFrame[i].FLegsInc = (KeyFrame[i + 1].FLegs - KeyFrame[i].FLegs) / i_max_steps;

		// Interpolación para RLegsAux y rotDogX
		KeyFrame[i].RLegsAuxInc = (KeyFrame[i + 1].RLegsAux - KeyFrame[i].RLegsAux) / i_max_steps;
		KeyFrame[i].rotBearXInc = (KeyFrame[i + 1].rotBearX - KeyFrame[i].rotBearX) / i_max_steps;
	}
}






// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");


	// --------------------------------------------------------------------------------------------------------
	// -------------------------- CARGA DE MODELOS -------------------------------------------------------------
	Model mountain((char*)"Models/mountain/mountain.obj");
	Model Tree1((char*)"Models/trees/tree.obj");
	//Model Stand((char*)"Models/stand/stand.obj");
	//Model Sculptures((char*)"Models/sculptures/sculptures.obj");
	Model Base((char*)"Models/sculptures/base.obj");
	Model Floor((char*)"Models/house/snowFloor.obj");
	//Model House((char*)"Models/house/house.obj");
	//Model Windows((char*)"Models/house/windows.obj");
	Model Snowman((char*)"Models/snowman/snowman.obj");
	//Model Doors((char*)"Models/house/doors.obj"); 
	//Model fishTank((char*)"Models/house/fishTank.obj"); 
	Model ski((char*)"Models/ski/ski.obj");
	//Model SkiLift((char*)"Models/skiLift/skiLift.obj");
	Model MotoSki((char*)"Models/motoski/motoski.obj");
		//Model Donovan((char*)"Models/restaurant/restaurant.obj");
	//Model DonovanWindows((char*)"Models/restaurant/windowsRes.obj");
	//Model Ball((char*)"Models/ball.obj");
	Model bearBody((char*)"Models/bear/bearBody.obj");
	Model HeadBear((char*)"Models/bear/HeadBear.obj");
	Model F_RightLeg((char*)"Models/bear/F_RightLegBear.obj");
	Model F_LeftLeg((char*)"Models/bear/F_LeftLegBear.obj");
	Model B_RightLeg((char*)"Models/bear/B_RightLegBear.obj");
	Model B_LeftLeg((char*)"Models/bear/B_LeftLegBear.obj");

	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].BearPosX = 0.0f;
		KeyFrame[i].BearPosY = 0.0f;
		KeyFrame[i].BearPosZ = 0.0f;
		KeyFrame[i].incX = 0.0f;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotBear = 0;
		KeyFrame[i].rotBearInc = 0;
		KeyFrame[i].rotBearX = 0;
		KeyFrame[i].rotBearXInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].FLegs = 0;
		KeyFrame[i].FLegsInc = 0;
		KeyFrame[i].RLegs = 0;
		KeyFrame[i].RLegsInc = 0;
		KeyFrame[i].FLegsI = 0;
		KeyFrame[i].FLegsIInc = 0;
		KeyFrame[i].FLegsD = 0;
		KeyFrame[i].FLegsDInc = 0;
		KeyFrame[i].RLegsAux = 0;
		KeyFrame[i].RLegsAuxInc = 0;
	}

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGenBuffers(1, &EBO);
	///////////////////////////////////////////////


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//skybox
	GLuint skyBoxVBO, skyBoxVAO;
	glGenVertexArrays(1, &skyBoxVAO);
	glGenBuffers(1, &skyBoxVBO);
	glBindVertexArray(skyBoxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Load textures
	vector < const GLchar* > faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	///////////Aquí cierra la configuración del skybox

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));




		//Carga de modelo 
		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		glm::mat4 modelWindows;
		glm::mat4 modelWindows2;
		glm::mat4 modelSculptures;
		glm::mat4 modelBase;
		glm::mat4 modelSki;
		glm::mat4 modelSkiLift;
		glm::mat4 modelBody;
		glm::mat4 modelMoto;

		/*
		glm::mat4 modelHead;
		glm::mat4 modelFR;
		glm::mat4 modelFL;
		glm::mat4 modelBR;
		glm::mat4 modelBL;*/

		/*=====================================  Para dibujar los modelos ==================================*/
		//				------------------------------ Mountain --------------------------
		model = glm::mat4(1);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mountain.Draw(lightingShader);

		//				------------------------------ Stand --------------------------
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(-25.0f, -0.80f, -1.0f));  // Ajuste de posición
		//model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));  // Escala reducida
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Stand.Draw(lightingShader);

		//				------------------------------ Bases --------------------------
		modelBase = glm::mat4(1);
		//modelBase = glm::translate(modelBase, glm::vec3(-10.0f, -0.80f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBase));
		Base.Draw(lightingShader);

		//				------------------------------ Esculturas --------------------------
		modelSculptures = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSculptures));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSculptures));
		//Sculptures.Draw(lightingShader);
		glDisable(GL_BLEND);

		//				------------------------------ Ski --------------------------  
		modelSki = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSki));
		ski.Draw(lightingShader);

		//				------------------------------ Ski Telesilla --------------------------
		modelSkiLift = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSkiLift));
		//SkiLift.Draw(lightingShader);

		//				------------------------------ MotoSki --------------------------
		modelSkiLift = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSkiLift));
		MotoSki.Draw(lightingShader);

		//		--------------------------------------- Snowman ----------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.Draw(lightingShader);

		//			 ------------------------------ Restaurant --------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Donovan.Draw(lightingShader);

		//			 ------------------------------ Windows Restaurant --------------------------
		modelWindows = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelWindows));
		//DonovanWindows.Draw(lightingShader);
		glDisable(GL_BLEND);

		//			 ------------------------------ Casas 1 --------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//House.Draw(lightingShader);

		//			 ------------------------------ Windows --------------------------
		modelWindows = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelWindows));
		//Windows.Draw(lightingShader);
		glDisable(GL_BLEND);

		//			 ------------------------------ Doors --------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Doors.Draw(lightingShader);

				//			 ------------------------------ Arbol --------------------------
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(-48.0f, -1.0f, 20.0f));  // Asegúrate de que esté dentro de la vista de la cámara
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelWindows2 = glm::rotate(modelWindows2, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tree1.Draw(lightingShader);
		

		//			 ------------------------------ Piso --------------------------
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 0.3f, 1.0f));  // Escala ajustada
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Floor.Draw(lightingShader);



		//		-------------------- Oso ------------------------
		modelBody = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp = modelBody = glm::translate(modelBody, glm::vec3(BearPosX, BearPosY, BearPosZ));
		//model = glm::translate(model, glm::vec3(-8.302f, 0.193f, -1.388f));
		modelTemp = modelBody = glm::rotate(modelBody, glm::radians(rotBearX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBody));
		bearBody.Draw(lightingShader);
		//Head
		glm::mat4 modelHead = modelTemp;
		//model = glm::translate(model, glm::vec3(-9.286f, 0.247f, -1.308f));
		modelHead = glm::rotate(modelHead, glm::radians(head), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHead));
		HeadBear.Draw(lightingShader);
		//Front left Leg
		glm::mat4 modelFL = modelTemp;
		//model = glm::translate(model, glm::vec3(-8.666f, 0.022f, -1.75f));
		modelFL = glm::rotate(modelFL, glm::radians(FLegsI), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelFL = glm::rotate(modelFL, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelFL = glm::rotate(modelFL, glm::radians(FLegsD), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFL));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		glm::mat4 modelFR = modelTemp;
		//model = glm::translate(model, glm::vec3(-8.681f, -0.051f, -1.053f));
		modelTemp = glm::rotate(modelTemp, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = glm::rotate(modelTemp, glm::radians(FLegsD), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTemp));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		glm::mat4 modelBL = modelTemp;
		//model = glm::translate(model, glm::vec3(-7.83f, -0.034, -1.739));
		modelBL = glm::rotate(modelBL, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBL = glm::rotate(modelBL, glm::radians(RLegsAux), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBL));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		glm::mat4 modelBR = modelTemp;
		//model = glm::translate(model, glm::vec3(-7.58f, 0.138f, -1.035f));
		modelBR = glm::rotate(modelBR, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBR = glm::rotate(modelBR, glm::radians(RLegsAux), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBR));
		B_RightLeg.Draw(lightingShader);


		//model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		//model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Ball.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		//Draw Skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyBoxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &skyBoxVAO);
	glDeleteBuffers(1, &skyBoxVAO);



	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Save
	if (keys[GLFW_KEY_C]) {
		saveKeyframesToFile("animacion_guardada.txt");
	}

	if (keys[GLFW_KEY_X]) {
		loadKeyframesFromFile("animacion_guardada.txt");
		resetElements();
		interpolation();
	}

	// Activa o desactiva el avance circular
	if (keys[GLFW_KEY_M]) {
		moveInCircle = !moveInCircle;
	}



	////Dog Controls
	if (keys[GLFW_KEY_1])
	{
		circularAngle += angularSpeed;  

		BearPosX -= 0.001f; // Posición en X
		BearPosZ -= 0.001f; // Posición en Z

		rotBear += 0.09f;
		FLegs = 15.0f * sin(rotBear * 0.05f); // Mueve las patas delanteras
		RLegs = 15.0f * sin(rotBear * 0.05f);  // Mueve las patas traseras
		// Mantener circularAngle en el rango [0, 2π]
		if (circularAngle > 0.5f * 3.0f) {
			circularAngle -= 0.5f * 3.0f;
		}
	}

	if (keys[GLFW_KEY_Q])
	{
		circularAngle += angularSpeed;  // Incrementa el ángulo para el movimiento circular
		//dogPosX = RADIUS * cos(circularAngle); // Posición en X
		//dogPosZ = RADIUS * sin(circularAngle); // Posición en Z

		BearPosX += 0.001f; // Posición en X
		BearPosZ += 0.001f; // Posición en Z

		rotBear += 0.09f;
		FLegs = 15.0f * sin(rotBear * 0.05f); // Mueve las patas delanteras
		RLegs = 15.0f * sin(rotBear * 0.05f);  // Mueve las patas traseras
		// Mantener circularAngle en el rango [0, 2π]
		if (circularAngle > 0.5f * 3.0f) {
			circularAngle -= 0.5f * 3.0f;
		}
	}



	if (keys[GLFW_KEY_2])
	{

		rotBearX += 0.1f;
	}

	if (keys[GLFW_KEY_3])
	{

		rotBearX -= 0.1f;
	}

	if (keys[GLFW_KEY_4])
	{

		head += 0.1f;

	}

	if (keys[GLFW_KEY_5])
	{

		head -= 0.1f;

	}

	
	

	if (keys[GLFW_KEY_9])
	{

		FLegsD += 0.1f;
		//FLegs += 0.1f;

	}

	if (keys[GLFW_KEY_0])
	{

		//FLegs -= 0.1f;
		FLegsD -= 0.1f;


	}

	if (keys[GLFW_KEY_U])
	{

		FLegsI += 0.1f;
		//FLegs += 0.01f;

	}

	if (keys[GLFW_KEY_I])
	{

		//FLegs -= 0.1f;
		FLegsI -= 0.1f;


	}

	if (keys[GLFW_KEY_O])
	{

		RLegs += 0.1f;

	}

	if (keys[GLFW_KEY_P])
	{

		RLegs -= 0.1f;

	}




	if (keys[GLFW_KEY_H])
	{
		BearPosZ += 0.1f;
	}

	if (keys[GLFW_KEY_Y])
	{
		BearPosZ -= 0.1f;
	}

	if (keys[GLFW_KEY_G])
	{
		BearPosX -= 0.1f;
	}

	if (keys[GLFW_KEY_J])
	{
		BearPosX += 0.1f;
	}

	if (keys[GLFW_KEY_B])
	{
		BearPosY -= 0.1f;
	}

	if (keys[GLFW_KEY_N])
	{
		BearPosY += 0.1f;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.001f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.001f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.001f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.001f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.001f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.001f;
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}



	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuando es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}


}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 3)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				//i_curr_steps = 0;   // Reset step counter
				play = false;


			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			BearPosX += KeyFrame[playIndex].incX;
			BearPosY += KeyFrame[playIndex].incY;
			BearPosZ += KeyFrame[playIndex].incZ;
			head += KeyFrame[playIndex].headInc;
			FLegs += KeyFrame[playIndex].FLegsInc;
			RLegs += KeyFrame[playIndex].RLegsInc;

			FLegsI += KeyFrame[playIndex].FLegsIInc;
			FLegsD += KeyFrame[playIndex].FLegsDInc;
			RLegsAux += KeyFrame[playIndex].RLegsAuxInc;
			rotBear += KeyFrame[playIndex].rotBearInc;
			rotBearX += KeyFrame[playIndex].rotBearXInc;
			i_curr_steps++;
		}
	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}





