///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Camera.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Sprite
//      - Sprite (copy constructor)
//      - Update (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include <glm\gtc\matrix_transform.hpp>

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Camera
{
public:
	~Camera();
	static Camera * GetCamera() { if (camera) { return camera; } else { return new Camera(); }; }

	static glm::mat3& ViewMat();

	static glm::mat3& ProjectionMat();

	void SetTranslation(glm::vec3 pos);

	static void SetZoom(float zoom);

	static float GetZoom() { return zoom_; }

	inline static glm::vec3 GetTranslation() { return translation_; }

private:
	static Camera* camera;

  Camera();

  static float zoom_;

	static bool viewMatDirty_;

	static bool projMatDirty_;
	
	const float near_ = -1;

	const float far_ = 100;

	static glm::vec3 translation_;

	static glm::mat3 projectionMat_;

	static glm::mat3 viewMat_;
};

///---------------------------------FUNCTIONS-----------------------------------


