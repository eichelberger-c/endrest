///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Necromancer.h
//
//  Author: Sonya Toft
//
//  Copyright � 2018 DigiPen (USA) Corporation
//
//  Brief: A behavior "script" that //fill this in 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "BehaviorScripts.h"
#include "Spaces.h"
#include "EventManager.h"
#include "Trigger.h"
#include "EndGs.h"

///----------------------------------GLOBALS------------------------------------
typedef enum NecromancerStates
{
  nInvalid,
  nChase,
  nPause,
  nDash,

} nState;

///----------------------------------CLASSES------------------------------------
class Necromancer : public BehaviorScript
{
public:

  Necromancer(std::string name = "Necromancer"); //always use the default DO NOT CHANGE THE NAME
  Necromancer(Necromancer& ogNecromancer);

  Player *play;
  Timer *tim;

  float chaseSpeed;
  float dashSpeed;
  float range;
  float timerDash = 5.0f;
  float timerPause = 2.0f;
  float pauseLimit;

  virtual void Reset() { tim->Reset(); }

  virtual void Start() override
  {
    Player& p = Space::GetPlayer();
    play = &p;

    chaseSpeed = 12.0f;
    dashSpeed = 30.0f;

    Timer& t = Tim();
    tim = &t;
    tim->MaxTime(timerDash);
    pauseLimit = timerDash - timerPause;

    StateCurrent(nInvalid);
    StateNext(nChase);
    range = 220.0f;

    onHit.text = "Please, my love...";
    onHit.length = 140;
    onHit.height = 30;
    onHit.obj = Parent();
  }

  virtual void SpecialUpdate(float dt) override
  {
    if (!ScreensActive())
      return;

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

    nState state = (nState)StateCurrent();
    Vec2D myPos = { 0 };
    Vec2D playerPos = { 0 };
    Vec2D diff = { 0 };
    Vec2D diff2 = { 0 };

    switch (state)
    {
    case nInvalid:
      break;
    case nChase:
      myPos = Parent()->Has(Transform)->Translation();
      playerPos = Space::GetPlayer().Has(Transform)->Translation();

      diff = playerPos.Abs() - myPos.Abs();
      diff2 = playerPos - myPos;
      if (diff.Abs() <= range)
      {
        tim->Reset();
        StateNext(nPause);
      }

      ~diff2;

      diff2 *= chaseSpeed;
      Parent()->Has(Physics)->Velocity(diff2);
      break;
    case nDash:
      tim->Update(dt);

      if (tim->Time() <= 0.0f)
      {
        tim->Reset();
        StateNext(nChase);
      }

      myPos = Parent()->Has(Transform)->Translation();
      playerPos = Space::GetPlayer().Has(Transform)->Translation();

      diff = playerPos - myPos;
      ~diff;

      diff *= dashSpeed;
      Parent()->Has(Physics)->Velocity(diff);
      break;
    case nPause:
      Parent()->Has(Physics)->Velocity(0.0f, 0.0f);
      //tim->Adjust(-dt);
      tim->Update(dt);

      if (tim->Time() <= pauseLimit)
      {
        tim->Reset();
        StateNext(nDash);
      }
      break;
    }
  }

  virtual void Exit() override
  {

  }

  virtual void OnCollisionWithPlayer() override
  {
    //EventManager::GetInstance()->popup(onHit);
  }

  virtual void OnCollision(Collider& other) override
  {

  }

  virtual void OnDeactivate() override
  {
    Player& player = Space::GetPlayer();
    player.GetFade()->SetOutro();
  }

  void ChaseSpeed(float val);
  void DashSpeed(float val);
  void DashTimer(float val);
  void PauseTimer(float val);
  void Range(float val);


  private:
    PopupParam onHit;
};

///---------------------------------FUNCTIONS-----------------------------------


