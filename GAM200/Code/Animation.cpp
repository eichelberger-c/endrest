///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Animation.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Animation.h"
#include "Sprite.h"
#include "Obj.h"
#include "ObjManager.h"
#include "Player.h"
#include "Spaces.h"
#include "Graphics.h"
#include "EndGs.h"
#include "GameStateManager.h"

static ObjManager objMan;
///----------------------------------GLOBALS-----------------------------------




///---------------------------------FUNCTIONS-----------------------------------

Animation::Animation(const char *name, int beg, int end, float dur, bool animate) : Component(Component::Animation_)
{
  name_ = name;
  isAnimating_ = animate;
  frameBeg_ = beg;
  frameEnd_ = end;
  frameDuration_ = dur;
}

Animation::Animation(const Animation& ogAnimation) : Component(Component::Animation_)
{
  name_ = ogAnimation.name_;
  isAnimating_ = ogAnimation.isAnimating_;
  frameBeg_ = ogAnimation.frameBeg_;
  frameEnd_ = ogAnimation.frameEnd_;
  frameDuration_ = ogAnimation.frameDuration_;
  IsActive(ogAnimation.IsActive());
}

Animation::Animation(Component * animation) : Component(Component::Animation_)
{
  Animation *ani = dynamic_cast<Animation*>(animation);

  name_ = ani->name_;
  isAnimating_ = ani->isAnimating_;
  frameBeg_ = ani->frameBeg_;
  frameEnd_ = ani->frameEnd_;
  frameDuration_ = ani->frameDuration_;
  IsActive(animation->IsActive());
}

Component * Animation::Clone() const
{
  Animation *animation = new Animation(name_.c_str());
  if (animation == NULL)
    return NULL;

  *animation = *this;

  return animation;
}

void Animation::Advance(float dt)
{
if (frameDuration_ <= Graphics::FPS())
	internalDuration_ = float(Graphics::FPS()) / frameDuration_;
else
	internalDuration_ = 1.0f;

  Sprite* sprite = Parent()->Has(Sprite);
  int index = int(sprite->GetFrameIndex());

  if(direction_ == Forward_)
  {
    index++;
    if (index >= frameEnd_)
      index = frameBeg_;
  }
  else if (direction_ == Reverse_)
  {
    index--;
    if (index <= frameBeg_)
      index = frameEnd_;
  }
  else if (direction_ == Death_)
  {
    index++;
    if (index >= frameEnd_)
    {
      bool f = false;
      Obj* lanternPointer = objMan.GetObj("Lantern");
      frameBeg_ = frameEnd_;
      index = frameEnd_;
      Parent()->IsActive(f);
      Parent()->IsDead(true);
      lanternPointer->Has(Sprite)->SetAlpha(0);
      lanternPointer->IsActive(f);
      lanternPointer->IsDead(true);
      isAnimating_ = false;
      static_cast<Player*>(Parent())->Dying(false);

      SetEnding(Lose);

      static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->BackgroundMusic("Data/Audio/EndRestMusic/EndRestEvent1.wav");
      GameStateManager *Gman = dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"));
      Gman->SetNextState(EndGs);
    }
  }
  sprite->SetIndex(index);
  delay_ = internalDuration_ * dt;
}

void Animation::Update(float dt)
{
  if (!IsActive())
    return;

    if (isAnimating_)
    {
      delay_ -= dt;
      if (delay_ <= 0)
      {
        Advance(dt);
      }
    }
}

void Animation::SetAnimating(bool animating)
{
  isAnimating_ = animating;
}

bool Animation::GetAnimating()
{
  return isAnimating_;
}

void Animation::Name(std::string name)
{
  name_ = name;
}

std::string Animation::Name()
{
  return name_;
}

void Animation::SetBegFrame(int beg)
{
  frameBeg_ = beg;
}

void Animation::SetEndFrame(int end)
{
  frameEnd_ = end;
}

void Animation::SetFrameDur(float dur)
{
  frameDuration_ = dur;
}

float Animation::GetFrameDur()
{
  return frameDuration_;
}

int Animation::GetBegFrame()
{
  return frameBeg_;
}

int Animation::GetFramEnd()
{
  return frameEnd_;
}

animationDirection Animation::GetDirection()
{
  return direction_;
}
void Animation::SetDirection(animationDirection direction)
{
  direction_ = direction;
}