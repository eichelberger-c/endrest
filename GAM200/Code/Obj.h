///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Obj.h
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
//#define Has(type) GetComp<type>(type)

#include "stdafx.h"
#include "Component.h"
#include "Utils.h"
//#include "Events.h"

///----------------------------------GLOBALS------------------------------------
typedef class Obj Obj;

typedef enum ResolutionType
{
  Player_,
  Enemy_,
  Wall_,
  Lantern_,
  Catalyst_,
  Item_, //something that goes in the player's inventory such as a key
  Trigger_,
  Other_,
  No_,  //no collision. collision bad.

} resType;

///----------------------------------CLASSES------------------------------------

class Obj
{
public:
#define Has(type) GetComponent<type>(Component::type##_)

  Obj(const char *name);
  Obj(std::string name);
  Obj(const Obj& ogObj);
  Obj(const Obj& ogObj, std::string name);
  virtual ~Obj();

  void Add(Component *component);
  bool HasComponent(Component::compType type);
  void Swap(Component *newComponent);
  Component * Get(Component::compType type) const;

  virtual void Update(float dt);
  void Destroy();
  bool MarkedForDeath() const;
  void ResolveCollision(Obj *otherObj);
  float CalculateAlphaforHealth();
  void Kill();
  void Resurrect(); //uses Reset
  void Respawn(); //only sets IsRespawn bool as a flag
  virtual void Reset(); //for instant respawning 
  void ResetAll(); // resets all components

  void ResolutionType(resType type);
  void ResolutionType(std::string type);
  std::string ResolutionStringType();
  resType ResolutionType();
  void Hide(bool toggle);
  EventZone::Event<bool> hide;

  void Color(colorFlag color);
  colorFlag Color() const;
  std::string& Name();
  void MapName(std::string newName);
  std::string MapName();
  std::string& Type();
  bool IsDead();
  void IsDead(bool toggle);
  bool IsActive();
  void IsActive(bool& toggle);
  bool IsRespawn();
  void IsRespawn(bool respawn);
  bool IsTrigger();
  void IsTrigger(bool set);
  bool PartsLast();
  void PartsLast(bool set);

  virtual void Initialize();
  virtual void Shutdown();
  virtual void Render();

  template<typename type>
  type* GetComponent(Component::ComponentTypes typeId)
  {
    return static_cast<type*>(Get(typeId));
  }

  friend class Data;
  friend class EventManager;
  friend class Trigger;

  // added by megan
  void AddSubObj(Obj *object);
  std::vector<Obj*> GetSubObjList();
  Obj* GetSubObject(std::string filename);


private:
  Obj();
  Component ** GetPP(Component::compType type);
  float CalculateKill();

  std::string name_;
  bool killSwitch_; //set to free an object completely 
  bool isDead_; //set when an obj is dead in game and should be hidden
  bool isActive_;
  bool isTrigger_;
  std::vector<Component*> components_;
  colorFlag color_;
  std::string mapName_;
  std::string type_;
  resType resT_;
  bool respawn_;

  bool partsLast_;

  std::vector<Obj*> subList_; // added by megan -- for objects that belong to other objects (i.e.lantern)

};

///---------------------------------FUNCTIONS-----------------------------------
resType GetResTypeFromString(std::string resString);
std::string GetStringFromResType(resType restype);
