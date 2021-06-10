///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Component.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

//#include "stdafx.h"
//#include "Obj.h"

///----------------------------------GLOBALS------------------------------------
typedef class Component Component;
typedef class Obj Obj;
typedef Component *(*ConstructPtr)(void);
typedef class Collider Collider;

///----------------------------------CLASSES------------------------------------
//COMP
class Component
{
public:
  typedef enum ComponentTypes
  {
    Invalid_ = -1,

    Audio_,
    Animation_,
    Behavior_,
    Health_,
    Physics_,
    Sprite_,
    Transform_,
    Collider_,
    Stats_,

    HasMap_, //anything before this has a map in Space
    ObjInput_,
    Color_,
    ResType_,
    Timer_,
    Score_,
    Debug_,   
    ParticleEmitter_,
    Key_,
    Lighting_,
    IsEvent_, //anything after this is an EventComp type

    Spawn_,       //Spawns an obj
    
    FlashSprite_, //Makes a sprite visible for a short time
    ToggleDark_,
    SetDoor_,
    Inspection_,
    Particles_,
    ChangeMusic_,
    Aggro_,       //Aggros an enemy
    SetInvincible_,
    SetActive_,
    SetVisible_,  //Makes a sprite visible or invisible
    CMax_,
  } compType;

  Component(compType type); //TO DO: check type for EventComps?
  Component(const Component& ogComponent);  
  Component(Component *comp);
  virtual ~Component() {}

  virtual Component * Clone() { return nullptr; }
  virtual void Update(float dt);
  virtual void Reset() {}
  virtual void OnCollision(Collider& other) {}
  virtual void OnCollision(int type) {}
  virtual void ReGetEvent() {} //for EventComps only
  virtual void DoTheEvent() {} //same

  void Parent(Obj *parent);
  Obj * Parent();
  Obj * Parent() const;
  void Type(compType type);
  compType Type() const; 
  bool IsActive() const;
  void IsActive(bool toggle);

private:
  compType type_;
  Obj *parent_;
  bool isActive_;
};

///---------------------------------FUNCTIONS-----------------------------------
bool ComponentCompare(Component *left, Component *right);


