#include <Windows.h>
#include <locale>
#include <codecvt>
#include <GL/glew.h>
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include <glm.hpp>
#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1000;

float skylight;
glm::vec3 lightPos = glm::vec3(3000.0f, 1000.0f, 8000.0f);

std::vector<Mesh> UnderwaterEnvironment;

unsigned int MossyRocksTex;
unsigned int CoralTreeTex;

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

void UnderwaterEnvironmentInit(std::string path) {
	Mesh mossyRocks(path + "MossyRocks.obj", path);
	UnderwaterEnvironment.push_back(mossyRocks);

	Mesh starFish(path + "StarFish.obj", path);
	UnderwaterEnvironment.push_back(starFish);

	Mesh coralTree(path + "CoralTree.obj", path);
	UnderwaterEnvironment.push_back(coralTree);

	Mesh coral(path + "Coral.obj", path);
	UnderwaterEnvironment.push_back(coral);

	//Mesh coral1(path + "Coral1.obj", path);
	//UnderwaterEnvironment.push_back(coral1);

	//Mesh coral2(path + "Coral2.obj", path);
	//UnderwaterEnvironment.push_back(coral2);

	//Mesh coral3(path + "Coral3.obj", path);
	//UnderwaterEnvironment.push_back(coral3);

	//Mesh coral4(path + "Coral4.obj", path);
	//UnderwaterEnvironment.push_back(coral4);

	//Mesh coral5(path + "Coral5.obj", path);
	//UnderwaterEnvironment.push_back(coral5);
	//
	//Mesh coral6(path + "Coral6.obj", path);
	//UnderwaterEnvironment.push_back(coral6);

	for (int i = 0; i < UnderwaterEnvironment.size(); i++)
	{
		if (i == 0)
		{
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -7.f, 10.f));
		}
		else if (i == 1) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -4.f, 10.f));
		}
		else if (i == 2) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -10.f, 10.f));
		}
		else if (i == 3) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -9.f, 10.f));
		}
		/*else if (i == 4) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -9.f, 10.f));
		}
		else if (i == 5) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -3.5f, 10.f));
		}
		else if (i == 6) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -4.8f, 10.f));
		}
		else if (i == 7) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -3.f, 10.f));
		}
		else if (i == 8) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -1.f, 10.f));
		}
		else if (i == 9) {
			UnderwaterEnvironment[i].setPosition(glm::vec3(10.f, -8.5f, 10.f));
		}*/
		UnderwaterEnvironment[i].setScale(glm::vec3(5.f));
		UnderwaterEnvironment[i].initVAO();
	}

	MossyRocksTex = CreateTexture(path + "Textures\\MossyRocksTex.png");
	CoralTreeTex = CreateTexture(path + "Textures\\CoralTreeTex.jpg");

	//GrindaTex = CreateTexture(path + "Resources\\Shelter_simple_frame.bmp");
}

static void UnderwaterRender(Shader& shader1, Shader shader2) {
	shader1.Activate();
	glBindTexture(GL_TEXTURE_2D, MossyRocksTex);
	UnderwaterEnvironment[0].render(&shader2);

	shader2.Activate();
	UnderwaterEnvironment[2].setScale(glm::vec3(0.5f));
	glBindTexture(GL_TEXTURE_2D, CoralTreeTex);
	UnderwaterEnvironment[1].render(&shader2);

	for (int i = 0; i < UnderwaterEnvironment.size(); i++) {
		UnderwaterEnvironment[i].render(&shader2);
	}
}

Camera* pCamera = nullptr;
bool leftpressed = false;
bool rightpressed = false;
bool UpPressed = false;
bool DownPressed = false;
bool SpacePressed = false;
bool ShiftPressed = false;


//bool cursor = true;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void Cleanup()
{
	delete pCamera;
}
void processInput(GLFWwindow* window, Camera* pCamera, double deltaTime, Mesh* Player)
{
	float movement = 0.1f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		UpPressed = true;
	}
	else
	{
		UpPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		DownPressed = true;
	}
	else
	{
		DownPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		leftpressed = true;
	}
	else
	{
		leftpressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		rightpressed = true;
	}
	else
	{
		rightpressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		SpacePressed = true;
	}
	else
	{
		SpacePressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		ShiftPressed = true;
	}
	else
	{
		ShiftPressed = false;
	}
}
int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Submarine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
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
	// first, configure the cube's VAO (and VBO)
	unsigned int cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create camera
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 2.0));



	std::string SubmarinePath = "..\\..\\Models\\submarine\\";
	std::string MapPath = "..\\..\\Models\\Map\\";
	std::string UnderwaterPath = "..\\..\\Models\\Underwater_Objects\\";


	//Shaders
	Shader programShader("default.vs", "default.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader lampShader("Lamp.vs", "Lamp.fs");
	Shader terrainShader("terrain.vs", "terrain.fs");


	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);


	terrainShader.Activate();
	terrainShader.SetVec3("lightColor", lightColor);

	Mesh Submarine(SubmarinePath + "submarine.obj", SubmarinePath);
	Submarine.setPosition(glm::vec3(0.f));
	Submarine.setColor(0, glm::vec3(1.0f, 0.0f, 0.0f));
	Submarine.setPosition(glm::vec3(0.0f, 1.8f, -1.5f));
	Submarine.initVAO();

	Mesh Map(MapPath + "Map.obj", MapPath);
	Map.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	Map.setPosition(glm::vec3(1000.0f, -165.0f, -80.0f));
	Map.initVAO();

	// All the faces of the cubemap (make sure they are in this exact order)
	std::vector<std::string> facesCubemap =
	{
		"Skybox3.bmp",
		"Skybox4.bmp",
		"SkyboxUp.bmp",
		"SkyboxDown.bmp",
		"Skybox2.bmp",
		"Skybox1.bmp",
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture = LoadSkybox(facesCubemap);
	int floorTexture = CreateTexture(MapPath + "Map.jpg");
	int submarineTexture = CreateTexture(SubmarinePath + "Base_color.jpg");
	UnderwaterEnvironmentInit(UnderwaterPath);

	float deltaTime = 0.f;
	float lastFrame = 0.f;
	float deltaAltitude = 0.f;
	bool desprindere = false;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		double FrameStart = glfwGetTime();
		deltaTime = FrameStart - lastFrame;
		lastFrame = FrameStart;

		/*float currentAltitude = Avion.getPosition().y;*/
		//deltaAltitude = (pCamera->speed - 0.5f) * 2.f;
		//float currentTilt = Avion.getRotation().y;
		//changeHour(shader, terrainShader);

		float clearR = 0.07f + skylight / 2.f - 0.1f;
		float clearG = 0.13f + skylight / 2.f - 0.1f;
		float clearB = 0.17 + skylight / 2.f - 0.1f;
		glClearColor(clearR, clearG, clearB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		pCamera->SetPosition(glm::vec3(0.0f, 50.0f, 100.0f));

		processInput(window, pCamera, deltaTime, &Submarine);
		//if()
		pCamera->SetPosition(glm::vec3(Submarine.getPosition() + glm::vec3(-sin(glm::radians(/*Avion.getRotation().y +*/ pCamera->offset)) * 50.f, 15.0f, -cos(glm::radians(/*Avion.getRotation().y*/ + pCamera->offset)) * 50.f)));
		pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(0.0f, glm::radians((pCamera->frontTilt - 13.f) / 1.5f) * 50.f, 0.0f));
		pCamera->pitch = -pCamera->frontTilt;
		pCamera->yaw = -(/*(float)Avion.getRotation().y +*/ (float)pCamera->offset - 90.f);
		////changeHour(terrainShader, skyboxShader, programShader, airportShader);

		glm::mat4 projection = pCamera->GetProjectionMatrix();
		glm::mat4 view = pCamera->GetViewMatrix();
		pCamera->UpdateCameraVectors();

		if (UpPressed)
		{
			Submarine.setPosition(Submarine.getPosition()+ glm::vec3(
				0.4f * glm::sin(glm::radians(Submarine.getRotation().y)),
				0.0f,
				0.4f * glm::cos(glm::radians(Submarine.getRotation().y))
			));
			
		}
		if (leftpressed)
		{
			
			Submarine.setRotation(Submarine.getRotation() + glm::vec3(0.0f, 0.2f, 0.0f));
		}
		if (DownPressed)
		{
			Submarine.setPosition(Submarine.getPosition() - glm::vec3(
				0.4f * glm::sin(glm::radians(Submarine.getRotation().y)),
				0.0f,
				0.4f * glm::cos(glm::radians(Submarine.getRotation().y))
			));
		}
		if (rightpressed)
		{
			Submarine.setRotation(Submarine.getRotation() + glm::vec3(0.0f, -0.2f, 0.0f));
		}
		if (SpacePressed)
		{
			Submarine.setPosition(Submarine.getPosition() + glm::vec3(0.0f, 0.2f, 0.0f));
		}
		if (ShiftPressed)
		{
			Submarine.setPosition(Submarine.getPosition() + glm::vec3(0.0f, -0.2f, 0.0f));
		}

		lampShader.Activate();
		lampShader.SetMat4("projection", projection);
		lampShader.SetMat4("view", view);
		glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(300.f)); // a smaller cube
		lampShader.SetMat4("model", lightModel);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		programShader.Activate();
		programShader.SetVec3("lightPos", lightPos);
		programShader.SetVec3("lightColor", lightColor);
		programShader.SetVec3("viewPos", pCamera->GetPosition());

		programShader.SetMat4("projection", projection);
		programShader.SetMat4("view", view);
		//glBindTexture(GL_TEXTURE_2D, submarineTexture);
		Submarine.render(&programShader);

		terrainShader.Activate();
		terrainShader.SetMat4("projection", projection);
		terrainShader.SetMat4("view", view);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		Map.render(&terrainShader);
		glBindTexture(GL_TEXTURE_2D, 0);

		//render skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Activate();
		projection = pCamera->GetProjectionMatrix();
		view = glm::mat4(glm::mat3(pCamera->GetViewMatrix()));
		skyboxShader.SetMat4("projection", projection);
		skyboxShader.SetMat4("view", view);

		UnderwaterRender(programShader, terrainShader);

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Cleanup();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	programShader.Delete();
	skyboxShader.Delete();
	lampShader.Delete();
	terrainShader.Delete();

	// glfw: terminate, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}