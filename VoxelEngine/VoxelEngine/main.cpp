// Main code file

#pragma comment(lib, "GLFWDLL")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "glew32")

#define GLFW_DLL

#include <GL\glew.h>
#include <GL\glfw.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\constants.hpp>

#include <cstdlib>
#include <sstream>
#include <iostream>

#include "Block.h"
#include "Camera.h"
#include "Chunk.h"
#include "EntityManager.h"
#include "Projectile.h"
#include "Shader.h"

// Screen dimensions
static const int SCREEN_WIDTH  = 800;
static const int SCREEN_HEIGHT = 600;
std::string windowTitle = "Voxel Engine v0.2";

bool running = GL_TRUE;	// Flag to indicate if program is running

bool hasCollided = false;

// World information
// Create new chunk
Chunk* chunk = new Chunk();

// Shader variables
GLuint shaders[2];
GLuint program;

GLint MVPuniform;
GLint MITuniform;
GLint diffuseMaterialUniform;
GLint diffuseLightUniform;
GLint lightDirUniform;

glm::mat4 projection;
float lightAngle = 0.5f;

// Entities
EntityManager* entityManager;
Camera* camera;
Projectile* projectile;

// FPS Counter
// Taken from http://r3dux.org/2012/07/a-simple-glfw-fps-counter/
// Very handy
double calcFPS(double theTimeInterval = 1.0, std::string theWindowTitle = "NONE")
{
	// Static values which only get initialised the first time the function runs
	static double t0Value       = glfwGetTime(); // Set the initial time to now
	static int    fpsFrameCount = 0;             // Set the initial FPS frame count to 0
	static double fps           = 0.0;           // Set the initial FPS value to 0.0
 
	// Get the current time in seconds since the program started (non-static, so executed every time)
	double currentTime = glfwGetTime();
 
	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
	if (theTimeInterval < 0.1)
	{
		theTimeInterval = 0.1;
	}
	if (theTimeInterval > 10.0)
	{
		theTimeInterval = 10.0;
	}
 
	// Calculate and display the FPS every specified time interval
	if ((currentTime - t0Value) > theTimeInterval)
	{
		// Calculate the FPS as the number of frames divided by the interval in seconds
		fps = (double)fpsFrameCount / (currentTime - t0Value);
 
		// If the user specified a window title to append the FPS value to...
		if (theWindowTitle != "NONE")
		{
			// Convert the fps value into a string using an output stringstream
			std::ostringstream stream;
			stream << fps;
			std::string fpsString = stream.str();
 
			// Append the FPS value to the window title details
			theWindowTitle += " | FPS: " + fpsString;
 
			// Convert the new window title to a c_str and set it
			const char* pszConstString = theWindowTitle.c_str();
			glfwSetWindowTitle(pszConstString);
		}
		else // If the user didn't specify a window to append the FPS to then output the FPS to the console
		{
			std::cout << "FPS: " << fps << std::endl;
		}
 
		// Reset the FPS frame counter and set the initial time to be now
		fpsFrameCount = 0;
		t0Value = glfwGetTime();
	}
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
	{
		fpsFrameCount++;
	}
 
	// Return the current FPS - doesn't have to be used if you don't want it!
	return fps;
}

void initialise()
{
	// Set the window title
	const char* titleAsChar = windowTitle.c_str();
	glfwSetWindowTitle(titleAsChar);

	// Set BG colour to mid grey
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Initilise entity manager
	entityManager = new EntityManager();

	// Create & set up camera
	camera = new Camera();
	camera->setProjection(glm::pi<float>() / 4.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 2.414f, 10000.0f);
	camera->setPosition(glm::vec3(60.0f, 40.0f, 60.0f));
	camera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

	// Create projectile
	projectile = new Projectile(camera->getPosition(), camera->getTarget() - camera->getPosition(), 0.05f);
	entityManager->registerEntity(projectile);

	// Set the projection matrix
	projection = camera->getProjection();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);	// Enable depth testing
	glEnable(GL_CULL_FACE);		// Enable backface culling

	// Load shaders
	shaders[0] = loadShader("./Shaders/diffuse.vert", GL_VERTEX_SHADER);
	shaders[1] = loadShader("./Shaders/diffuse.frag", GL_FRAGMENT_SHADER);
	if (shaders[0] && shaders[1])
	{
		program = createProgram(shaders, 2);
		if (!program)
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_FAILURE);

	// Graphics Programming shader
	MVPuniform = glGetUniformLocation(program, "modelViewProjection");
	MITuniform = glGetUniformLocation(program, "modelInverseTranspose");
	diffuseMaterialUniform = glGetUniformLocation(program, "diffuseMaterial");
	diffuseLightUniform = glGetUniformLocation(program, "diffuseLight");
	lightDirUniform = glGetUniformLocation(program, "lightDir");

	// Generate the mesh for the chunk
	chunk->createMesh();
}

// Cleanup method - tidies up objects from memory
void clean()
{
	delete chunk;
	delete camera;
	delete projectile;
	delete entityManager;
}

void update(double deltaTime)
{
	// Update camera
	camera->update(deltaTime);

	// Kills program if ESC pressed
	// "running" flag set to false
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

	// Switch between cube & sphere
	if (glfwGetKey('X'))
		chunk->setupSphere();
	if (glfwGetKey('C'))
		chunk->setupCube();

	// Movement controls
	// Strafe
	if (glfwGetKey('W'))
		camera->move(glm::vec3( 0.0f, 0.0f,  5.0f) * (float)deltaTime);
	if (glfwGetKey('S'))
		camera->move(glm::vec3( 0.0f, 0.0f, -5.0f) * (float)deltaTime);
	if (glfwGetKey('A'))
		camera->move(glm::vec3( 5.0f, 0.0f,  0.0f) * (float)deltaTime);
	if (glfwGetKey('D'))
		camera->move(glm::vec3(-5.0f, 0.0f,  0.0f) * (float)deltaTime);

	// Turn
	if (glfwGetKey(GLFW_KEY_LEFT))
		camera->rotate(glm::pi<float>() * deltaTime, 0.0f);
	if (glfwGetKey(GLFW_KEY_RIGHT))
		camera->rotate(-glm::pi<float>() * deltaTime, 0.0f);

	// Pitch
	if (glfwGetKey(GLFW_KEY_UP))
		camera->rotate(0.0f, glm::pi<float>() * deltaTime);
	if (glfwGetKey(GLFW_KEY_DOWN))
		camera->rotate(0.0f, -glm::pi<float>() * deltaTime);

	// Per voxel collision
	// 
	if (projectile->isAlive())
	{
		if (chunk->isColliding(projectile->getPosition()))
		{
			chunk->removeBlock((int)projectile->getPosition().x, (int)projectile->getPosition().y, (int)projectile->getPosition().z);
			projectile->kill();
		}
	}

	// Calculate direction for projectile
	glm::vec3 direction = camera->getTarget() - camera->getPosition();

	// Launch "new" projectile when SPACE is pressed
	// When old projectile is dead, move projectile to camera position
	// Set status to alive
	if (glfwGetKey(GLFW_KEY_SPACE))
	{
		if (!projectile->isAlive())
		{
			projectile->setPosition(camera->getPosition());
			projectile->setDirection(direction);
			projectile->setAlive(true);
		}
	}

	// Update chunk and entity manager
	chunk->update(deltaTime);
	entityManager->update(deltaTime);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get view matrix from camera
	glm::mat4 view = camera->getView();

	// Use shader program
	glUseProgram(program);

	// Generate model matrix
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 model = translate * rotate;

	// Calculate model view projection & assign to MVPUniform
	glm::mat4 mvp = projection * view * model;
	glUniformMatrix4fv(MVPuniform, 1, GL_FALSE, glm::value_ptr(mvp));

	// Create the model inverse transpose matrix and assign it to the uniform
	glm::mat4 mit = glm::inverse(glm::transpose(model));
	glUniformMatrix4fv(MITuniform, 1, GL_FALSE, glm::value_ptr(mit));

	// Set diffuse material & light uniforms
	glUniform4fv(diffuseMaterialUniform, 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	glUniform4fv(diffuseLightUniform, 1, glm::value_ptr(glm::vec4(1.0f, 0.0, 0.0, 1.0f)));

	// Calculate light direction vector by assuming angle rotated around Y-axis
	glm::vec3 lightDirection = glm::vec3(-cosf(lightAngle), 0.0f, -sinf(lightAngle));
	// Currently light direction goes from the centre outwards.  Therefore, we invert it
	glUniform3fv(lightDirUniform, 1, glm::value_ptr(-lightDirection));

	// Call chunk & entity render methods
	chunk->render();
	entityManager->render();

	// Stop using shader program
	glUseProgram(0);
	
	// Load identity matrix
	glLoadIdentity();

	calcFPS(1.0, windowTitle);	// Calculate framerate every second

	glfwSwapInterval(0);	// Disable vsync
	glfwSwapBuffers();
}

int main()
{
	// Initialise GLFW
	// If GLFW has failed to initialise, exit
	if (!glfwInit())
		exit(EXIT_FAILURE);

	// Open a window
	if (!glfwOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	initialise();	// Call initialise method

	double prevTimeStamp = glfwGetTime();
	double currentTimeStamp;
	while (running)
	{
		currentTimeStamp = glfwGetTime();
		update(currentTimeStamp - prevTimeStamp);
		render();
		prevTimeStamp = currentTimeStamp;
	}

	clean();	// Call cleanup method

	// Exit
	glfwTerminate();
	exit(EXIT_SUCCESS);
}