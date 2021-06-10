///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Mesh.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Mesh.h"
#include <vector>


///----------------------------------GLOBALS-----------------------------------
static std::vector<Mesh*> meshes;

///---------------------------------FUNCTIONS-----------------------------------

Mesh::Mesh(unsigned x, unsigned y, MeshType typeMesh)
{
	float uSize = 1.0f / x;
	float vSize = 1.0f / y;


	if (typeMesh == Square_)
	{

		//Basic vertices for the "unit box" mesh, most used mesh in game for our 2D
		Vertex vertices[] =
		{
			//Positions                       //Colors                       //TexUVCoords     
			glm::vec3(0.5f, 0.5f, 0.f),     glm::vec3(0.f, 0.f, 0.f),      glm::vec2(uSize, vSize),
			glm::vec3(0.5f, -0.5f, 0.f),    glm::vec3(0.f, 0.f, 0.f),      glm::vec2(uSize, 0.f),
			glm::vec3(-0.5f, -0.5f, 0.f),     glm::vec3(0.f, 0.f, 0.f),      glm::vec2(0.0f, 0.f),
			glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(0.f, 0.f, 0.f),      glm::vec2(0.0f, vSize)
		};

		//Used so we have no need to memorize the amount of Verts per objects
		amntOfVertices_ = sizeof(vertices) / sizeof(Vertex);

		vertices_ = vertices;

		GLuint indices[] = {  // note that we start from 0!
			0, 1, 2,  // first Triangle
			0, 2, 3   // second Triangle
		};

		//Used so we have no need to memorize the amount of indicies
		amntOfIndices_ = sizeof(indices) / sizeof(GLuint);

		indices_ = indices;
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glGenVertexArrays(1, &meshBufferID_);
		glBindVertexArray(meshBufferID_);

		//Generate VBO data and send it over to gpu
		//glGenBuffers(1, &VBO);  MOVE TO MESH
		glGenBuffers(1, &VertexBufferObject_);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//Generate EBO and send it over to gpu
		//glGenBuffers(1, &EBO);  MOVE TO MESH
		glGenBuffers(1, &ElementBufferObject_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//POSITIONS
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);

		//COLORS
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);

		//TEXCOORDS
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 6));
		glEnableVertexAttribArray(2);


		//glBindBuffer(GL_ARRAY_BUFFER, meshBufferID);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



		columns_ = x;
		rows_ = y;
	}

	if (typeMesh == Triangle_)
	{

		//Basic vertices for the "unit box" mesh, most used mesh in game for our 2D
		Vertex vertices[] =
		{
			//Positions                       //Colors                       //TexUVCoords     
			glm::vec3(0.f, 0.5f, 0.f),       glm::vec3(0.f, 0.f, 0.f),      glm::vec2(0.f, vSize),
			glm::vec3(-0.5f, -0.5f, 0.f),    glm::vec3(0.f, 0.f, 0.f),      glm::vec2(0.f, 0.f),
			glm::vec3(0.5f, -0.5f, 0.f),     glm::vec3(0.f, 0.f, 0.f),      glm::vec2(uSize, 0.f)
		};

		vertices_ = vertices;

		//Used so we have no need to memorize the amount of Verts per objects
		amntOfVertices_ = sizeof(vertices) / sizeof(Vertex);

		indices_ = NULL;

		//glBindBuffer(GL_ARRAY_BUFFER, meshBufferID_);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		this->columns_ = x;
		this->rows_ = y;
	}

	typeMesh_ = typeMesh;
}

//Double Check this
Mesh::~Mesh()
{
	//delete meshes[0];
}

GLuint Mesh::MeshGetID()
{
	return meshBufferID_;
}

glm::vec2 Mesh::MeshGetRowsColumns()
{
	glm::vec2 rowsColumns(columns_, rows_);

	return rowsColumns;
}

//Mesh& Mesh::FindMesh(glm::vec2 rowsColumns, MeshType typeMesh)
//{
//	//for (Mesh * itr : meshes)
//	//{
//	//	if (itr-columns_ == rowsColumns.x && itr.rows_ == rowsColumns.y && itr.typeMesh_ == typeMesh)
//	//	{
//	//		//check if right
//	//		return *(new Mesh(itr));
//	//	}
//	//}
//
//	//Mesh* meshThing = new Mesh(rowsColumns.x, rowsColumns.y, typeMesh);
//
//	//meshes.push_back(*meshThing);
//
//	return &NULL;
//
//}

bool Mesh::operator==(Mesh otherMesh)
{
	if (columns_ == otherMesh.columns_ && rows_ == otherMesh.rows_)
	{
		return true;
	}

	return false;
}

bool Mesh::operator==(glm::vec2 otherRowColumn)
{
	if (columns_ == otherRowColumn.x && rows_ == otherRowColumn.y)
	{
		return true;
	}

	return false;
}

Vertex * Mesh::GetVertices()
{
	return vertices_;
}

GLuint * Mesh::GetIndices()
{
	return indices_;
}

GLuint Mesh::GetVBO()
{
	return VertexBufferObject_;
}

GLuint Mesh::GetEBO()
{
	return ElementBufferObject_;
}

glm::vec2 Mesh::GetNumRowsCols()
{
	return glm::vec2(rows_, columns_);
}
