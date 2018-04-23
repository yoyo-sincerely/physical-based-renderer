#include "shader.h"
#include "camera.h"
#include "model.h"
#include "shape.h"
#include "texture.h"
#include "light.h"
#include "skybox.h"
#include "material.h"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/renderer/the_renderer.h"

#include <stdio.h>

#include <direct.h>
#include<iostream>
#include<string>

#define GetCurrentDir _getcwd

using namespace PBR;

Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));

bool showDemoWindow = false;
bool showRendererWindow = false;
bool showInspectorWindow = false;


//---------------------------------
// Variables & objects declarations
//---------------------------------

GLuint WIDTH = 1280;
GLuint HEIGHT = 720;

GLuint screenQuadVAO, screenQuadVBO;
GLuint gBuffer, zBuffer, gPosition, gNormal, gAlbedo, gEffects;
GLuint saoFBO, saoBlurFBO, saoBuffer, saoBlurBuffer;
GLuint postprocessFBO, postprocessBuffer;
GLuint envToCubeFBO, irradianceFBO, prefilterFBO, brdfLUTFBO, envToCubeRBO, irradianceRBO, prefilterRBO, brdfLUTRBO;

GLint gBufferView = 1;
GLint tonemappingMode = 1;
GLint lightDebugMode = 3;
GLint attenuationMode = 2;
GLint saoSamples = 12;
GLint saoTurns = 7;
GLint saoBlurSize = 4;
GLint motionBlurMaxSamples = 32;

GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat deltaGeometryTime = 0.0f;
GLfloat deltaLightingTime = 0.0f;
GLfloat deltaSAOTime = 0.0f;
GLfloat deltaPostprocessTime = 0.0f;
GLfloat deltaForwardTime = 0.0f;
GLfloat deltaGUITime = 0.0f;
GLfloat materialRoughness = 0.01f;
GLfloat materialMetallicity = 0.02f;
GLfloat ambientIntensity = 0.005f;
GLfloat saoRadius = 0.3f;
GLfloat saoBias = 0.001f;
GLfloat saoScale = 0.7f;
GLfloat saoContrast = 0.8f;
GLfloat lightPointRadius1 = 3.0f;
GLfloat lightPointRadius2 = 3.0f;
GLfloat lightPointRadius3 = 3.0f;
GLfloat cameraAperture = 16.0f;
GLfloat cameraShutterSpeed = 0.5f;
GLfloat cameraISO = 1000.0f;
GLfloat modelRotationSpeed = 0.0f;

bool cameraMode;
bool pointMode = false;
bool directionalMode = false;
bool iblMode = true;
bool saoMode = false;
bool fxaaMode = false;
bool motionBlurMode = false;
bool screenMode = false;
bool firstMouse = true;
bool guiIsOpen = true;
bool keys[1024];

glm::vec3 albedoColor = glm::vec3(1.0f);
glm::vec3 materialF0 = glm::vec3(0.04f);  // UE4 dielectric
glm::vec3 lightPointPosition1 = glm::vec3(1.5f, 0.75f, 1.0f);
glm::vec3 lightPointPosition2 = glm::vec3(-1.5f, 1.0f, 1.0f);
glm::vec3 lightPointPosition3 = glm::vec3(0.0f, 0.75f, -1.2f);
glm::vec3 lightPointColor1 = glm::vec3(1.0f);
glm::vec3 lightPointColor2 = glm::vec3(1.0f);
glm::vec3 lightPointColor3 = glm::vec3(1.0f);
glm::vec3 lightDirectionalDirection1 = glm::vec3(-0.2f, -1.0f, -0.3f);
glm::vec3 lightDirectionalColor1 = glm::vec3(1.0f);
glm::vec3 modelPosition = glm::vec3(0.0f);
glm::vec3 modelRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 modelScale = glm::vec3(0.1f);

glm::mat4 projViewModel;
glm::mat4 prevProjViewModel = projViewModel;
glm::mat4 envMapProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 envMapView[] =
{
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

//Shader gBufferShader;
//Shader latlongToCubeShader;
Shader simpleShader;
//Shader lightingBRDFShader;
//Shader irradianceIBLShader;
//Shader prefilterIBLShader;
//Shader integrateIBLShader;
//Shader firstpassPPShader;
//Shader saoShader;
//Shader saoBlurShader;

Texture objectAlbedo;
Texture objectNormal;
Texture objectRoughness;
Texture objectMetalness;
Texture objectAO;
Texture envMapHDR;
Texture envMapCube;
Texture envMapIrradiance;
Texture envMapPrefilter;
Texture envMapLUT;

Material pbrMat;

Model objectModel;

Light lightPoint1;
Light lightPoint2;
Light lightPoint3;
Light lightDirectional1;

Shape quadRender;
Shape envCubeRender;

//---------------
// GLFW Callbacks
//---------------
static void errorCallback(int error, const char * description);
void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow * window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);

//---------------
// setting
//---------------
void imguiSetup();
void cameraMove();
void gBufferSetup();
void saoSetup();
void postprocessSetup();
void iblSetup();

std::string getCurrentWorkingDir(void);

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

#if FULL_SCREEN
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Awesome Renderer", monitor, NULL);
#else
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Awesome Renderer", NULL, NULL);
#endif

    glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1); // Enable vsync

	//gl init
	gladLoadGL();

    //glViewport(0, 0, WIDTH, HEIGHT);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

	//window callback
    ImGui_ImplGlfwGL3_Init(window, true);
	glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::cout << "Current Working Dir is : " + getCurrentWorkingDir() << std::endl;

	//----------
   	// Shader(s)
   	//----------
   	simpleShader.setShader("../resources/shaders/lighting/simple.vert", "../resources/shaders/lighting/simple.frag");

   	//-----------
   	// Textures(s)
   	//-----------
   	objectAlbedo.setTexture("../resources/textures/pbr/rustediron/rustediron_albedo.png", "ironAlbedo", true);
   	objectNormal.setTexture("../resources/textures/pbr/rustediron/rustediron_normal.png", "ironNormal", true);
   	objectRoughness.setTexture("../resources/textures/pbr/rustediron/rustediron_roughness.png", "ironRoughness", true);
   	objectMetalness.setTexture("../resources/textures/pbr/rustediron/rustediron_metalness.png", "ironMetalness", true);
   	objectAO.setTexture("../resources/textures/pbr/rustediron/rustediron_ao.png", "ironAO", true);

   	envMapHDR.setTextureHDR("../resources/textures/hdr/appart.hdr", "appartHDR", true);

   	envMapCube.setTextureCube(512, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
   	envMapIrradiance.setTextureCube(32, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR);
   	envMapPrefilter.setTextureCube(128, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
   	envMapPrefilter.computeTexMipmap();
   	envMapLUT.setTextureHDR(512, 512, GL_RG, GL_RG16F, GL_FLOAT, GL_LINEAR);

   	//---------
   	// Model(s)
   	//---------
   	objectModel.loadModel("../resources/models/shaderball/shaderball.obj");

   	//---------------
   	// Shape(s)
   	//---------------
   	envCubeRender.setShape("cube", glm::vec3(0.0f));
   	quadRender.setShape("quad", glm::vec3(0.0f));

   	//----------------
   	// Light source(s)
   	//----------------
   	lightPoint1.setLight(lightPointPosition1, glm::vec4(lightPointColor1, 1.0f), lightPointRadius1, true);
   	lightPoint2.setLight(lightPointPosition2, glm::vec4(lightPointColor2, 1.0f), lightPointRadius2, true);
   	lightPoint3.setLight(lightPointPosition3, glm::vec4(lightPointColor3, 1.0f), lightPointRadius3, true);

   	lightDirectional1.setLight(lightDirectionalDirection1, glm::vec4(lightDirectionalColor1, 1.0f));

	
    //---------------
    // G-Buffer setup
    //---------------
    gBufferSetup();


    //------------
    // SAO setup
    //------------
    saoSetup();


    //---------------------
    // Postprocessing setup
    //---------------------
    postprocessSetup();


    //----------
    // IBL setup
    //----------
    iblSetup();

    //------------------------------
    // Queries setting for profiling
    //------------------------------
    GLuint64 startGeometryTime, startLightingTime, startSAOTime, startPostprocessTime, startForwardTime, startGUITime;
    GLuint64 stopGeometryTime, stopLightingTime, stopSAOTime, stopPostprocessTime, stopForwardTime, stopGUITime;

    unsigned int queryIDGeometry[2];
    unsigned int queryIDLighting[2];
    unsigned int queryIDSAO[2];
    unsigned int queryIDPostprocess[2];
    unsigned int queryIDForward[2];
    unsigned int queryIDGUI[2];

    glGenQueries(2, queryIDGeometry);
    glGenQueries(2, queryIDLighting);
    glGenQueries(2, queryIDSAO);
    glGenQueries(2, queryIDPostprocess);
    glGenQueries(2, queryIDForward);
    glGenQueries(2, queryIDGUI);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
		GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		glfwPollEvents();
		cameraMove();

		//setting
		imguiSetup();

		//------------------------
        // Geometry Pass rendering
        //------------------------
        glQueryCounter(queryIDGeometry[0], GL_TIMESTAMP);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        screenMode = !screenMode;


    }

    if (keys[GLFW_KEY_1])
        gBufferView = 1;

    if (keys[GLFW_KEY_2])
        gBufferView = 2;

    if (keys[GLFW_KEY_3])
        gBufferView = 3;

    if (keys[GLFW_KEY_4])
        gBufferView = 4;

    if (keys[GLFW_KEY_5])
        gBufferView = 5;

    if (keys[GLFW_KEY_6])
        gBufferView = 6;

    if (keys[GLFW_KEY_7])
        gBufferView = 7;

    if (keys[GLFW_KEY_8])
        gBufferView = 8;

    if (keys[GLFW_KEY_9])
        gBufferView = 9;

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}


void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (cameraMode)
        camera.mouseCall(xoffset, yoffset);
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        cameraMode = true;
    else
        cameraMode = false;
}


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (cameraMode)
        camera.scrollCall(yoffset);
}

void imguiSetup() 
{
	ImGui_ImplGlfwGL3_NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		//if (ImGui::BeginMenu("Edit"))
		//{
		//	if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		//	if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
		//	ImGui::Separator();
		//	if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		//	if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		//	if (ImGui::MenuItem("Paste", "CTRL+V")) {}
		//	ImGui::EndMenu();
		//}
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Demo", NULL, &showDemoWindow);
			ImGui::MenuItem("Renderer", NULL, &showRendererWindow);
			//ImGui::MenuItem("Attribute", NULL, &show_inspector_window);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

	}

    if (showDemoWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

	if (showRendererWindow) 
	{
		ShowRendererWindow(&showRendererWindow);
	}

	//ImGui::End();
}

void cameraMove() 
{
    if (keys[GLFW_KEY_W])
        camera.keyboardCall(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.keyboardCall(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.keyboardCall(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.keyboardCall(RIGHT, deltaTime);
}

void gBufferSetup()
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// Position
	glGenFramebuffers(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// Albedo + Roughness
	glGenFramebuffers(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gAlbedo, 0);
}

void saoSetup()
{

}

void postprocessSetup()
{

}

void iblSetup()
{

}

std::string getCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

