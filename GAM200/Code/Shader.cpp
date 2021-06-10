///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Shader.cpp
//
//  Author: Thornton Stockwin
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Shader
//      - Use
//      - CheckCompileErrors
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
///----------------------------------INCLUDES-----------------------------------
#include "Shader.h"
#include "stdafx.h"

///---------------------------------FUNCTIONS-----------------------------------

///------------------------------------------
///Author: Thornton Stockwin
///Brief: Opens two files for vertex and fragment, then compiles them together
///       Throws and error if failed
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  //used to retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // exception handling for file reading
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure e)
  {
    TraceMessage(GraphicsLog, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
  }

  const char* vShaderCode = vertexCode.c_str();
  const char * fShaderCode = fragmentCode.c_str();
  //compile shaders
  GLint vertex, fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, "VERTEX");
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, "FRAGMENT");
  // shader Program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertex);
  glAttachShader(shaderProgram, fragment);
  glLinkProgram(shaderProgram);
  CheckCompileErrors(shaderProgram, "PROGRAM");
  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  vertName_ = vertexPath;
  fragName_ = fragmentPath;
}

///------------------------------------------
///Author: Thornton Stockwin
///Brief: Activate the Shader
void Shader::UseShader()
{
  glUseProgram(shaderProgram);
}

void Shader::Unuse()
{
  glUseProgram(0);
}

//This Giant block is repeated code with minute changes to each
//Follow one and the rest are the same

void Shader::SendBool(const char* name, bool value) const
{
  //Setting up the location on GPU for data under name
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  //Error checking, if the location on GPU was not found
  if (uniformVarLoc >= 0) 
  {
    //if found send the variable to GPU for shaders
    glUniform1i(uniformVarLoc, (int)value);
  }
  else 
  {
    //you messed up and got an error, how could you
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}
void Shader::SendInt(const char* name, int value) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0) 
  {
    glUniform1i(uniformVarLoc, value);
  }
  else 
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}
void Shader::SendFloat(const char* name, float value) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0) 
  {
    glUniform1f(uniformVarLoc, value);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}

void Shader::SendVec2(const char* name, glm::vec2 &value) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniform2fv(uniformVarLoc, 1, &value[0]);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}
void Shader::SendVec2(const char* name, float x, float y) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniform2f(uniformVarLoc, x, y);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}

void Shader::SendVec3(const char* name, glm::vec3 &value) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniform3fv(uniformVarLoc, 1, &value[0]);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}
void Shader::SendVec3(const char* name, float x, float y, float z) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniform3f(uniformVarLoc, x, y, z);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}

void Shader::SendVec4(const char* name, glm::vec4 &value) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniform4fv(uniformVarLoc, 1, &value[0]);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}
void Shader::SendVec4(const char* name, float x, float y, float z, float w) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniform4f(uniformVarLoc, x, y, z, w);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}

void Shader::SendMat2(const char* name, const glm::mat2 &mtx) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniformMatrix2fv(uniformVarLoc, 1, GL_FALSE, &mtx[0][0]);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}
void Shader::SendMat3(const char* name, const glm::mat3 &mtx) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniformMatrix3fv(uniformVarLoc, 1, GL_FALSE, &mtx[0][0]);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }

}
void Shader::SendMat4(const char* name, const glm::mat4 &mtx) const
{
  GLint uniformVarLoc = glGetUniformLocation(shaderProgram, name);

  if (uniformVarLoc >= 0)
  {
    glUniformMatrix4fv(uniformVarLoc, 1, GL_FALSE, &mtx[0][0]);
  }
  else
  {
    TraceMessage(GraphicsLog, "Uniform variable with name %s doesn't exist!!!", name);
    std::exit(EXIT_FAILURE);
  }
}

///------------------------------------------
///Author: Thornton Stockwin
///Brief: utility function for checking shader compilation/linking errors.
void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      TraceMessage(GraphicsLog, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n-- -------------------------------------------------- - -- ", type.c_str(), infoLog);
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      TraceMessage(GraphicsLog, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- ", type.c_str(), infoLog);
    }
  }
}