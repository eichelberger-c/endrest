/********************************************/
/*
Author: Charles Eichelberger

Description:
This file contains basic implimentation for a Camera

Copyright © 2019 DigiPen (USA) Corporation.

*/
/********************************************/

/* /////////////// FILE VARIABLES /////////////// */

/* //// INCLUDES //// */
#include "Camera.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


////////////////////////
/* FORWARD REFERENCES */


////////////////////////


////////////////////////////////////////////////////
/* ////////// MEMBER STATIC VARIABLES  ////////// */

Camera* Camera::camera = nullptr;
bool Camera::projMatDirty_ = true;
bool Camera::viewMatDirty_ = true;
float Camera::zoom_ = 1;
glm::mat3 Camera::projectionMat_;
glm::mat3 Camera::viewMat_;
glm::vec3 Camera::translation_ = glm::vec3(0, 0, -.5);
////////////////////////////////////////////////////
/* ////////// MEMBER STATIC FUNCTIONS  ////////// */


////////////////////////////////////////////////////
/* /// CONSTRUCTORS, DESTRUCTORS, & OPERATORS /// */

Camera::Camera()
{
}

Camera::~Camera()
{
}


////////////////////////////////////////////////////
/* ///// MEMBER FUNCTIONS & NESTED CLASSES  ///// */

glm::mat3 & Camera::ViewMat()
{
	//This stuff is broken. If the coordinate commented out down below is greater than 1, then the image is not rendered. Basically, camera zoom isn't supported
	if (viewMatDirty_)
	{
    viewMat_ = { zoom_, 0, 0,
                 0, zoom_, 0,
                 0, 0, 1 };
    viewMatDirty_ = false;
	}
	return viewMat_;
}

glm::mat3 & Camera::ProjectionMat()
{
  if (projMatDirty_)
  {
    projectionMat_ = glm::ortho(-1920 / (2.f), 1920 / (2.f), -1080 / (2.f), 1080 / (2.f));
    projMatDirty_ = false;;
  }
	return projectionMat_;
}

void Camera::SetTranslation(glm::vec3 pos)
{
	viewMatDirty_ = true;
	translation_ = pos;
}

void Camera::SetZoom(float zoom)
{
	zoom_ = zoom;
  viewMatDirty_ = true;
}


////////////////////////////////////////////////////

