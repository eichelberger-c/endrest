///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Spaces.cpp
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
//COMP

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Spaces.h"
#include "Lighting.h"

///----------------------------------GLOBALS-----------------------------------
  //class-external declarations of static members
AnimationList Space::animations_;
AudioList Space::audios_;
BehaviorList Space::behaviors_;
ColliderList Space::colliders_;
CircleColliderList Space::circles_;
SquareColliderList Space::squares_;
HealthList Space::healths_;
PhysicsList Space::physics_;
SpriteList Space::sprites_;
TransformList Space::transforms_;
StatList Space::stats_;
ObjectList Space::ObjList;
ArchetypeList Space::archetypes_;

std::map<Component::compType, CompAdderP> Space::compAdders_;

ObjectList Space::partsLast_;
ObjectList currPartsLast;
std::string currScreen;

Player Space::player_;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Stores Adder function pointers in map under their component type
void Space::SpaceInit()
{
  compAdders_[Component::Animation_] = &AddAnimation;
  compAdders_[Component::Audio_] = &AddAudio;
  compAdders_[Component::Behavior_] = &AddBehavior;
  compAdders_[Component::Collider_] = &AddCollider;
  compAdders_[Component::Health_] = &AddHealth;
  compAdders_[Component::Physics_] = &AddPhysics;
  compAdders_[Component::Sprite_] = &AddSprite;
  compAdders_[Component::Transform_] = &AddTransform;
  compAdders_[Component::Stats_] = &AddStats;

  currScreen = "EntranceToEndrest";
}

///-------------------------------------
/// Brief: returns a reference to the list of Audio components
AudioList & Space::GetAudioList()
{
  return audios_;
}

BehaviorList & Space::GetBehaviorList()
{
  return behaviors_;
}

///-------------------------------------
/// Brief: returns a reference to the list of Collider components
ColliderList & Space::GetColliderList()
{
  return colliders_;
}

CircleColliderList & Space::GetCircleColliderList()
{
  return circles_;
}

SquareColliderList & Space::GetSquareColliderList()
{
  return squares_;
}

HealthList & Space::GetHealthList()
{
  return healths_;
}

///-------------------------------------
/// Brief: returns a reference to the list of Physics components
PhysicsList & Space::GetPhysicsList()
{
  return physics_;
}

///-------------------------------------
/// Brief: returns a reference to the list of Sprite components 
SpriteList & Space::GetSpriteList()
{
  return sprites_;
}

///-------------------------------------
/// Brief: returns a reference to the list of Transform components
TransformList & Space::GetTransformList()
{
  return transforms_;
}

StatList & Space::GetStatList()
{
  return stats_;
}

AnimationList & Space::GetAnimationList()
{
  return animations_;
}

///-------------------------------------
/// Brief: Adds an Audio component to the appropriate map
void Space::AddAudio(Component * audio, std::string objName)
{
  audios_[objName] = dynamic_cast<Audio*>(audio);
}

void Space::AddBehavior(Component * behavior, std::string objName)
{
  behaviors_[objName] = dynamic_cast<Behavior*>(behavior);
}

void Space::AddAnimation(Component * animation, std::string objName)
{
  animations_[objName] = dynamic_cast<Animation*>(animation);
}

///-------------------------------------
/// Brief: Adds a Collider component to the appropriate map
void Space::AddCollider(Component * collider, std::string objName)
{
  colliders_[objName] = dynamic_cast<Collider*>(collider);
}

///-------------------------------------
/// Brief: Adds a Circle Collider component to the appropriate map
void Space::AddCircleCollider(Component * circle, std::string objName)
{
  circles_[objName] = dynamic_cast<ColliderCircle*>(circle);
}

///-------------------------------------
/// Brief: Adds a Square Collider component to the appropriate map
void Space::AddSquareCollider(Component * square, std::string objName)
{
  squares_[objName] = dynamic_cast<ColliderSquare*>(square);
}

void Space::AddHealth(Component * health, std::string objName)
{
  healths_[objName] = dynamic_cast<Health*>(health);
}

///-------------------------------------
/// Brief: Adds a Physics component to the appropriate map
void Space::AddPhysics(Component * physics, std::string objName)
{
  physics_[objName] = dynamic_cast<Physics*>(physics);
}

///-------------------------------------
/// Brief: Adds a Sprite component to the appropriate map
void Space::AddSprite(Component * sprite, std::string objName)
{
  sprites_[objName] = dynamic_cast<Sprite*>(sprite);
}

///-------------------------------------
/// Brief: Adds a Transform component to the appropriate map
void Space::AddTransform(Component * transform, std::string objName)
{
  transforms_[objName] = dynamic_cast<Transform*>(transform);
}

void Space::AddStats(Component * stats, std::string objName)
{
  stats_[objName] = dynamic_cast<Stats*>(stats);
}

void Space::AddArchetype(Obj * obj)
{
  archetypes_[obj->Name()] = obj;
}

Obj * Space::GetArchetype(std::string type)
{
  return archetypes_[type];
}

ArchetypeList & Space::GetArchetypeList()
{
  return archetypes_;
}

///-------------------------------------
/// Brief: Returns a reference to the Object vector
ObjectList & Space::GetObjList()
{
  return ObjList;
}

///-------------------------------------
/// Brief: Adds any component to the appropriate map
void Space::AddComponent(Component * comp)
{
  if (comp->Type() >= Component::HasMap_)
    return;

  CompAdderP adder = compAdders_[comp->Type()];
  adder(comp, comp->Parent()->Name());

  if (comp->Type() == Component::Collider_)
  {
    ColliderCircle *circ = dynamic_cast<ColliderCircle*>(comp);
    ColliderSquare *squa = dynamic_cast<ColliderSquare*>(comp);

    if (circ)
      AddCircleCollider(comp, comp->Parent()->Name());
    else if (squa)
      AddSquareCollider(comp, comp->Parent()->Name());
  }
}

///-------------------------------------
/// Brief: Returns a Transform pointer by Obj name
Transform * Space::GetTransform(std::string objName)
{
  return transforms_[objName];
}

///-------------------------------------
/// Brief: Returns a Collider pointer by Obj name
Collider * Space::GetCollider(std::string objName)
{
  return colliders_[objName];
}

Physics * Space::GetPhysics(std::string objName)
{
  return physics_[objName];
}

ColliderCircle * Space::GetCircleCollider(std::string objName)
{
  return circles_[objName];
}

ColliderSquare * Space::GetSquareCollider(std::string objName)
{
  return squares_[objName];
}

Health * Space::GetHealth(std::string objName)
{
  return healths_[objName];
}

Stats * Space::GetStats(std::string objName)
{
  return stats_[objName];
}

ObjectList & Space::GetPartsLastList(ScreenManager *sMan)
{
  Screen *screen = sMan->CurrentScreen();
  std::string screenN = screen->GetName();
  if (screenN.compare(currScreen) == 0)
    return currPartsLast;

  int size = currPartsLast.size();
  for (int i = 0; i < size; ++i)
    currPartsLast[i] = nullptr;

  for (Obj *obj : partsLast_)
    if (obj->MapName().compare(screenN) == 0)
      currPartsLast.push_back(obj);
  currScreen = screenN;

  return currPartsLast;
}

void Space::AddPartsLast(Obj * obj)
{
  partsLast_.push_back(obj);
}

Player & Space::GetPlayer()
{
  return player_;
}

void ToggleDarkness()
{
  Obj* temp = Space::GetPlayer().GetPeepHole();
  bool b = !temp->IsActive();
  temp->IsActive(b);
  Lighting* light = Space::GetPlayer().Has(Lighting);
  light->Toggle(!light->Toggle());

  ScreensActive(b); //Remember to take out later >:0
}
