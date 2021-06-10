///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Shader.h
//
//  Author: Thornton Stockwin
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Cool thing that'll compile shaders together for us and let us use
//         them freely.
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

///----------------------------------GLOBALS------------------------------------

///----------------------------------CLASSES------------------------------------

class Shader
{
public:

  GLuint shaderProgram;

  // activate the shader
  void UseShader();

  void Unuse();

  //For sending values in a uniform to the shader program
  void SendBool(const char* name, bool value) const;
  void SendInt(const char* name, int value) const;
  void SendFloat(const char* name, float value) const;

  void SendVec2(const char* name, glm::vec2 &value) const;
  void SendVec2(const char* name, float x, float y) const;

  void SendVec3(const char* name, glm::vec3 &value) const;
  void SendVec3(const char* name, float x, float y, float z) const;

  void SendVec4(const char* name, glm::vec4 &value) const;
  void SendVec4(const char* name, float x, float y, float z, float w) const;

  void SendMat2(const char* name, const glm::mat2 &mtx) const;
  void SendMat3(const char* name, const glm::mat3 &mtx) const;
  void SendMat4(const char* name, const glm::mat4 &mtx) const;


private:
  // constructor generates the shader on demand
  Shader(const char* vertexPath, const char* fragmentPath);
  // utility function for checking shader compilation/linking errors.
  void CheckCompileErrors(unsigned int shader, std::string type);
  std::string vertName_;
  std::string fragName_;

  friend class ShaderSys;
};