///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Mesh.h
//
//  Author: Chales Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
///----------------------------------GLOBALS------------------------------------

//structure type to hold Vertex Data
struct Vertex
{
  glm::vec3 position_;
  glm::vec3 color_;
  glm::vec2 texCoord_;
};

enum MeshType {
  Square_,
  Triangle_,
};

///----------------------------------CLASSES------------------------------------
class Mesh
{
public:
  ~Mesh();

  glm::vec2 MeshGetRowsColumns();

  Vertex * GetVertices();

  GLuint * GetIndices();

  GLuint GetVBO();
  GLuint MeshGetID();
  GLuint GetEBO();

  bool operator==(Mesh otherMesh);

  bool operator==(glm::vec2 otherRowColumn);
  glm::vec2 GetNumRowsCols();
	
private:
  Mesh(unsigned x, unsigned y, MeshType typeMesh = Square_);



  //VBO
  Vertex * vertices_;
  GLuint amntOfVertices_;

  //EBO
  GLuint * indices_;
  GLuint amntOfIndices_;

  //VAO
  GLuint meshBufferID_;
  //VBO
  GLuint VertexBufferObject_;
  //EBO
  GLuint ElementBufferObject_;


  MeshType typeMesh_;

  unsigned columns_;
  unsigned rows_;

  friend class TextureSys;
};

///---------------------------------FUNCTIONS-----------------------------------


