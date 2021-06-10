///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Graphics.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Graphics
//      - Initialize
//      - InitGLFW
//      - InitGlew
//      - InitGlState
//      - ~Graphics
//      - Update
//      - Render
//      - Draw
//      - handleMode
//      - UpdateWindowTitle
//      - ScreenToWorld
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
///----------------------------------INCLUDES-----------------------------------
//Extentions in use for graphics
#include "stdafx.h"
#include "Graphics.h"
#include "Spaces.h"
#include "Callbacks.h"
#include "Transform.h"
#include "ObjManager.h"
#include "ScreenManager.h"
#include "GameStateManager.h"
#include "Engine.h"
#include "Debug.h"
#include "FrameRateController.h"
#include "Camera.h"
#include "PeepHole.h"
#include "ParticleEmitter.h"
#include "Editor.h"

///----------------------------------GLOBALS-----------------------------------
extern FrameData stats;
static ObjectList objList;
#ifdef _DEBUG
WindowMode Graphics::windowMode_ = Maximized;
#else
WindowMode Graphics::windowMode_ = FullScreen;
#endif // _DEBUG


GLFWwindow * Graphics::window_ = nullptr;
bool Graphics::isDirty_ = true;
int Graphics::fbWidth_ = 1920;
int Graphics::fbHeight_ = 1080;
GLdouble Graphics::fps = 60;
float Graphics::time_;

static bool init = true;
GameStateManager* gsm;
///---------------------------------FUNCTIONS-----------------------------------

///---------------------------------------
/// Brief: Default Constructor to 
/// construct a system
Graphics::Graphics() : System("Graphics")
{
}

///----------------------------------------
/// Breif: Initializes the graphics system
void Graphics::Initialize()
{
  // Init GLFW
  InitGLFW();
  // Init Glew and Check For error
  if (GL_FALSE == InitGlew())
  {
    glfwDestroyWindow(window_);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  // Init OpenGL
  InitGlState();

  BuildShaders();
}

///---------------------------------------
/// Brief: Initializes GLFW with a window
/// and callbacks
void Graphics::InitGLFW()
{
  //  Initialize GLFW
  if (!glfwInit())
  {
    TraceMessage(GraphicsLog, "GLFW Failed to Initialize");
    std::exit(EXIT_FAILURE);
  }

  // OpenGL Version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // Modern OpenGL
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // set window to be visable
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  // set window to rezisable
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  // set window to start maximized
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  // Tell GLFW Whether the window has a border
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

  // Create the window
  window_ = glfwCreateWindow(fbWidth_, fbHeight_, frameTitle.c_str(), NULL, NULL);
  // set active window
  glfwMakeContextCurrent(window_);
  glfwShowWindow(window_);
  /*const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwSetWindowMonitor(window_, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);*/

  // If window was failed to craeted exit 
  if (window_ == NULL)
  {
    TraceMessage(GraphicsLog, "Failed to create GLFW window");
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  //setting that the mouse is visible 
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  
}

///------------------------------------------
/// Brief: Sets defsault background color
/// and blend modes
void Graphics::InitGlState()
{
  //Default blank background color for now
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

///------------------------------------------
///Author: Thornton Stockwin
///Brief: creates the openGL context, sets version of openGL
///       for use, sets core functionality
GLboolean Graphics::InitGlew()
{
  // Init Glew
  GLenum err = glewInit();

  // Set version and check for error while do so
  if (GLEW_OK != err)
  {
    std::cerr << "Unable to initialize GLEW - error: " << glewGetErrorString(err) << std::endl;
    std::exit(EXIT_FAILURE);
    return GL_FALSE;
  }
  if (GLEW_VERSION_3_3)
  {
    TraceMessage(GraphicsLog, "\tUsing glew %s", glewGetString(GLEW_VERSION));
    TraceMessage(GraphicsLog, "Driver supports OpenGL 3.3\nDetails:");
    return GL_TRUE;
  }
  else
  {
    TraceMessage(GraphicsLog, "\tUsing glew ", glewGetString(GLEW_VERSION));
    return GL_FALSE;
  }
}

void Graphics::SetCallbacks()
{
  // Window Resizing Callback
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCB);

  // Keyboard callback
  glfwSetKeyCallback(window_, InputCB);

  // Mouse Scroll callback
  glfwSetScrollCallback(window_, MouseScrollCB);

  // Mouse Button callback
  glfwSetMouseButtonCallback(window_, MouseButtonCB);
}

///------------------------------------------
///Brief: Shutdown graphics also dleetes the
/// window
Graphics::~Graphics()
{
  // Shutdown the window
  glfwTerminate();
}

///-------------------------------------
/// Brief: checks glfw events to see if 
/// window should close and updates
/// framebuffer, window title, window
/// mode and draws everything
void Graphics::Update(float dt)
{
  // Update frame buffer size
  glfwGetFramebufferSize(window_, &fbWidth_, &fbHeight_);

  //Update The title
  UpdateWindowTitle(window_);

  // Change window mode if neccessary
  handleMode();

  // After all updates have occured Draw Everything
  Draw();
}

///------------------------------------------
/// Brief: Takes a game Object and sends
/// everything that is neccessary to render
/// it to the shader
void Graphics::Render(Obj &gameObject)
{
  //Set how much of the window the world takes up
  glViewport(0, 0, Graphics::GetWidth(), fbHeight_);

  gameObject.Render();
}

///----------------------------------------
/// Breif: Collects all the Object Lists
/// and renders each object in them
/// also renders editor
void Graphics::Draw()
{
  // Get all Objects to draw them on the screen
  //objList = Space::GetObjList();
  ScreenManager *sMan = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
  objList = sMan->CurrentObjList();
  Debug* debug = reinterpret_cast<Debug*>(EngineGetSystem("Debug"));

  // Loop through all object lists and editor to draw them
  for (Obj* obj : objList)
  {
    Render(*obj);
  }

  if (init)
  {
    gsm = (GameStateManager*)EngineGetSystem("GameStateManager");
    init = false;
  }

  if ((gsm->GetCurrentGameState()) == GameGs)
  {
    Render(Space::GetPlayer());

      //Added by Sonya for making certain particles appear over darkness
    if (ScreensActive())
    {
      ObjectList& pLast = Space::GetPartsLastList(sMan);
      for (Obj *obj : pLast)
        if(obj)
          for (Obj *subObj : obj->GetSubObjList())
            if(subObj)
              if (obj->IsActive())
                Render(*subObj);
    }
  }

  Editor::Draw();

  // Swap the double buffer
  glfwSwapBuffers(window_);
  // Clear the color buffer so that image dont draw in their previous location
  glClear(GL_COLOR_BUFFER_BIT);
}

///------------------------------------------
///Author: Thornton Stockwin
///Brief: will build and compile the shader program
///       --seperating this into it's own class
void Graphics::BuildShaders()
{
  //shaderProgram_ = new Shader("Shaders/VertexShaderUI.glsl", "Shaders/FragShaderUI.glsl");
}

///--------------------------------------------------
/// Brief: with the window supllied(usually only one)
/// Updates the title on the title bar with fps
void Graphics::UpdateWindowTitle(GLFWwindow* window)
{
  // string to hold the title
  std::string title;
  // bool to tel if fps should be shown
  bool fps_ = reinterpret_cast<Editor*>(EngineGetSystem("Editor"))->GetActive();
  double delta_time = update_time(1.0f);
  // add preset title to the new title
  title += frameTitle;
  if (1)
  {
    char frame[24];
    sprintf(frame, "%f", fps);
    title += frame;
  }
  // tell glfw to set title to new title
  glfwSetWindowTitle(window, title.c_str());
}

///---------------------------------------------
/// Brief: Switch the window to the correct size
/// baded on the mode it is put in
void Graphics::handleMode()
{
  if (isDirty_)
  {
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (windowMode_ == FullScreen)
    {
      glfwSetWindowMonitor(window_, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    if (windowMode_ == Maximized)
    {
	  glfwSetWindowMonitor(window_, NULL, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    if (windowMode_ == Secret)
    {
      int width;
      int height;
      glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &width, &height);
      glfwSetWindowMonitor(window_, glfwGetPrimaryMonitor(), 0, 0, width, height, mode->refreshRate);
    }
    isDirty_ = false;
  }
}

///------------------------------------------
/// Brief: converts coordinates in screen
/// space to world space. Exaplme mouse coords
/// to world
glm::vec2 Graphics::ScreenToWorld(glm::vec2& screenCoordinates)
{
  // convert from pixel space to ndc space
  screenCoordinates.x = (screenCoordinates.x / fbWidth_) * 2;
  screenCoordinates.y = (screenCoordinates.y / fbHeight_) * 2;

  // negate the y
  screenCoordinates -= glm::vec2(1.0f, 1.0f);
  screenCoordinates.y *= -1;

  // Muliply by inverse camera matrix to get to world space form ndc
  glm::vec3 vec3(screenCoordinates, 0);
  glm::mat3 inverse = glm::inverse(Camera::ProjectionMat());
  glm::vec3 coord = inverse * vec3;

  // return new coordinates in world space
  return coord;
}

glm::vec2 Graphics::WorldToTexture(glm::vec2& worldCoordinates, float size)
{
  glm::vec3 vec3(worldCoordinates, 0);
  vec3 = Camera::ProjectionMat() * vec3;
  vec3.y *= -1;

  //(scale - 1)
  vec3 += glm::vec3(size - 1, size - 1, 0);

  //times scale
  glm::mat3 conversion(1/2.f, 0, 0, 0, 1/2.f, 0, 0, 0, 1);
  conversion *= 1/size;
  vec3 = conversion * vec3;
  glm::mat3 translation(1, 0, 1 , 0, 1, 1 , 0, 0, 1);
  vec3 = translation * vec3;
  vec3.y = (1 - vec3.y);
  return vec3;
}

double Graphics::update_time(double fps_calc_interval) {
  // get elapsed time (in seconds) between previous and current frames
  static double prev_time = glfwGetTime();
  double curr_time = glfwGetTime();
  double delta_time = curr_time - prev_time;
  prev_time = curr_time;

  // fps calculations
  static double count = 0.0; // number of game loop iterations
  static double start_time = glfwGetTime();
  // get elapsed time since very beginning (in seconds) ...
  double elapsed_time = curr_time - start_time;

  ++count;

  // update fps at least every 10 seconds ...
  fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
  fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
  if (elapsed_time > fps_calc_interval) {
    fps = count / elapsed_time;
    start_time = curr_time;
    count = 0.0;
  }

  // done calculating fps ...
  return delta_time;
}