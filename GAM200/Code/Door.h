///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Door.h
//
//  Author: Megan Works
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief: A behavior "script" that //fill this in 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "BehaviorScripts.h"
#include "Collider.h"
#include "Graphics.h"
#include "Engine.h"
#include "Input.h"
#include "ScreenManager.h"
//#include "HealthBar.h"
#include "Spaces.h"

///----------------------------------GLOBALS------------------------------------
typedef enum DoorStates
{
  dInvalid,
  dOpen,      // opened door - doesn't need interacting
  dLocked,    // needs key to open
  dUnlocked,  // key has been used
  dItsATrap,  // secret?
} dState;

///----------------------------------CLASSES------------------------------------
class Door : public BehaviorScript
{
public:
  Door(std::string name = "Door", dState state = dUnlocked, std::string area = ""); //always use the default DO NOT CHANGE THE NAME
  Door(Door& ogDoor, std::string area = "");

  dState StartState();
  void StartState(dState state);

  ScreenManager::direction wall; // wall door connected to
  std::string area; // what area this door connects to
  std::string ID;   // this MUST be set manually

  soundObject unlock = nullptr;
  soundObject locked = nullptr;
  soundObject open = nullptr;
  int goaway = 60;

  virtual void Start() override
  {
    score_ = (&Space::GetPlayer())->Has(Score);
    screenManager_ = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
    StateCurrent(dInvalid);
    StateNext(startState_);

    unlock = aEngine.GetSoundObject("Data/Audio/Unlock.wav", sfx_);
    locked = aEngine.GetSoundObject("Data/Audio/Locked.wav", sfx_);
    open = aEngine.GetSoundObject("Data/Audio/OpenDoor.wav", sfx_);
  }

  virtual void SpecialUpdate(float dt) override
  {
    if (goaway != 60)
      goaway++;
  }

  virtual void Exit() override
  {

  }

  void OnCollisionWithPlayer()
  {
    if (area.empty())
      return;

    dState state = static_cast<dState>(StateCurrent());
    
    if (state == dOpen) // does not require interaction - i.e. stairs
    {
      // go through door
      screenManager_->ChangeCurrentScreen(area, wall);
      return;
    }

    // pop-up button option should show -- press e
    else if (glfwGetKey(Graphics::Window(), input.Push()) != GLFW_PRESS)
      return;

    Obj* key = nullptr;
    switch (state)
    {
    case dLocked:
      key = score_->FindKey(ID);

      if (key)
      {
        // unlock sound effect
        aEngine.Play(unlock);
        score_->RemoveItem(key); 
        StateNext(dUnlocked);
        goaway = 0;
        break;
      }
      else
      {
        // door locked sound effect
        if (!aEngine.IsPlaying(locked))
          aEngine.Play(locked);
      }
      break;
    case dUnlocked: 
      if (goaway != 60)
        return;
      
      // door open sound
      aEngine.Play(open);
      screenManager_->ChangeCurrentScreen(area, wall); // go through door
      break;
    case dItsATrap:
      break;
    }
  }

  void OnCollision(Collider& other)
  {
  }

  void OnDeactivate()
  {
  }

private:
  ScreenManager* screenManager_;
  Score *score_;
  dState startState_;
  Input &input = *reinterpret_cast<Input*>(EngineGetSystem("Input"));
  AudioEngine& aEngine = *reinterpret_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));
};

///---------------------------------FUNCTIONS-----------------------------------
dState GetDoorStateFromString(std::string state);

