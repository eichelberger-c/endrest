///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Fade.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  brief: This class is for the the fading in and out at beg and end of game
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "Component.h"
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

///----------------------------------GLOBALS------------------------------------
///----------------------------------CLASSES------------------------------------
enum Fades
{
	Intro_,
	Outro_,
};
class Fade : public Obj
{
public:
	Fade();
	virtual void Initialize();
	virtual void Update(float dt);
  void SetOutro();

private:
	float frames_ = 15;
	Fades type_ = Intro_;
	bool init_ = true;
	float soundFrames_ = 60*7;
};


///---------------------------------FUNCTIONS-----------------------------------


#pragma once
#pragma once
