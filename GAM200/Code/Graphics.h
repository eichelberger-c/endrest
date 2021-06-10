///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Graphics.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Does some graphic stuff man, Frankenstein style
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#define MAX_FPS 60
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW\glfw3.h>
#include "System.h"
#include "Obj.h"
#include "Shader.h"
///----------------------------------GLOBALS------------------------------------

///----------------------------------CLASSES------------------------------------
typedef enum WindowMode
{
  FullScreen,
  Maximized,
  Secret,
} WindowMode;

class Graphics : public System
{
public:

  Graphics();

  virtual void Initialize();

  void InitGLFW();

  GLboolean InitGlew();

  void InitGlState();

  static void SetCallbacks();

  ~Graphics();

  virtual void Update(float dt);

  static void Render(Obj &gameObject);

  void Draw();


  void BuildShaders();
  

  void handleMode();

  void UpdateWindowTitle(GLFWwindow* window);

  static glm::vec2 ScreenToWorld(glm::vec2& screenCoordinates);

  static glm::vec2 WorldToTexture(glm::vec2& worldCoordinates, float size);

  inline static GLFWwindow* Window() { return window_; }

  inline static bool isDirty() { return isDirty_; }

  inline static void isDirty(bool dirty) { isDirty_ = dirty; }

  inline static WindowMode GetWindowMode() { return windowMode_; }

  inline static void SetWindowMode(WindowMode newMode) { windowMode_ = newMode; isDirty_ = true; }

  inline static int GetWidth() { return fbWidth_; }

  inline static int GetHeight() { return fbHeight_; }

  inline static GLdouble FPS() { return fps; }

  double update_time(double fps_calc_interval);

private:

  Shader * shaderProgram_;
  static bool isDirty_;
  static GLFWwindow* window_;
  static int fbWidth_;
  static int fbHeight_;
  std::string frameTitle = "EndRest ";
  static WindowMode windowMode_;
  static GLdouble fps;
  static float time_;
  float time = 0.f;
};

///---------------------------------FUNCTIONS-----------------------------------