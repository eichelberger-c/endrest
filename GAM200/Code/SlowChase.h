///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SlowChase.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: A behavior "script" that //fill this in 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
//#include "Spaces.h"
//#include "Behavior.h"
#include "BehaviorScripts.h"
//#include "Vec2D.h"
#include "Transform.h"
//#include "Obj.h"
#include "ColliderCircle.h"
#include "EventManager.h"
#include "Engine.h"

///----------------------------------GLOBALS------------------------------------
typedef enum SlowChaseStates
{
  ///REQUIRED:
  scInvalid,
  scFlee,
  scChase,

} scState;

///----------------------------------CLASSES------------------------------------
class SlowChase : public BehaviorScript
{
public:

  SlowChase(std::string name = "SlowChase", float radius = 250.0f); //always use the default DO NOT CHANGE THE NAME
  SlowChase(SlowChase& ogSlowChase);

  float speed;
  Player *play;
  Timer *tim;
  Vec2D myPos;
  Vec2D playerPos;
  Vec2D diff;

  virtual void Start() override
  {
    scn = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

    Player& p = Space::GetPlayer();
    play = &p;
    Timer& t = Tim();
    tim = &t;

    speed = 15.0f;
    StateCurrent(scInvalid);
    StateNext(scChase);
  }

  virtual void SpecialUpdate(float dt) override
  {
    if (!IsAggroed()/* && Parent()->Has(Sprite)->GetVisible()*/)
    {
      Vec2D pos = Parent()->Has(Transform)->Translation();
      Vec2D player = play->Has(Transform)->Translation();

      if (pos.IsInRange(player, AggroRadius()))
      {
        bool b = true;
        IsAggroed(b);

        // plays aggro sfx
        Audio *audio = Parent()->Has(Audio);
        if (audio)
          audio->Play(Audio::Aggro_);
      }
      else
        return;
    }

		ColliderCircle *collider = Space::GetCircleCollider(Parent()->Name());
		//if (collider->Collided())
			//return;

    scState state = (scState)StateCurrent();
    switch (state)
    {
    case scInvalid:
      break;
    case scChase:
      myPos = Parent()->Has(Transform)->Translation();
      playerPos = Space::GetPlayer().Has(Transform)->Translation();

      diff = GetFollow(myPos, playerPos, speed);
      Parent()->Has(Physics)->Velocity(diff);
      break;
    case scFlee:
      tim->Update(dt);

      if (tim->Beep())
      {
        tim->Reset();
        //ColliderCircle *col = dynamic_cast<ColliderCircle*>(Parent()->Has(Collider));
        //if (!col->Collided())
        StateNext(scChase);
      }

      myPos = Parent()->Has(Transform)->Translation();
      playerPos = Space::GetPlayer().Has(Transform)->Translation();

      diff = GetFollow(myPos, playerPos, speed);
      Parent()->Has(Physics)->Velocity(-diff);
      break;
    }
  }

  virtual void Exit() override
  {

  }

  virtual void OnCollisionWithPlayer() override
  {

  }

  virtual void OnCollision(Collider& other) override
  {

  }

  virtual void OnDeactivate() override
  {
    //EventManager *man = dynamic_cast<EventManager*>(EngineGetSystem("EventManager"));
    //Vec2D pos = Parent()->Has(Transform)->Translation();
    //SpawnData data(pos, "BansheeTears");

    //man->spawn(data);
  }

  void Speed(float val);

  private:
    ScreenManager *scn;
};

///---------------------------------FUNCTIONS-----------------------------------

