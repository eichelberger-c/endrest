///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Spaces.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Space class. Basically a container for all the things so that they
//  know about each other (Theoretically - they can be used in several ways
//  and how to best use them for our purposes is potentially unknown so far)
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
//COMP

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "ObjFactory.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "Audio.h"
#include "Collider.h"
#include "Player.h"
#include "ColliderCircle.h"
#include "ColliderSquare.h"
#include "Lantern.h"
#include "Animation.h"
#include "Behavior.h"
#include "Health.h"
#include "Stats.h"

///----------------------------------GLOBALS------------------------------------
using AnimationList = std::map<std::string, Animation*>;
using AudioList = std::map<std::string, Audio*>;
using BehaviorList = std::map<std::string, Behavior*>;
using ColliderList = std::map<std::string, Collider*>;
using HealthList = std::map<std::string, Health*>;
using PhysicsList = std::map<std::string, Physics*>;
using SpriteList = std::map<std::string, Sprite*>;
using TransformList = std::map<std::string, Transform*>;
using StatList = std::map<std::string, Stats*>;
//using BlockList = std::vector<std::string, Block*>; //Need Block pointers to exist; find by area name?

using CircleColliderList = std::map<std::string, ColliderCircle*>;
using SquareColliderList = std::map<std::string, ColliderSquare*>;

using ObjectList = std::vector<Obj*>;
using ArchetypeList = std::map<std::string, Obj*>;

//using CompAdderP = void(Space::*)(Component *, std::string);
//using CompAdder = void(*)(Component *, std::string);

typedef void(*CompAdderP)(Component *, std::string);

///----------------------------------CLASSES------------------------------------
class Space
{
public:
  static void SpaceInit();

  static AnimationList& GetAnimationList();
  static AudioList& GetAudioList();
  static BehaviorList& GetBehaviorList();
  static ColliderList& GetColliderList();
  static CircleColliderList& GetCircleColliderList();
  static SquareColliderList& GetSquareColliderList();
  static HealthList& GetHealthList();
  static PhysicsList& GetPhysicsList();
  static SpriteList& GetSpriteList();
  static TransformList& GetTransformList();
  static StatList& GetStatList();

  static void AddAnimation(Component *animation, std::string objName);
  static void AddAudio(Component *audio, std::string objName);
  static void AddBehavior(Component *behavior, std::string objName);
  static void AddCollider(Component *collider, std::string objName);
  static void AddCircleCollider(Component *circle, std::string objName);
  static void AddSquareCollider(Component *square, std::string objName);
  static void AddHealth(Component *health, std::string objName);
  static void AddPhysics(Component *physics, std::string objName);
  static void AddSprite(Component *sprite, std::string objName);
  static void AddTransform(Component *transform, std::string objName);
  static void AddStats(Component *stats, std::string objName);

  static void AddArchetype(Obj * obj);
  static Obj * GetArchetype(std::string type);
  static ArchetypeList& GetArchetypeList();

  static ObjectList& GetObjList(); 
  //static ObjectList& GetEntireObjList();

  static void AddComponent(Component *comp);

  static Transform * GetTransform(std::string objName);
  static Collider * GetCollider(std::string objName);
  static Physics * GetPhysics(std::string objName);
  static ColliderCircle * GetCircleCollider(std::string objName);
  static ColliderSquare * GetSquareCollider(std::string objName);
  static Health * GetHealth(std::string objName);
  static Stats * GetStats(std::string objName);

  static ObjectList& GetPartsLastList(ScreenManager *sMan);
  static void AddPartsLast(Obj *obj);

  static Player& GetPlayer();

  friend class ObjFactory;
  friend class ObjManager;

private:
  static ArchetypeList archetypes_; // map of object archetypes (from files)

  static ObjectList ObjList; //style guid exception - signifying this is THE object list

  static AnimationList animations_;
  static AudioList audios_;
  static BehaviorList behaviors_;

  static ColliderList colliders_;
  static CircleColliderList circles_;
  static SquareColliderList squares_;

  //std::vector<ObjInput*> inputs_;
  static HealthList healths_;
  static PhysicsList physics_;
  static SpriteList sprites_;
  static TransformList transforms_;
  static StatList stats_;

  static std::map<Component::compType, CompAdderP> compAdders_;

  static ObjectList partsLast_;

  static Player player_;
};


///---------------------------------FUNCTIONS-----------------------------------
void ToggleDarkness();

