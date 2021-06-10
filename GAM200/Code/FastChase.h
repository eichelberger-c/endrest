///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FastChase.h
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief: A behavior "script" that chases the player quickly and dashes when close
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
//#include "Spaces.h"
#include "BehaviorScripts.h"
#include "Physics.h"
#include "Transform.h"
//#include "Vec2D.h"
//#include "Obj.h"
#include "ScreenManager.h"
#include "EventManager.h"
#include "Engine.h"

///----------------------------------GLOBALS------------------------------------
typedef enum FastChaseStates
{
  ///REQUIRED:
  fcInvalid,
  fcFlee,
  fcChase,
  fcPause,
  fcDash,

} fcState;

///----------------------------------CLASSES------------------------------------
class FastChase : public BehaviorScript
{
public:

  FastChase(std::string name = "FastChase", float radius = 400.0f); //always use the default DO NOT CHANGE THE NAME
  FastChase(FastChase& ogFastChase);

  Player *play;
  Timer *tim;

  float chaseSpeed;
  float dashSpeed;
  float range;
  float timerDash = 4.0f;
  float timerPause = 2.5f;
  float pauseLimit;

  virtual void Reset() { tim->Reset(); }

  virtual void Start() override
  {
    scn = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
    Player& p = Space::GetPlayer();
    play = &p;

    chaseSpeed = 21.0f;
    dashSpeed = 30.0f;

    Timer& t = Tim();
    tim = &t;
    tim->MaxTime(timerDash);
    pauseLimit = timerDash - timerPause;

    StateCurrent(fcInvalid);
    StateNext(fcChase);
    range = 220.0f;
  }

  virtual void SpecialUpdate(float dt) override
  {
    if (!IsAggroed())
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

        //scn->AddAggro(); // this needs to be after sfx has played -- mw
      }
      else
        return;
    }

		ColliderCircle *collider = Space::GetCircleCollider(Parent()->Name());
		if (collider->Collided())
			return;

    fcState state = (fcState)StateCurrent();
    Vec2D myPos = { 0 };
    Vec2D playerPos = { 0 };
    Vec2D diff = { 0 };
    Vec2D diff2 = { 0 };

    switch (state)
    {
    case fcInvalid:
      break;
    case fcChase:
      myPos = Parent()->Has(Transform)->Translation();
      playerPos = Space::GetPlayer().Has(Transform)->Translation();

      diff = playerPos.Abs() - myPos.Abs();
      diff2 = playerPos - myPos;
      if (diff.Abs() <= range)
      {
        tim->Reset();
        StateNext(fcPause);
      }

      ~diff2;

      diff2 *= chaseSpeed;
      Parent()->Has(Physics)->Velocity(diff2);
      break;
    case fcDash:
      //tim->Adjust(-dt);
      tim->Update(dt);

      if (tim->Time() <= 0.0f)
      {
        tim->Reset();
        StateNext(fcChase);
      }

      myPos = Parent()->Has(Transform)->Translation();
      playerPos = Space::GetPlayer().Has(Transform)->Translation();

      diff = playerPos - myPos;
      ~diff;

      diff *= dashSpeed;
      Parent()->Has(Physics)->Velocity(diff);
      break;
    case fcPause:
      Parent()->Has(Physics)->Velocity(0.0f, 0.0f);
      //tim->Adjust(-dt);
      tim->Update(dt);

      if (tim->Time() <= pauseLimit)
      {
        tim->Reset();
        StateNext(fcDash);
      }
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
    EventManager *man = dynamic_cast<EventManager*>(EngineGetSystem("EventManager"));
    Vec2D pos = Parent()->Has(Transform)->Translation();
    SpawnData data(pos, "PhantomBlood");

    man->spawn(data);
  }

  void ChaseSpeed(float val);
  void DashSpeed(float val);
  void DashTimer(float val);
  void PauseTimer(float val);
  void Range(float val);

  private:
    ScreenManager *scn;
};

///---------------------------------FUNCTIONS-----------------------------------


