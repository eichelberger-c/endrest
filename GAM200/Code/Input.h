///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Input.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles keyboard input and the mouse's position.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "System.h"
#include "Vec2D.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Input : public System
{
public:
  Input();
  ~Input();
  virtual void Initialize();
  virtual void Update(float dt);
  void UpdateMouse(GLFWwindow *window);
  static Obj * CheckMouse(glm::vec2 mousePos);
  void IsActive(bool toggle);
  bool IsActive();
  int GetDirection(glm::vec3 mouseVec);
  //void CheckPuzzleInput(int zone, GLFWwindow *window);
  void CheckAttack(float dt);

  friend class Lantern;

  inline void Left(GLuint left) { leftMovement_ = left; }
  inline void Right(GLuint right) { rightMovement_ = right; }
  inline void Up(GLuint up) { upMovement_ = up; }
  inline void Down(GLuint down) { downMovement_ = down; }

  inline GLuint Left() { return leftMovement_; }
  inline GLuint Right() { return rightMovement_; }
  inline GLuint Down() { return downMovement_; }
  inline GLuint Up() { return upMovement_; }
  inline GLuint Push() { return pushBlock_;  }
  inline GLuint Attack() { return attack_; }

  inline bool attackReady() { return attackReady_; }
  inline void attackReady(bool attack) { attackReady_ = attack; }

private:
  Vec2D mousePos_;
  Vec2D worldPos_;
  bool isActive_;
  bool isReady_ = false;
  bool attackReady_ = false;
  int timer_ = 60;
  int flashFrames_ = 0;

  GLuint leftMovement_ = GLFW_KEY_A;
  GLuint rightMovement_ = GLFW_KEY_D;
  GLuint upMovement_ = GLFW_KEY_W;
  GLuint downMovement_ = GLFW_KEY_S;
  GLuint pushBlock_ = GLFW_KEY_SPACE;
  GLuint attack_ = GLFW_MOUSE_BUTTON_LEFT; 

};
///---------------------------------FUNCTIONS-----------------------------------


