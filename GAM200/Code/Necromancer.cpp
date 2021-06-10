///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Necromancer.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2018 DigiPen (USA) Corporation
//
//  Functions:
//      - Necromancer
//      - Necromancer (copy constructor)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Necromancer.h"
//#include "EventManager.h"
//#include "Trigger.h"
//#include "Spaces.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
Necromancer::Necromancer(std::string name)
{
  Name(name);
  AggroRadius(360.0f);
  Start();
}

Necromancer::Necromancer(Necromancer& ogNecromancer)
{
  Name(ogNecromancer.Name());
  AggroRadius(ogNecromancer.AggroRadius());
  Start();
}

void Necromancer::ChaseSpeed(float val)
{
  chaseSpeed = val;
}

void Necromancer::DashSpeed(float val)
{
  dashSpeed = val;
}

void Necromancer::DashTimer(float val)
{
  timerDash = val;
}

void Necromancer::PauseTimer(float val)
{
  timerPause = val;

  pauseLimit = timerDash - timerPause;
}

void Necromancer::Range(float val)
{
  range = val;
}

//void Necromancer::CallFunction(bool set)
//{
//  callFunction = set;
//}

//void Necromancer::Start()
//{
//  Player& p = Space::GetPlayer();
//  play = &p;
//
//  chaseSpeed = 12.0f;
//  dashSpeed = 30.0f;
//
//  Timer& t = Tim();
//  tim = &t;
//  tim->MaxTime(timerDash);
//  pauseLimit = timerDash - timerPause;
//
//  StateCurrent(nInvalid);
//  StateNext(nChase);
//  range = 220.0f;
//
//  tim2.MaxTime(3.0f);
//  tim3.MaxTime(4.0f);
//  tim4.MaxTime(3.0f);
//  tim2Done = false;
//  tim3Done = false;
//  dialogueTime = false;
//  callFunction = false;
//
//  box1 = dynamic_cast<Trigger*>(ObjManagerGetObject("Trigger16L1"));
//  box2 = dynamic_cast<Trigger*>(ObjManagerGetObject("Trigger17L1"));
//}

//void Necromancer::SpecialUpdate(float dt)
//{
//  if (callFunction)
//  {
//    BossRoomEvent(true);
//    callFunction = false;
//    dialogueTime = true;
//  }
//
//  if (dialogueTime)
//  {
//    if (!tim2Done)
//      tim2.Update(dt);
//    else if (!tim3Done)
//      tim3.Update(dt);
//    else
//      tim4.Update(dt);
//
//    if (tim2.Beep() && !tim2Done)
//    {
//      tim2Done = true;
//      box1->DoTheEvents();
//    }
//
//    if (tim3.Beep() && !tim3Done)
//    {
//      tim3Done = true;
//      box2->DoTheEvents();
//    }
//
//    if (tim4.Beep())
//    {
//      dialogueTime = false;
//      BossRoomEvent(false);
//    }
//  }
//
//  if (!IsAggroed())
//  {
//    Vec2D pos = Parent()->Has(Transform)->Translation();
//    Vec2D player = play->Has(Transform)->Translation();
//
//    if (pos.IsInRange(player, AggroRadius()))
//    {
//      bool b = true;
//      IsAggroed(b);
//
//      // plays aggro sfx
//      Audio *audio = Parent()->Has(Audio);
//      if (audio)
//        audio->Play(Audio::Aggro_);
//
//      //scn->AddAggro(); // this needs to be after sfx has played -- mw
//    }
//    else
//      return;
//  }
//
//  ColliderCircle *collider = Space::GetCircleCollider(Parent()->Name());
//  if (collider->Collided())
//    return;
//
//  nState state = (nState)StateCurrent();
//  Vec2D myPos = { 0 };
//  Vec2D playerPos = { 0 };
//  Vec2D diff = { 0 };
//  Vec2D diff2 = { 0 };
//
//  switch (state)
//  {
//  case nInvalid:
//    break;
//  case nChase:
//    myPos = Parent()->Has(Transform)->Translation();
//    playerPos = Space::GetPlayer().Has(Transform)->Translation();
//
//    diff = playerPos.Abs() - myPos.Abs();
//    diff2 = playerPos - myPos;
//    if (diff.Abs() <= range)
//    {
//      tim->Reset();
//      StateNext(nPause);
//    }
//
//    ~diff2;
//
//    diff2 *= chaseSpeed;
//    Parent()->Has(Physics)->Velocity(diff2);
//    break;
//  case nDash:
//    //tim->Adjust(-dt);
//    tim->Update(dt);
//
//    if (tim->Time() <= 0.0f)
//    {
//      tim->Reset();
//      StateNext(nChase);
//    }
//
//    myPos = Parent()->Has(Transform)->Translation();
//    playerPos = Space::GetPlayer().Has(Transform)->Translation();
//
//    diff = playerPos - myPos;
//    ~diff;
//
//    diff *= dashSpeed;
//    Parent()->Has(Physics)->Velocity(diff);
//    break;
//  case nPause:
//    Parent()->Has(Physics)->Velocity(0.0f, 0.0f);
//    //tim->Adjust(-dt);
//    tim->Update(dt);
//
//    if (tim->Time() <= pauseLimit)
//    {
//      tim->Reset();
//      StateNext(nDash);
//    }
//    break;
//  }
//}
