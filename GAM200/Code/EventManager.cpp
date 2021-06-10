///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: EventManager.cpp
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
#include "stdafx.h"
#include "BehaviorScripts.h"
#include "SlowChase.h"
#include "FastChase.h"
#include "MimicPlayer.h"
#include "EventManager.h"
#include "Engine.h"
#include "Collect.h"
#include "Door.h"
#include "Trigger.h"
#include "Component.h"
#include "EventComp.h"
#include "Spawner.h"
#include "System.h"
#include "Menu.h"
#include "ScreenManager.h"
#include "Necromancer.h"

///----------------------------------GLOBALS-----------------------------------
EventManager *eMan;

Player *player;
Score *score;
Health *hp;

std::vector<std::string> tickTocks;

///---------------------------------FUNCTIONS-----------------------------------
void SetTickTockVisible(bool& vis)
{
  for (std::string s : tickTocks)
    eMan->SendSetVisibleEvent(s, vis);
}

void AddTickTockToTickTocks(std::string name)
{
  tickTocks.push_back(name);
}

//void NecromancerCallFunction(bool& set)
//{
//  Obj *nec = ObjManagerGetObject("NecromancerL1");
//  Necromancer *n = dynamic_cast<Necromancer*>(nec->Has(Behavior));
//  n->CallFunction(set);
//}


EventManager::EventManager() : System("EventManager")
{
  eMan = this;
}

EventManager::~EventManager()
{
  Player& p = Space::GetPlayer();
  player = &p;
  score = player->Has(Score);
  hp = player->Has(Health);
  ScreenManager *sMan = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

  spawn -= EventZone::EventHandler::Bind(SpawnFromEvent);
  setDoor -= EventZone::EventHandler::Bind(&ScreenManager::SetDoor, sMan);
  popup -= EventZone::EventHandler::Bind(PopupMenu);
  setSound -= EventZone::EventHandler::Bind(SoundEvent);
  toggleDark -= EventZone::EventHandler::Bind(ToggleDarkness);
  catPlus -= EventZone::EventHandler::Bind(&Score::IncreaseCatsBy1, score);
  catMin -= EventZone::EventHandler::Bind(&Score::DecreaseCatsBy1, score);
  cats -= EventZone::EventHandler::Bind(&Score::AdjustCatsFromEvent, score);
  playerHP -= EventZone::EventHandler::Bind(&Health::AdjustHP, hp);

  setTickTocks -= EventZone::EventHandler::Bind(SetTickTockVisible);
  //bossEvent -= EventZone::EventHandler::Bind(NecromancerCallFunction);

  ObjectList& objs = Space::GetObjList();

  //loop thru and check for any objs with behaviors
  int size = objs.size();
  BehaviorScript* behav = nullptr;
  SlowChase* slow = nullptr;
  FastChase* fast = nullptr;
  MimicPlayer* mim = nullptr;
  Collect* col = nullptr;
  Door *dor = nullptr;
  Necromancer *nec = nullptr;

  Sprite *s = nullptr;
  Collider *c = nullptr;
  Health *h = nullptr;

  for (int i = 0; i < size; ++i)
  {
    std::string& name = objs[i]->Name();

    behav = dynamic_cast<BehaviorScript*>((objs[i]->Has(Behavior)));
    if (behav)
    {
      //playerCollisionEvents_[objs[i]->Name()] -= EventZone::EventHandler::Bind(&BehaviorScript::OnCollisionWithPlayer, );
      slow = dynamic_cast<SlowChase*>(behav);
      fast = dynamic_cast<FastChase*>(behav);
      mim = dynamic_cast<MimicPlayer*>(behav);
      col = dynamic_cast<Collect*>(behav);
      dor = dynamic_cast<Door*>(behav);
      nec = dynamic_cast<Necromancer*>(behav);
      
      //connect function to event with obj name
      if (slow != nullptr)
      {
        //playerCollisionEvents_[objs[i]->Name()] = new EventZone::Event<void>;
        (playerCollisionEvents_[name]) -= EventZone::EventHandler::Bind(&SlowChase::OnCollisionWithPlayer, slow);
        (collisionEvents_[name]) -= EventZone::EventHandler::Bind(&SlowChase::OnCollision, slow);
        (deactivateEvents_[name]) -= EventZone::EventHandler::Bind(&SlowChase::OnDeactivate, slow);

        isAggroedEvents_[name] -= EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);
      }
      else if (fast != nullptr)
      {
        (playerCollisionEvents_[name]) -= EventZone::EventHandler::Bind(&FastChase::OnCollisionWithPlayer, fast);
        (collisionEvents_[name]) -= EventZone::EventHandler::Bind(&FastChase::OnCollision, fast);
        (deactivateEvents_[name]) -= EventZone::EventHandler::Bind(&FastChase::OnDeactivate, fast);

        isAggroedEvents_[name] -= EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);
      }
      else if (mim != nullptr)
      {
        (playerCollisionEvents_[name]) -= EventZone::EventHandler::Bind(&MimicPlayer::OnCollisionWithPlayer, mim);
        (collisionEvents_[name]) -= EventZone::EventHandler::Bind(&MimicPlayer::OnCollision, mim);
        (deactivateEvents_[name]) -= EventZone::EventHandler::Bind(&MimicPlayer::OnDeactivate, mim);

        isAggroedEvents_[name] -= EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);
      }
      else if (nec != nullptr)
      {
        (playerCollisionEvents_[name]) -= EventZone::EventHandler::Bind(&Necromancer::OnCollisionWithPlayer, nec);
        (collisionEvents_[name]) -= EventZone::EventHandler::Bind(&Necromancer::OnCollision, nec);
        (deactivateEvents_[name]) -= EventZone::EventHandler::Bind(&Necromancer::OnDeactivate, nec);

        isAggroedEvents_[name] -= EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);
      }
      else if (col != nullptr)
      {
        (playerCollisionEvents_[name]) -= EventZone::EventHandler::Bind(&Collect::OnCollisionWithPlayer, col);
        (collisionEvents_[name]) -= EventZone::EventHandler::Bind(&Collect::OnCollision, col);
      }
      else if (dor != nullptr)
      {
        (playerCollisionEvents_[name]) -= EventZone::EventHandler::Bind(&Door::OnCollisionWithPlayer, dor);
        (collisionEvents_[name]) -= EventZone::EventHandler::Bind(&Door::OnCollision, dor);
        //(deactivateEvents_[name]) -= EventZone::EventHandler::Bind(&Door::OnDeactivate, dor);
      }
    }

    resType rType = objs[i]->ResolutionType();
    c = objs[i]->Has(Collider);
    s = objs[i]->Has(Sprite);
    if ((!c && s) || (s && rType == No_))
      setVisibleEvents_[name] -= EventZone::EventHandler::Bind(&Sprite::SetVisible, s);

    if (c && rType != Other_ && rType != No_ && rType != Wall_)
      setActiveEvents_[name] -= EventZone::EventHandler::Bind(&Obj::IsActive, objs[i]);

    h = objs[i]->Has(Health);
    if (h)
      setInvincibleEvents_[name] += EventZone::EventHandler::Bind(&Health::Invincible, h);
  }
}

void EventManager::Initialize()
{
  numEventComps_ = 0;

  Player& p = Space::GetPlayer();
  player = &p;
  score = player->Has(Score);
  hp = player->Has(Health);
  ScreenManager *sMan = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

  spawn += EventZone::EventHandler::Bind(SpawnFromEvent);
  setDoor += EventZone::EventHandler::Bind(&ScreenManager::SetDoor, sMan);
  popup += EventZone::EventHandler::Bind(PopupMenu);
  setSound += EventZone::EventHandler::Bind(SoundEvent); 
  toggleDark += EventZone::EventHandler::Bind(ToggleDarkness);
  catPlus += EventZone::EventHandler::Bind(&Score::IncreaseCatsBy1, score);
  catMin += EventZone::EventHandler::Bind(&Score::DecreaseCatsBy1, score);
  cats += EventZone::EventHandler::Bind(&Score::AdjustCatsFromEvent, score);
  playerHP += EventZone::EventHandler::Bind(&Health::AdjustHP, hp);

  setTickTocks += EventZone::EventHandler::Bind(SetTickTockVisible);
  //bossEvent += EventZone::EventHandler::Bind(NecromancerCallFunction);

  //get object list from space
  ObjectList& objs = Space::GetObjList();

  //loop thru and check for any objs with behaviors
  int size = objs.size();
  BehaviorScript* behav = nullptr;
  SlowChase* slow = nullptr;
  FastChase* fast = nullptr;
  MimicPlayer* mim = nullptr;
  Collect* col = nullptr;
  Door *dor = nullptr;
  Necromancer *nec = nullptr;

  Sprite *s = nullptr;
  Collider *c = nullptr;
  Health *h = nullptr;

  for (int i = 0; i < size; ++i)
  {
    std::string& name = objs[i]->Name();

    behav = dynamic_cast<BehaviorScript*>((objs[i]->Has(Behavior)));
    if (behav)
    {
      slow = dynamic_cast<SlowChase*>(behav);
      fast = dynamic_cast<FastChase*>(behav);
      mim = dynamic_cast<MimicPlayer*>(behav);
      col = dynamic_cast<Collect*>(behav);
      dor = dynamic_cast<Door*>(behav);
      nec = dynamic_cast<Necromancer*>(behav);

      //connect function to event with obj name
      if (slow != nullptr)
      {
        (playerCollisionEvents_[name]) += EventZone::EventHandler::Bind(&SlowChase::OnCollisionWithPlayer, slow);
        (collisionEvents_[name]) += EventZone::EventHandler::Bind(&SlowChase::OnCollision, slow);
        (deactivateEvents_[name]) += EventZone::EventHandler::Bind(&SlowChase::OnDeactivate, slow);

        isAggroedEvents_[name] += EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);
      }
      else if (fast != nullptr)
      {
        (playerCollisionEvents_[name]) += EventZone::EventHandler::Bind(&FastChase::OnCollisionWithPlayer, fast);
        (collisionEvents_[name]) += EventZone::EventHandler::Bind(&FastChase::OnCollision, fast);
        (deactivateEvents_[name]) += EventZone::EventHandler::Bind(&FastChase::OnDeactivate, fast);

        isAggroedEvents_[name] += EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);
      }
      else if (mim != nullptr)
      {
        (playerCollisionEvents_[name]) += EventZone::EventHandler::Bind(&MimicPlayer::OnCollisionWithPlayer, mim);
        (collisionEvents_[name]) += EventZone::EventHandler::Bind(&MimicPlayer::OnCollision, mim);
        (deactivateEvents_[name]) += EventZone::EventHandler::Bind(&MimicPlayer::OnDeactivate, mim);

        isAggroedEvents_[name] += EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);
      }
      else if (nec != nullptr)
      {
        (playerCollisionEvents_[name]) += EventZone::EventHandler::Bind(&Necromancer::OnCollisionWithPlayer, nec);
        (collisionEvents_[name]) += EventZone::EventHandler::Bind(&Necromancer::OnCollision, nec);
        (deactivateEvents_[name]) += EventZone::EventHandler::Bind(&Necromancer::OnDeactivate, nec);

        isAggroedEvents_[name] += EventZone::EventHandler::Bind(&BehaviorScript::IsAggroed, behav);

        //nec->box1 = dynamic_cast<Trigger*>(ObjManagerGetObject("Trigger16L1"));
        //nec->box2 = dynamic_cast<Trigger*>(ObjManagerGetObject("Trigger17L1"));
      }            
      else if (col != nullptr)
      {
        (playerCollisionEvents_[name]) += EventZone::EventHandler::Bind(&Collect::OnCollisionWithPlayer, col);
        (collisionEvents_[name]) += EventZone::EventHandler::Bind(&Collect::OnCollision, col);
      }
      else if (dor != nullptr)
      {
        (playerCollisionEvents_[name]) += EventZone::EventHandler::Bind(&Door::OnCollisionWithPlayer, dor);
        (collisionEvents_[name]) += EventZone::EventHandler::Bind(&Door::OnCollision, dor);
        //(deactivateEvents_[name]) += EventZone::EventHandler::Bind(&Door::OnDeactivate, dor);
      }
    }

    resType rType = objs[i]->ResolutionType();
    c = objs[i]->Has(Collider);
    s = objs[i]->Has(Sprite);
    if ((!c && s) || (s && rType == No_))
      setVisibleEvents_[name] += EventZone::EventHandler::Bind(&Sprite::SetVisible, s);

    if (c && rType != Other_ && rType != No_ && rType != Wall_)
      setActiveEvents_[name] += EventZone::EventHandler::Bind(&Obj::IsActive, objs[i]);

    h = objs[i]->Has(Health);
    if (h)
      setInvincibleEvents_[name] += EventZone::EventHandler::Bind(&Health::Invincible, h);

    if (objs[i]->IsTrigger())
    {
      for (Component *comp : objs[i]->components_)
      {
        if (comp->Type() > Component::ChangeMusic_)
        {
          if (comp->Type() == Component::SetVisible_)
          {
            EventComp<bool> *eComp = static_cast<EventComp<bool>*>(comp);
            if (eComp->IsCustomized())
            {
              //if (objs[i]->Name().compare("Trigger18L1") == 0)
              //  eComp->Customize(&bossEvent);
              //else
              eComp->Customize(&setTickTocks);
            }
            else
              comp->ReGetEvent();
          }
          else
            comp->ReGetEvent();
        }
      }
    }
  }
}

void EventManager::Update(float dt)
{
}

EventManager * EventManager::GetInstance()
{
  return eMan;
}

void EventManager::SendPlayerCollisionEvent(std::string objName)
{
  playerCollisionEvents_[objName]();
}

void EventManager::SendCollisionEvent(std::string objName, Collider & other)
{
  //using namespace std;

  //if (objName.find("Trigger") != string::npos)
  //{
  //  pair<multimap<string, EventZone::Event<Collider>>::iterator, multimap<string, EventZone::Event<Collider>>::iterator> ret;
  //  ret = compCollisionEvents_.equal_range(objName);
  //  for (multimap<string, EventZone::Event<Collider>>::iterator it = ret.first; it != ret.second; ++it)
  //  {
  //    it->second(other);
  //  }
  //}
  //else
  collisionEvents_[objName](other);
}

void EventManager::SendDeactivateEvent(std::string objName)
{
  deactivateEvents_[objName]();
}

void EventManager::SendSetVisibleEvent(std::string objName, bool vis)
{
  setVisibleEvents_[objName](vis);
}

void EventManager::SendSetActiveEvent(std::string objName, bool active)
{
  setActiveEvents_[objName](active);
}

//void EventManager::SendCompCollisionEvent(int ID)
//{
//}

void EventManager::IncreaseEventComps()
{
  numEventComps_ += 1;
}

int EventManager::NumEventComps()
{
  return numEventComps_;
}

EventZone::Event<bool>* EventManager::GetIsAggroedEvent(std::string enemyName)
{
  if (enemyName.compare("") == 0)
    return nullptr;

  return &(isAggroedEvents_[enemyName]);
}

EventZone::Event<bool>* EventManager::GetSetVisibleEvent(std::string objName)
{
  if (objName.compare("") == 0)
    return nullptr;

  return &(setVisibleEvents_[objName]);
}

EventZone::Event<bool>* EventManager::GetSetActiveEvent(std::string objName)
{
  if (objName.compare("") == 0)
    return nullptr;

  return &(setActiveEvents_[objName]);
}

EventZone::Event<bool>* EventManager::GetSetInvincibleEvent(std::string objName)
{
  if (objName.compare("") == 0)
    return nullptr;

  return &(setInvincibleEvents_[objName]);
}

void BossRoomEvent(bool start)
{
  if (start)
  {
    ScreensActive(false);
    ToggleDarkness();
  }
  else
  {
    ToggleDarkness();
    ScreensActive(true);
  }
}
