///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Serialization.h
//
//  Author: Megan Works 
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: serializes and deserializes game data
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "ScreenManager.h"
#include "document.h"
#include "FastChase.h"
#include "SlowChase.h"
#include "Door.h"
#include "Transform.h"
#include "Health.h"
#include "Trigger.h"
#include "EventComp.h"
#include "EventManager.h"
#include "PushBlockManager.h"
#include "Necromancer.h"

///----------------------------------GLOBALS------------------------------------
enum JSONFileType
{
  Object_,
  Component_,
  TiledMap_,
};

///----------------------------------CLASSES------------------------------------
class Data : public System
{
public:
  Data() : System("Data") {};
  ~Data();

  virtual void Initialize();
  virtual void Update(float dt) {}; // doesn't need to be updated
  void Shutdown();

  Obj* FromJSON(std::string filename);
  void ToJSON(Obj& object);
  void LoadScreen(std::string filename, std::string mapName);

  // for saving changes in editor
  void SaveScreen(Screen *screen);
  void SaveArchetype(Obj& object);

private:
  template <typename T>
  void loadScreenObjects(T &objects, Screen *screen);  // loads in objects in screen

  void* getMember(void * doc, const char * name);
};

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: loads in screen json data (maps)
template<typename T>
void Data::loadScreenObjects(T & objects, Screen * screen)
{
  using namespace rapidjson;

  assert(objects.IsArray());
  SizeType size = objects.Size();
  bool playerFound = false;
  PushBlock * puzzle = nullptr;

  if (screen->IsStartScreen()) // initializes player and adds to obj list
  {
    playerFound = true;
    Space::GetPlayer().Initialize();

    reinterpret_cast<Area*>(screen)->hasPlayer(true);
    ObjManagerAddObject(&Space::GetPlayer());
  }

  for (SizeType i = 0; i < size; i++)
  {
    Value &doc = objects[i];
    assert(doc.IsObject());

    // screen object info
    std::string name = (*static_cast<Value*>(getMember(&doc, "name"))).GetString();
    std::string type = (*static_cast<Value*>(getMember(&doc, "type"))).GetString();
    float x = (*static_cast<Value*>(getMember(&doc, "x"))).GetFloat();
    float y = (*static_cast<Value*>(getMember(&doc, "y"))).GetFloat();
    float scale = (*static_cast<Value*>(getMember(&doc, "scale"))).GetFloat();

    Obj *obj = nullptr;
    Transform* transform;
    Vec2D translation;
    translation.Set(x, y);

    // creates object copy with unique ID
    int count = screen->IncreaseObjectCount(type);
    name += (screen->GetID() + std::to_string(count));

    if (name.find("Trigger") != std::string::npos)
      obj = new Trigger(*Space::GetArchetype(type), name);
    else
      obj = new Obj(*Space::GetArchetype(type), name);

    //ObjManagerAddObject(obj);
    screen->AddObject(obj);

    if (name.compare("FirefliesL1") == 0)
      obj->PartsLast(true);

    obj->color_ = static_cast<colorFlag>((*static_cast<Value*>(getMember(&doc, "color"))).GetInt());

    obj->mapName_ = screen->GetName(); // what map object belongs to

    // sets transform
    transform = obj->Has(Transform);
    transform->Translation(translation);
    transform->OGTrans(translation);
    transform->MultScale(scale);
    transform->setDirty(true);
    transform->Update(0.0);

    Health* health = obj->Has(Health);
    Sprite* spritz = obj->Has(Sprite);
    Key* key = obj->Has(Key);
    ParticleEmitter* emitter = obj->Has(ParticleEmitter);
    Door *door = dynamic_cast<Door*>(obj->Has(Behavior));
    FastChase *fast = dynamic_cast<FastChase*>(obj->Has(Behavior));
    SlowChase *slow = dynamic_cast<SlowChase*>(obj->Has(Behavior));
    Necromancer *nec  = dynamic_cast<Necromancer*>(obj->Has(Behavior));
    float speed = 0.0f;

    Behavior *be = obj->Has(Behavior);

    Aggro* aggro = obj->Has(Aggro);
    ChangeMusic* music = obj->Has(ChangeMusic);
    FlashSprite* flash = obj->Has(FlashSprite);
    ColliderCircle* circle = dynamic_cast<ColliderCircle*>(obj->Has(Collider));
    ColliderSquare* square = dynamic_cast<ColliderSquare*>(obj->Has(Collider));
    Inspection* inspect = obj->Has(Inspection);
    SetActive* active = obj->Has(SetActive);
    SetDoor* door2 = obj->Has(SetDoor);
    SetInvincible* invince = obj->Has(SetInvincible);
    SetVisible* vis = obj->Has(SetVisible);
    Spawn* spawn = obj->Has(Spawn);
    ToggleDark *dark = obj->Has(ToggleDark);

    trigType trigT;
    resType collT;
    std::string s;
    std::string text;
    std::string ss;
    bool b;
    Obj *o = nullptr;

    if (obj->isTrigger_)
    {
      Trigger *trig = dynamic_cast<Trigger*>(obj);
      trigT = (trigType)((*static_cast<Value*>(getMember(&doc, "trigger type"))).GetInt());
      trig->TriggerType(trigT);
      if (trigT == OnCollision_)
      {
        s = (*static_cast<Value*>(getMember(&doc, "collision type"))).GetString();
        collT = GetResTypeFromString(s);

        trig->CollisionType(collT);
      }
      else if (trigT == OnRoomsComplete_)
      {
        std::vector<std::string>rooms;
        Value * arealist = static_cast<Value*>(getMember(&doc, "areas"));

        // reads in rooms needed
        Value::ConstMemberIterator it2End = arealist->MemberEnd();
        for (Value::ConstMemberIterator it2 = arealist->MemberBegin(); it2 != it2End; it2++)
        {
          rooms.push_back(it2->name.GetString());
        }

        trig->Rooms(rooms);
      }
      else if (trigT == OnTimer_)
      {
        float max = (*static_cast<Value*>(getMember(&doc, "timer max"))).GetFloat();
        Timer *tim = trig->Has(Timer);
        tim->MaxTime(max);
        tim->Time(max);
      }

      if (aggro)
      {
        aggro->GetInfoFromParent();
        s = (*static_cast<Value*>(getMember(&doc, "enemy name"))).GetString();
        b = (*static_cast<Value*>(getMember(&doc, "aggro bool"))).GetBool();

        BoolEventData data(s, b);
        aggro->SetData(data);
      }

      if (music)
      {
        music->GetInfoFromParent();
        s = (*static_cast<Value*>(getMember(&doc, "filename"))).GetString();
        bool play = (*static_cast<Value*>(getMember(&doc, "play"))).GetBool();

        SoundData data(s, play);
        music->Data(data);
      }

      if (flash)
      {
        flash->GetInfoFromParent();
        s = (*static_cast<Value*>(getMember(&doc, "flash name"))).GetString();
        secs dur = (*static_cast<Value*>(getMember(&doc, "duration"))).GetFloat();

        FlashData data(s, dur);
        flash->SetData(data);
      }

      if (inspect)
      {
        inspect->GetInfoFromParent();

        int l = 100;
        int h = 25;
        float t = 2.0f;

        text = (*static_cast<Value*>(getMember(&doc, "text"))).GetString();
        Value *v = static_cast<Value*>(getMember(&doc, "inspection obj"));
        if (v)
        {
          s = (*v).GetString();
          o = ObjManagerGetObject(s.c_str());
        }
        else
          o = &Space::GetPlayer();

        v = static_cast<Value*>(getMember(&doc, "length"));
        if (v)
          l = (*v).GetInt();

        v = static_cast<Value*>(getMember(&doc, "height"));
        if (v)
          h = (*v).GetInt();

        v = static_cast<Value*>(getMember(&doc, "time"));
        if (v)
          t = (*v).GetFloat();

        PopupParam param(text, o, l, h, t);
        inspect->SetData(param);
      }

      if (active)
      {
        active->GetInfoFromParent();
        s = (*static_cast<Value*>(getMember(&doc, "active name"))).GetString();
        b = (*static_cast<Value*>(getMember(&doc, "active bool"))).GetBool();

        BoolEventData data(s, b);
        active->SetData(data);
      }

      if (door2)
      {
        door2->GetInfoFromParent();
        s = (*static_cast<Value*>(getMember(&doc, "door state"))).GetString();
        dState d = GetDoorStateFromString(s);
        text = (*static_cast<Value*>(getMember(&doc, "door ID"))).GetString();
        ss = (*static_cast<Value*>(getMember(&doc, "screen name"))).GetString();

        DoorData data(d, text, ss);
        door2->SetData(data);
      }

      if (invince)
      {
        invince->GetInfoFromParent();
        s = (*static_cast<Value*>(getMember(&doc, "invincible name"))).GetString();
        b = (*static_cast<Value*>(getMember(&doc, "invincible bool"))).GetBool();

        BoolEventData data(s, b);
        invince->SetData(data);
      }

      if (vis)
      {
        vis->GetInfoFromParent();
        s = (*static_cast<Value*>(getMember(&doc, "visible name"))).GetString();
        b = (*static_cast<Value*>(getMember(&doc, "visible bool"))).GetBool();

        BoolEventData data(s, b);
        vis->SetData(data);
      }

      if (spawn)
      {
        spawn->GetInfoFromParent();

        float x;
        float y;
        bool byName = false;

        x = (*static_cast<Value*>(getMember(&doc, "spawn x"))).GetFloat();
        y = (*static_cast<Value*>(getMember(&doc, "spawn y"))).GetFloat();
        s = (*static_cast<Value*>(getMember(&doc, "spawn type"))).GetString();

        Value *v = static_cast<Value*>(getMember(&doc, "byName"));
        if (v)
          byName = (*v).GetBool();

        SpawnData data(Vec2D(x, y), s, byName);
        spawn->SetData(data);
      }

      if (dark)
      {
        dark->GetInfoFromParent();
      }

      bool active = (*static_cast<Value*>(getMember(&doc, "isActive"))).GetBool();
      obj->IsActive(active);
    }

    if (health)
    {
      int hp = (*static_cast<Value*>(getMember(&doc, "hp"))).GetInt();
      health->MaxHP(hp);

      bool vince = false;
      Value *v = static_cast<Value*>(getMember(&doc, "invincible"));
      if (v)
        vince = (*v).GetBool();
      health->Invincible(vince);
    }

    if (spritz)
    {
      glm::vec4 color;
      color.r = (*static_cast<Value*>(getMember(&doc, "r"))).GetFloat();
      color.g = (*static_cast<Value*>(getMember(&doc, "g"))).GetFloat();
      color.b = (*static_cast<Value*>(getMember(&doc, "b"))).GetFloat();
      color.a = (*static_cast<Value*>(getMember(&doc, "a"))).GetFloat();
	    bool temp = (*static_cast<Value*>(getMember(&doc, "visible"))).GetBool();
	    bool& thing = temp;
	    spritz->SetVisible(thing);
      spritz->SetRGBA(color);
    }

    if (be && door == nullptr && spritz && key == nullptr)
    {
      //bool active = (*static_cast<Value*>(getMember(&doc, "isActive"))).GetBool();
      //obj->IsActive(active);

      if (!spritz->GetVisible())
      {
        bool f = false;
        obj->IsActive(f);
      }
    }

    if (emitter)
    {
      glm::vec4 color;
      color.r = (*static_cast<Value*>(getMember(&doc, "Emitter R"))).GetFloat();
      color.g = (*static_cast<Value*>(getMember(&doc, "Emitter G"))).GetFloat();
      color.b = (*static_cast<Value*>(getMember(&doc, "Emitter B"))).GetFloat();
      color.a = (*static_cast<Value*>(getMember(&doc, "Emitter A"))).GetFloat();
      emitter->Color(color);

      Vec2D offset;
      offset.X((*static_cast<Value*>(getMember(&doc, "Emitter X"))).GetFloat());
      offset.Y((*static_cast<Value*>(getMember(&doc, "Emitter Y"))).GetFloat());
      emitter->Offset(offset);

      emitter->Radius((*static_cast<Value*>(getMember(&doc, "Emitter Radius"))).GetInt());
      emitter->Size((*static_cast<Value*>(getMember(&doc, "Emitter Size"))).GetFloat());
      emitter->Time(secs((*static_cast<Value*>(getMember(&doc, "Emitter Time"))).GetInt()));
      emitter->MaxParticles((*static_cast<Value*>(getMember(&doc, "Emitter Max"))).GetInt());

      emitter->Type(EmitterType((*static_cast<Value*>(getMember(&doc, "Emitter Type"))).GetInt()));
      emitter->Behavior(EmitterBehavior((*static_cast<Value*>(getMember(&doc, "Emitter Behavior"))).GetInt()));
    }

    if (key)
    {
      std::string keyID = (*static_cast<Value*>(getMember(&doc, "id"))).GetString();

      if (keyID.empty())
        keyID = "TEST";
        
      key->ID(keyID);
      
      Value *v = static_cast<Value*>(getMember(&doc, "isActive"));
      bool active = true;
      if(v)
        active = (*v).GetBool();
      obj->IsActive(active);
    }

    if (door)
    {
      door->ID = (*static_cast<Value*>(getMember(&doc, "id"))).GetString();
      door->area = (*static_cast<Value*>(getMember(&doc, "area"))).GetString();
      int status = (*static_cast<Value*>(getMember(&doc, "status"))).GetInt();
      int wall = (*static_cast<Value*>(getMember(&doc, "wall"))).GetInt();

      door->wall = static_cast<ScreenManager::direction>(wall);
      door->StartState(static_cast<dState>(status));
    }

    if (square)
    {
      Vec2D dim;
      dim.X((*static_cast<Value*>(getMember(&doc, "Collider Width"))).GetFloat());
      dim.Y((*static_cast<Value*>(getMember(&doc, "Collider Height"))).GetFloat());
      square->Dimensions(dim);
    }

    if (slow)
    {
      speed = (*static_cast<Value*>(getMember(&doc, "speed"))).GetFloat();
    }
    else if (fast)
    {
      speed = (*static_cast<Value*>(getMember(&doc, "speed"))).GetFloat();
    }
    else if(nec)
      speed = (*static_cast<Value*>(getMember(&doc, "speed"))).GetFloat();

    // checks if object is any kind of block and adds to push block puzzle
    if (type.find("PushBlock") != -1)
    {
      if (!puzzle)
        puzzle = new PushBlock();

      puzzle->GetPushBlocks().push_back(obj);
    }
    else if (type.find("WinBlock") != -1)
    {
      if (!puzzle)
        puzzle = new PushBlock();

      puzzle->GetWinBlocks().push_back(obj);
    }
    else if (type.compare("TickTock") == 0)
      AddTickTockToTickTocks(name);
    else if (obj->PartsLast())
      Space::AddPartsLast(obj);
  }

  // area only object related stuff 
  if (screen->GetType() == Screen::ScreenType::Area_)
  {
    if (puzzle != nullptr)
    {
      PushBlockManager *pbm = dynamic_cast<PushBlockManager*>(EngineGetSystem("PushBlockManager"));
      reinterpret_cast<Area*>(screen)->SetPushBlockPuzzle(puzzle);

      pbm->saveStartPositions(puzzle);
      pbm->saveWinPositions(puzzle);
    }
  }
}
