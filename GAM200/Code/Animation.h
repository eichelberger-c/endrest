///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Animation.h
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

#include "Component.h"
#include "Mesh.h"
#include "SpriteSource.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
typedef enum animationDirection
{
  Forward_,
  Reverse_,
  Death_,
} animationDirection;

class Animation : public Component
{
public:
	Animation(const char *name, int beg = 0, int end = 34, float dur = 60.0f, bool animate = true);
	Animation(const Animation& ogAnimation);
  Animation(Component *animation);

	virtual Component * Clone() const;
	virtual void Update(float dt);
  void Advance(float dt);
  virtual void Reset() {} // write me

	void SetAnimating(bool animating);
	void SetBegFrame(int beg);
	void SetEndFrame(int end);
	void SetFrameDur(float dur);

	float GetFrameDur();
	int GetBegFrame();
	int GetFramEnd();
	bool GetAnimating();

  animationDirection GetDirection();
  void SetDirection(animationDirection);

  void Name(std::string name);
  std::string Name();

private:
	std::string name_;

	int frameBeg_;

	int frameEnd_;

  float delay_;

	float frameDuration_;

  float internalDuration_;

	bool isAnimating_ = true;

	float timer_;

  animationDirection direction_ = Forward_;
};

///---------------------------------FUNCTIONS-----------------------------------


