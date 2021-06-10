///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Serialization.cpp
//
//  Author: Megan Works
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - FillDocument
//      - ~Data
//      - Initialize
//      - Shutdown
//      - FromJSON
//      - ToJSON
//      - LoadScreen
//      - SaveScreen 
//      - SaveArchetype
//      - getMember (private)
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include <fstream>
#include "ObjFactory.h"
#include "Engine.h"
#include "CompCreator.h"
#include "BehaviorScripts.h"
#include "Serialization.h"
#include "FileUtils.h"
#include "istreamwrapper.h"
#include "Trigger.h"

///----------------------------------GLOBALS-----------------------------------
using namespace rapidjson;

// holds names of types -- used for debugging
static const char *rapidJSONType[] = { "NULL", "FALSE", "TRUE", "OBJECT", "ARRAY", "STRING", "NUMBER" };


///-------------------------------------
/// Brief: Parses json file into a document
///        returns parsed document (or exits)
static Document FillDocument(std::string filename)
{
  // reads in json file as stream
  std::ifstream inFile(filename);

  // wraps the stream (if slow switch to filereadstream)
  IStreamWrapper ISW(inFile);
  Document document;

  // attempts to parse stream into DOM tree
  ParseErrorCode error = document.ParseStream(ISW).GetParseError();

  // parsing unsuccessful - check error code
  if (error)
  {
    TraceMessage(TraceLog, "File %s had parsing error: %i", filename.c_str(), error);
    exit(-1);
  }

  // parsing was successful
  TraceMessage(TraceLog, "File %s was parsed successfully", filename.c_str());

  // documents are required to be objects
  if (!document.IsObject())
  {
    TraceMessage(TraceLog, "error: file %s not an object", filename.c_str());
    exit(-1);
  }

  // parsed document
  return document;
}

Data::~Data()
{
  Shutdown();
}

///-------------------------------------
/// Brief: reads in archetype and level data
void Data::Initialize()
{
  ObjFactory *fac = dynamic_cast<ObjFactory*>(EngineGetSystem("ObjFactory"));

  // stores object archetype filenames
  std::vector<std::string>filenames = GetFilesFromDirectory("Data/Objects");

  // creates and stores all object archetypes
  for (std::string it : filenames)
  {
    std::string path = "Data/Objects/" + it;
    Obj *obj = FromJSON(path);

    CleanFileName(it);

    obj->type_ = it;
    Space::AddArchetype(obj);
  }

  // stores tiled map filenames
  filenames.clear();
  filenames = GetFilesFromDirectory("Data/Screens");

  // loads and stores all tiled maps
  for (std::string it : filenames)
  {
    std::string filename = "Data/Screens/" + it;
    CleanFileName(it);
    LoadScreen(filename, it);
  }
}

///-------------------------------------
/// Brief: currently only saves changes made to archetypes
void Data::Shutdown()
{
  TraceMessage(TraceLog, "Data Shutdown");
}

///-------------------------------------
/// Brief: Gets object data from json file (archetypes)
///        returns pointer to object
Obj* Data::FromJSON(std::string filename)
{
  Document doc = FillDocument(filename);
  Value::ConstMemberIterator itEnd = doc.MemberEnd();

  CompCreator creator;
  Obj *obj;
  
  if (filename.find("Player") != -1)
    obj = &(Space::GetPlayer()); // player object created in space
  else if (filename.find("Trigger") != std::string::npos) //Added by Sonya
    obj = new Trigger();
  else
    obj = new Obj(); // object being created

  std::string name = filename;
  CleanFileName(name);
  obj->name_ = name;

  if (name.find("SmallFlame") != -1 || name.find("Key") != -1)
    obj->PartsLast(true);

  // grabs each object member
  for (Value::ConstMemberIterator it = doc.MemberBegin(); it != itEnd; it++)
  {
    // right now components can only be strings or objects
    if (it->name.GetType() == kStringType)
    {
      Component::compType type = creator.GetComponentType(it->name.GetString());

      TraceMessage(TraceLog, "component %s found in %s", it->name.GetString(), filename.c_str());

      std::string filename;
      std::string colliderType;
      std::string mask;

      // possible components
      Animation *animation = nullptr;
      Audio *audio = nullptr;
      Behavior *behavior = nullptr;
      Health *health = nullptr;
      Debug *debug = nullptr;
      ParticleEmitter *partemit = nullptr;
      Physics *physics = nullptr;
      Sprite *sprite = nullptr;
      Transform *transform = nullptr;
      ColliderCircle *colliderCircle = nullptr;
      ColliderSquare *colliderSquare = nullptr;
      Score *score = nullptr;
      Stats *stats = nullptr;
      Timer *timer = nullptr;
      Key *key = nullptr;
	    Spawn* spn = nullptr;
	    FlashSprite* flash = nullptr;
	    Aggro* aggro = nullptr;
	    SetVisible* vis = nullptr;
	    SetDoor* door = nullptr;
	    SetInvincible* invince = nullptr;
	    SetActive* active = nullptr;
	    Inspection* inspect = nullptr;
	    ChangeMusic* music = nullptr;
      ToggleDark *dark = nullptr;

	    trigType triggerType;
	    std::string text;

      Value::ConstMemberIterator it2End;

      switch (type)
      {
      case Component::compType::Animation_:
        int begin;
        int end;
        float duration;
        bool animate;

        filename = obj->name_ + "Animation";
        begin = (it->value.FindMember("begin"))->value.GetUint();
        end = (it->value.FindMember("end"))->value.GetUint();
        duration = (it->value.FindMember("duration"))->value.GetFloat();
        animate = (it->value.FindMember("animate"))->value.GetBool();
        animation = creator.ConstructAnimation(filename.c_str(), begin, end, duration, animate);
        obj->Add(animation);
        continue;

      case Component::compType::Audio_:
        audio = creator.ConstructAudio();

        it2End = it->value.MemberEnd();
        for (Value::ConstMemberIterator it2 = it->value.MemberBegin(); it2 != it2End; it2++)
        {
          audio->AddSound(static_cast<Audio::type>(it2->value.GetInt()), it2->name.GetString());
        }

        obj->Add(audio);
        continue;

      case Component::compType::Behavior_:
        filename = it->value.GetString();
        behavior = creator.ConstructBehavior(filename);
        obj->Add(behavior);
        continue;

      case Component::compType::Color_:
        filename = it->value.GetString();

        if (!filename.compare("yellow"))
          obj->color_ = Yellow_;
        else if (!filename.compare("green"))
          obj->color_ = Green_;
        else if (!filename.compare("blue"))
          obj->color_ = Blue_;
        else if (!filename.compare("purple"))
          obj->color_ = Purple_;
        else if (!filename.compare("red"))
          obj->color_ = Red_;
        else
          obj->color_ = NoColor_;
        continue;

      case Component::compType::Health_:
        int hp;
        hp = it->value.GetInt();
        health = creator.ConstructHealth(hp);
        obj->Add(health);
        continue;

      case Component::compType::Key_:
        filename = it->value.GetString();

        if (filename.empty())
          filename = "TEST";

        key = creator.ConstructKey(filename);
        obj->Add(key);
        continue;

      case Component::compType::ParticleEmitter_:
        int emax;
        secs etime;
        int eradius;
        float esize;
        int etype;
        int ebehave;
        glm::vec4 color;

        emax = (it->value.FindMember("max"))->value.GetInt();
        etime = secs((it->value.FindMember("time"))->value.GetInt());
        eradius = (it->value.FindMember("radius"))->value.GetInt();
        esize = (it->value.FindMember("size"))->value.GetFloat();
        color.r = (it->value.FindMember("red"))->value.GetFloat();
        color.g = (it->value.FindMember("green"))->value.GetFloat();
        color.b = (it->value.FindMember("blue"))->value.GetFloat();
        color.a = (it->value.FindMember("alpha"))->value.GetFloat();
        etype = (it->value.FindMember("type"))->value.GetInt();
        ebehave = (it->value.FindMember("behavior"))->value.GetInt();

        partemit = creator.ConstructParticleEmitter(emax, etime, eradius, esize, color, static_cast<EmitterType>(etype), static_cast<EmitterBehavior>(ebehave));
        obj->Add(partemit);

        if (obj->IsTrigger())
          obj->PartsLast(true);
        continue;

      case Component::compType::Physics_:
        physics = creator.ConstructPhysics();
        obj->Add(physics);
        continue;

      case Component::compType::ResType_:
        filename = it->value.GetString();
        obj->ResolutionType(filename);
        continue;

      case Component::compType::Score_:
        score = creator.ConstructScore();
        obj->Add(score);
        continue;

      case Component::compType::Sprite_:
        glm::uvec2 garbo;
        glm::vec4 rgba;
        bool visible;

        garbo.x = (it->value.FindMember("rows"))->value.GetUint();
        garbo.y = (it->value.FindMember("columns"))->value.GetUint();
        filename = (it->value.FindMember("image"))->value.GetString();
        rgba.x = (it->value.FindMember("red"))->value.GetFloat();
        rgba.y = (it->value.FindMember("green"))->value.GetFloat();
        rgba.z = (it->value.FindMember("blue"))->value.GetFloat();
        rgba.w = (it->value.FindMember("alpha"))->value.GetFloat();
        visible = (it->value.FindMember("visible"))->value.GetBool();

        if (it->value.HasMember("mask"))
          mask = (it->value.FindMember("mask"))->value.GetString();

        sprite = creator.ConstructSprite(filename.c_str(), mask.c_str(), garbo);
        sprite->SetRGBA(rgba);
        sprite->SetVisible(visible);
        obj->Add(sprite);
        continue;

      case Component::compType::Stats_:
        int attack;
        filename = (it->value.FindMember("type"))->value.GetString();
        attack = (it->value.FindMember("attackPower"))->value.GetInt();

        stats = creator.ConstrustStats(filename, attack);
        obj->Add(stats);
        continue;

      case Component::compType::Timer_:
        float max;
        float reset;
        tType type;

        filename = (it->value.FindMember("type"))->value.GetString();

        if (!filename.compare("countdown"))
          type = CountDown_;
        else if (!filename.compare("countup"))
          type = CountUp_;
        else
          type = CountDown_;

        max = (it->value.FindMember("max"))->value.GetFloat();
        reset = (it->value.FindMember("resetvalue"))->value.GetFloat();

        timer = creator.ConstrustTimer(max, type, reset);
        obj->Add(timer);

        continue;

      case Component::compType::Transform_:
        float x;
        float y;
        float scale;

        x = (it->value.FindMember("x"))->value.GetFloat();
        y = (it->value.FindMember("y"))->value.GetFloat();

        scale = (it->value.FindMember("scale"))->value.GetFloat();

        transform = creator.ConstructTransform(x, y);
        transform->MultScale(scale);
        obj->Add(transform);
        continue;
     
      case Component::compType::Spawn_:
        float posx;
        float posy;
        const char* objType;

        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        objType = (it->value.FindMember("Obj Type"))->value.GetString();
        posx = ((it->value.FindMember("Pos X"))->value.GetFloat());
        posy = ((it->value.FindMember("Pos Y"))->value.GetFloat());

        spn = creator.ConstructSpawn(triggerType, SpawnData(Vec2D(posx, posy), objType));
        obj->Add(spn);

        continue;

      case Component::compType::FlashSprite_:
        const char* objName;
        secs dur;

        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        objName = (it->value.FindMember("Object Name"))->value.GetString();
        dur = (it->value.FindMember("Duration"))->value.GetFloat();

        flash = creator.ConstructFlashSprite(triggerType, FlashData(objName, dur));
        obj->Add(flash);

        continue;

      case Component::compType::Aggro_:
        const char* enemyName;
        bool toggle;

        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        enemyName = (it->value.FindMember("Enemy Name"))->value.GetString();
        toggle = (it->value.FindMember("Toggle"))->value.GetBool();

        aggro = creator.ConstructAggro(triggerType, BoolEventData(enemyName, toggle));
        obj->Add(aggro);
        continue;


      case Component::compType::SetDoor_:
        int newState;
        const char* id;
        const char* screenName;
        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        id = (it->value.FindMember("ID"))->value.GetString();
        screenName = (it->value.FindMember("Screen Name"))->value.GetString();
        newState = (it->value.FindMember("State"))->value.GetInt();

        door = creator.ConstructSetDoor(triggerType, DoorData(newState, id, screenName));
        obj->Add(door);
        continue;

      case Component::compType::SetVisible_:
        const char* objectName;
        bool visTog;
        //eventNum num;

        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        objectName = (it->value.FindMember("Object Name"))->value.GetString();
        visTog = (it->value.FindMember("Visible"))->value.GetBool();

        vis = creator.ConstructSetVisible(triggerType, BoolEventData(objectName, visTog));
        if (it->value.FindMember("Customize") != it->value.MemberEnd())
        {
          vis->IsCustomized(true);
        }

        obj->Add(vis);
        continue;

      case Component::compType::SetInvincible_:
        bool invincState;

        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        objectName = (it->value.FindMember("Object Name"))->value.GetString();
        invincState = (it->value.FindMember("Invincible"))->value.GetBool();

        invince = creator.ConstructSetInvincible(triggerType, BoolEventData(objectName, invincState));
        obj->Add(invince);
        continue;

      case Component::compType::SetActive_:
        bool activeState;

        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        objectName = (it->value.FindMember("Object Name"))->value.GetString();
        activeState = (it->value.FindMember("Active"))->value.GetBool();

        active = creator.ConstructSetActive(triggerType, BoolEventData(objectName, activeState));
        obj->Add(active);
        continue;

      case Component::compType::Inspection_:
      {
        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        text = (it->value.FindMember("Text"))->value.GetString();
        int length = (it->value.FindMember("Length"))->value.GetInt();
        int height = (it->value.FindMember("Height"))->value.GetInt();
        Obj* null = nullptr;

        inspect = creator.ConstructInspection(triggerType, PopupParam(text, null, length, height));
        obj->Add(inspect);
        continue;
      }

      case Component::compType::ChangeMusic_:
      {
        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        text = (it->value.FindMember("Filename"))->value.GetString();
        SoundData data(text);
        music = creator.ConstructChangeMusic(triggerType, data);
        obj->Add(music);
        continue;
      }

      case Component::compType::ToggleDark_:
      {
        triggerType = trigType((it->value.FindMember("Trigger Type"))->value.GetInt());
        dark = creator.ConstructToggleDark(triggerType);
        obj->Add(dark);
        continue;
      }

      case Component::compType::Collider_:
        colliderType = (it->value.FindMember("type"))->value.GetString();

        // creates and adds circle collider
        if (!strcmp(colliderType.c_str(), "circle"))
        {
          float radius;
          bool dynamic;
          bool block;

          radius = (it->value.FindMember("radius"))->value.GetFloat();
          dynamic = (it->value.FindMember("dynamic"))->value.GetBool();
          block = (it->value.FindMember("block"))->value.GetBool();
          colliderCircle = creator.ConstructCircleCollider(radius, dynamic, block);
          obj->Add(colliderCircle);
        }

        // creates and adds square collider
        else if (!strcmp(colliderType.c_str(), "square"))
        {
          float width;
          float height;
          bool dynamic;
          bool block;

          width = (it->value.FindMember("width"))->value.GetFloat();
          height = (it->value.FindMember("height"))->value.GetFloat();
          dynamic = (it->value.FindMember("dynamic"))->value.GetBool();
          block = (it->value.FindMember("block"))->value.GetBool();

          width = width;
          height = height;

          colliderSquare = creator.ConstructSquareCollider(width, height, dynamic, block);
          obj->Add(colliderSquare);
        }

        else
        {
          TraceMessage(TraceLog, "invalid collider type");
          exit(-1);
        }

        continue;

      case Component::compType::Debug_:
        debug = creator.ConstructDebug();
        obj->Add(debug);
        continue;

      case Component::compType::Invalid_:
        TraceMessage(TraceLog, "component type not valid: check json file");
        continue;

      default:
        break;
      }
    }

    else
    {
      TraceMessage(TraceLog, "object component is wrong type");
      exit(-1);
    }
  }

  return obj;
}


///-------------------------------------
/// Brief: sends single object data to json file (for saving archetypes)
void Data::ToJSON(Obj& object)
{
  std::ofstream out("Data/Objects/" + object.name_ + ".json");

  int size = object.components_.size();
  int count = 0;

  // possible components
  Animation *animation = object.Has(Animation);
  Audio *audio = object.Has(Audio);
  BehaviorScript *behavior = dynamic_cast<BehaviorScript*>(object.Has(Behavior));
  ColliderCircle *circle = dynamic_cast<ColliderCircle*>(object.Has(Collider));
  ColliderSquare *square = dynamic_cast<ColliderSquare*>(object.Has(Collider));
  Health *health = object.Has(Health);
  Key *key = object.Has(Key);
  ParticleEmitter *partemit = object.Has(ParticleEmitter);
  Physics *physics = object.Has(Physics);
  Score *score = object.Has(Score);
  Sprite *sprite = object.Has(Sprite);
  Stats *stats = object.Has(Stats);
  Timer *timer = object.Has(Timer);
  Transform *transform = object.Has(Transform);

  // beginning of json file
  out << "{" << std::endl;

  std::string resType = object.ResolutionStringType();

  if (!resType.empty())
  {
    out << "\"ResType\":\"" << resType << "\"," << std::endl;
  }

  if (object.color_ != NoColor_)
  {
    out << "\"Color\":";
    if (object.color_ == Yellow_)
      out << "\"yellow\",";
    else if (object.color_ = Green_)
      out << "\"green\",";
    else if (object.color_ == Blue_)
      out << "\"blue\",";
    else if (object.color_ == Purple_)
      out << "\"purple\",";
    else if (object.color_ == Red_)
      out << "\"red\",";

    out << std::endl;
  }
  if (animation)
  {
    out << "\"Animation\":{";
    out << "\"begin\":" <<  animation->GetBegFrame() << "," << std::endl;
    out << std::string(13, ' ') << "\"end\":" << animation->GetFramEnd() << "," << std::endl;
    out << std::string(13, ' ') << "\"duration\":" << animation->GetFrameDur() << "," << std::endl;

    if (animation->GetAnimating())
      out << std::string(13, ' ') << "\"animate\":true" << std::endl;
    else
      out << std::string(13, ' ') << "\"animate\":false" << std::endl;

    out << std::string(12, ' ') << "}";

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (audio)
  {
    out << "\"Audio\":{" << std::endl;

    for (int i = Audio::Default_; i < Audio::None_; i++)
    {
      std::string file = audio->GetAudioFile(static_cast<Audio::type>(i));

      if (!file.empty())
        out << std::string(9, ' ') << "\"" << file << "\"";

      if (i != (Audio::None_ - 1))
        out << "," << std::endl;
    }

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (behavior)
  {
    out << "\"Behavior\":";
    out << "\"" << behavior->Name() << "\"";
    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (circle)
  {
    out << "\"Collider\":{";
    out << "\"type\":" << "\"circle\"," << std::endl;
    out << std::string(12, ' ') << "\"radius\":" << circle->Radius() << "," << std::endl;
    out << std::string(12, ' ') << "\"dynamic\":";
    if (circle->Dynamic())
      out << "true," << std::endl;
    else
      out << "false," << std::endl;

    out << std::string(12, ' ') << "\"block\":";
    if (circle->isBlock())
      out << "true" << std::endl;
    else
      out << "false" << std::endl;

    out << std::string(11, ' ') << "}";
   
    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (square)
  {
    out << "\"Collider\":{";
    out << "\"type\":" << "\"square\"," << std::endl;
    out << std::string(12, ' ') << "\"height\":" << square->Dimensions().Y() * 2.0 << ","<< std::endl;
    out << std::string(12, ' ') << "\"width\":" << square->Dimensions().X() * 2.0 << "," << std::endl;
    out << std::string(12, ' ') << "\"dynamic\":";

    if (square->Dynamic())
      out << "true" << std::endl;
    else
      out << "false" << std::endl;
    out << std::string(11, ' ') << "}";

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (health)
  {
    out << "\"Health\":" << health->MaxHP();
    
    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (key)
  {
    out << "\"Key\":" << "\"\"" << std::endl;

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (partemit)
  {
    glm::vec4 color = partemit->Color();
    Vec2D offset = partemit->Offset();
    out << "\"ParticleEmitter\":{";
    out << std::string(19, ' ') << "\"Emitter R\":" << color.r << "," << std::endl;
    out << std::string(19, ' ') << "\"Emitter G\":" << color.g << "," << std::endl;
    out << std::string(19, ' ') << "\"Emitter B\":" << color.b << "," << std::endl;
    out << std::string(19, ' ') << "\"Emitter A\":" << color.a << "," << std::endl;
                       
    out << std::string(19, ' ') << "\"Emitter X\":" << offset.X() << "," << std::endl;
    out << std::string(19, ' ') << "\"Emitter Y\":" << offset.Y() << "," << std::endl;
                       
    out << std::string(19, ' ') << "\"Emitter Radius\":" << partemit->Radius() << "," << std::endl;
                       
    out << std::string(19, ' ') << "\"Emitter Size\":" << partemit->Size() << "," << std::endl;
                       
    out << std::string(19, ' ') << "\"Emitter Time\":" << partemit->Time() << "," << std::endl;
                       
    out << std::string(19, ' ') << "\"Emitter Max\":" << partemit->MaxParticles() << "," << std::endl;
                       
    out << std::string(19, ' ') << "\"Emitter Type\":" << int(partemit->Type()) << "," << std::endl;
    out << std::string(19, ' ') << "\"Emitter Behavior\":" << int(partemit->Behavior()) << "," << std::endl;

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (physics)
  {
    out << "\"Physics\":\"\"";

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (score)
  {
    out << "\"Score\":\"\"";

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (sprite)
  {
    out << "\"Sprite\":{";
    out << "\"image\":\"" << sprite->GetName() << "\","<< std::endl;
    out << std::string(10, ' ') << "\"rows\":" << sprite->GetImgWidth() << "," << std::endl;
    out << std::string(10, ' ') << "\"columns\":" << sprite->GetImgHeight() << "," << std::endl;

    if (sprite->GetVisible())
      out << std::string(10, ' ') << "\"visible\":true," << std::endl;
    else
      out << std::string(10, ' ') << "\"visible\":false," << std::endl;

    out << std::string(10, ' ') << "\"red\":" << sprite->GetRGBA().r << "," << std::endl;
    out << std::string(10, ' ') << "\"green\":" << sprite->GetRGBA().g << "," << std::endl;
    out << std::string(10, ' ') << "\"blue\":" << sprite->GetRGBA().b << "," << std::endl;
    out << std::string(10, ' ') << "\"alpha\":" << sprite->GetAlpha() << std::endl;
    out << std::string(9, ' ') << "}";

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (stats)
  {
    out << "\"Stats\":{";
    out << "\"type\":\"" << stats->ClassToString(stats->Class()) << "\"," << std::endl;
    out << std::string(10, ' ') << "\"attackPower\":" << stats->Attack() << std::endl;
    out << std::string(9, ' ') << "}";
    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (timer)
  {
    out << "\"Timer\":{";
    out << "\"max\":" << timer->MaxTime() << "," << std::endl;

    tType ctype = timer->Type();
   
    if (ctype == CountUp_)
      out << std::string(9, ' ') << "\"type\":\"countup\"," << std::endl;
    else if (ctype == CountDown_)
      out << std::string(9, ' ') << "\"type\":\"countdown\"," << std::endl;
    else
      out << std::string(9, ' ') << "\"type\":\"default\"," << std::endl;

    out << std::string(9, ' ') << "\"resetvalue\":" << timer->ResetVal() << std::endl;
    out << std::string(8, ' ') << "}";

    count++;
    if (count != size)
      out << "," << std::endl;
  }
  if (transform)
  {
    out << "\"Transform\":{";
    out << "\"x\":" << 0.0 << "," << std::endl;
    out << std::string(13, ' ') << "\"y\":" << 0.0 << "," << std::endl;
    out << std::string(13, ' ') << "\"scale\":" << transform->MultScale() << std::endl;
    out << std::string(12, ' ') << "}";

    count++;
    if (count != size)
      out << "," << std::endl;
  }


  // ends json file
  out << std::endl << "}" << std::endl;
}


///-------------------------------------
/// Brief: loads in screen json data (maps)
void Data::LoadScreen(std::string filename, std::string mapName)
{
  Document document = FillDocument(filename);

  // grabs what type of screen
  std::string type = (*static_cast<Value*>(getMember(&document, "type"))).GetString();
  std::string background = (*static_cast<Value*>(getMember(&document, "background"))).GetString();

  // screen name
  std::string name = filename;
  CleanFileName(name);

  // screen
  Screen *screeny = nullptr;

  if (!type.compare("area"))
  {
    std::string north = (*static_cast<Value*>(getMember(&document, "north"))).GetString();
    std::string south = (*static_cast<Value*>(getMember(&document, "south"))).GetString();
    std::string east = (*static_cast<Value*>(getMember(&document, "east"))).GetString();
    std::string west = (*static_cast<Value*>(getMember(&document, "west"))).GetString();
    std::string enemyreset = (*static_cast<Value*>(getMember(&document, "enemyreset"))).GetString();
    std::string puzzlereset = (*static_cast<Value*>(getMember(&document, "puzzlereset"))).GetString();

    screeny = new Area(name);
    screeny->SetType(Screen::ScreenType::Area_);

    Area * temp = static_cast<Area*>(screeny);

    temp->AttachNorthArea(north);
    temp->AttachSouthArea(south);
    temp->AttachEastArea(east);
    temp->AttachWestArea(west);

    // enemy reset value
    if (enemyreset.empty())
      temp->EnemyReset(Area::reset::None_);
    else if (!enemyreset.compare("all"))
      temp->EnemyReset(Area::reset::All_);
    else if (!enemyreset.compare("pos"))
      temp->EnemyReset(Area::reset::Pos_);

    // puzzle reset value
    if (!puzzlereset.empty())
      temp->PuzzleReset(Area::reset::None_);
    else if (!puzzlereset.compare("all"))
      temp->PuzzleReset(Area::reset::All_);
    else if (!puzzlereset.compare("pos"))
      temp->PuzzleReset(Area::reset::Pos_);
  }
  else if (!type.compare("overlay"))
  {
    // overlay goes here
    //screeny->SetType(Screen::ScreenType::Overlay_);
  }
  else if (type.empty())
  {
    screeny = new Screen(name);
    screeny->SetType(Screen::ScreenType::Default_);
  }

  if (!screeny)
  {
    TraceMessage(TraceLog, "Serialization:loadScreens screen failed to be created");
    exit(-1);
  }

  if (getMember(&document, "start") != nullptr)
  {
    screeny->IsStartScreen(true);
    static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->StartScreen(screeny);
  }

  // adds background object (should always be first in object lists)
  Obj *obj = new Obj(name + "Background");
  Sprite *spriz = new Sprite("Data/Textures/" + background, "");
  Transform *trans = new Transform(0.0f, 0.0f);

  trans->Scale(1920.0f, 1080.0f);
  obj->Add(spriz);
  obj->Add(trans);
  //ObjManagerAddObject(obj);
  screeny->AddObject(obj);
  screeny->SetBackground(background);

  // load objects
  if (document.HasMember("objects"))
  {
    Value &objects = document.FindMember("objects")->value;
    loadScreenObjects(objects, screeny);
  }

  // adds screen to screen manager
  static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->AddScreen(name, screeny);

  // for debugging save screen
  //SaveScreen(screeny);
}

///-------------------------------------
/// Brief: saves gamescreen as json file
void Data::SaveScreen(Screen * screen)
{
  std::string ogName = screen->GetOGName();

  // removes old file if name changes
  if (ogName.compare(screen->GetName()) != 0)
  {
    if (std::remove(("Data/Screens/" + ogName + ".json").c_str()) != 0)
    {
      TraceMessage(TraceLog, "Error deleting file %s", ogName.c_str());
      exit(-1);
    }
  }

  std::ofstream out("Data/Screens/" + screen->GetName() + ".json");
  Screen::ScreenType type = screen->GetType();

  bool hasPlayer = false;

  out << "{" << std::endl; // start of json file

  if (screen->IsStartScreen() == true)
    out << "\"start\":\"start\"," << std::endl;

  out << "\"type\":";

  if (type = Screen::ScreenType::Area_)
  {
    Area *temp = static_cast<Area*>(screen);
    out << "\"area\"," << std::endl;
    out << "\"north\":\"" << temp->GetNorthArea() << "\"," << std::endl;
    out << "\"south\":\"" << temp->GetSouthArea() << "\"," << std::endl;
    out << "\"east\":\"" << temp->GetEastArea() << "\"," << std::endl;
    out << "\"west\":\"" << temp->GetWestArea() << "\"," << std::endl;
    out << "\"enemyreset\":";
    Area::reset enemyreset = temp->EnemyReset();
    Area::reset puzzlereset = temp->PuzzleReset();

    switch (enemyreset)
    {
    case Area::reset::None_: 
      out << "\"\"," << std::endl;
      break;
    case Area::reset::All_:
      out << "\"all\"," << std::endl;
      break;
    case Area::reset::Pos_:
      out << "\"pos\"," << std::endl;
      break;
    }

    out << "\"puzzlereset\":";

    switch (puzzlereset)
    {
    case Area::reset::None_:
      out << "\"\"," << std::endl;
      break;
    case Area::reset::All_:
      out << "\"all\"," << std::endl;
      break;
    case Area::reset::Pos_:
      out << "\"pos\"," << std::endl;
      break;
    }

    // checks if area obj list contains player
    hasPlayer = temp->hasPlayer();
  }
  //else if (type = Screen::ScreenType::Overlay_)
  //{
  //}
  else
  {
    out << "\"\"," << std::endl;
  }

  out << "\"background\":\"" << screen->GetBackground() << "\"," << std::endl;
  out << "\"objects\":[" << std::endl;

  std::vector<Obj*> objs = screen->GetObjectList();

  // fills json object array
  for (Obj *obj : objs)
  {
    if (obj->name_.find("Background") != -1)
    {
      continue;
    }

    Transform* transform = static_cast<Transform*>(obj->Has(Transform));
    Sprite* sprite = static_cast<Sprite*>(obj->Has(Sprite));
    Health* health = static_cast<Health*>(obj->Has(Health));
    FastChase *fast = dynamic_cast<FastChase*>(obj->Has(Behavior));
    SlowChase *slow = dynamic_cast<SlowChase*>(obj->Has(Behavior));
    Necromancer *nec = dynamic_cast<Necromancer*>(obj->Has(Behavior));
    ParticleEmitter* emitter = obj->Has(ParticleEmitter);
    Door *door = dynamic_cast<Door*>(obj->Has(Behavior));
    Key *key = static_cast<Key*>(obj->Has(Key));
    ColliderCircle* circle = dynamic_cast<ColliderCircle*>(obj->Has(Collider));
    ColliderSquare* square = dynamic_cast<ColliderSquare*>(obj->Has(Collider));

    Aggro* aggro = obj->Has(Aggro);
    ChangeMusic* music = obj->Has(ChangeMusic);
    FlashSprite* flash = obj->Has(FlashSprite);
    Inspection* inspect = obj->Has(Inspection);
    SetActive* active = obj->Has(SetActive);
    SetDoor* setDoor = obj->Has(SetDoor);
    SetInvincible* setInvince = obj->Has(SetInvincible);
    SetVisible* setVis = obj->Has(SetVisible);
    Spawn* spawn = obj->Has(Spawn);
    
    glm::vec4 blarg = sprite->GetRGBA();

    out << std::string(2, ' ') << "{" << std::endl;
    out << std::string(3, ' ') << "\"name\":\"" << obj->type_ << "\"," << std::endl;
    out << std::string(3, ' ') << "\"type\":\"" << obj->type_ << "\"," << std::endl;
    out << std::string(3, ' ') << "\"color\":" << obj->color_ << "," << std::endl;
    out << std::string(3, ' ') << "\"x\":" << transform->OGTrans().X() << "," << std::endl;
    out << std::string(3, ' ') << "\"y\":" << transform->OGTrans().Y() << "," << std::endl;
    out << std::string(3, ' ') << "\"r\":" << blarg.r << "," << std::endl;
    out << std::string(3, ' ') << "\"g\":" << blarg.g << "," << std::endl;
    out << std::string(3, ' ') << "\"b\":" << blarg.b << "," << std::endl;
    out << std::string(3, ' ') << "\"a\":" << blarg.a << "," << std::endl;
    if (sprite->GetVisible())
      out << std::string(3, ' ') << "\"visible\":true" << "," << std::endl;
    else
      out << std::string(3, ' ') << "\"visible\":false" << "," << std::endl;

    // checks for different components and parameters
    if (health)
    {
      out << std::string(3, ' ') << "\"hp\":" << health->MaxHP() << "," << std::endl;
      if (health->Invincible())
        out << std::string(3, ' ') << "\"invincible\":" << "true" << "," << std::endl;
      else
        out << std::string(3, ' ') << "\"invincible\":" << "false" << "," << std::endl;
    }

    if (fast)
      out << std::string(3, ' ') << "\"speed\":" << fast->chaseSpeed << "," << std::endl;
    else if (slow)
      out << std::string(3, ' ') << "\"speed\":" << slow->speed << "," << std::endl;
    else if(nec)
      out << std::string(3, ' ') << "\"speed\":" << nec->chaseSpeed << "," << std::endl;

    if (emitter)
    {
      glm::vec4 color = emitter->Color();
      Vec2D offset = emitter->Offset();
      out << std::string(3, ' ') << "\"Emitter R\":" << color.r << "," << std::endl;
      out << std::string(3, ' ') << "\"Emitter G\":" << color.g << "," << std::endl;
      out << std::string(3, ' ') << "\"Emitter B\":" << color.b << "," << std::endl;
      out << std::string(3, ' ') << "\"Emitter A\":" << color.a << "," << std::endl;

      out << std::string(3, ' ') << "\"Emitter X\":" << offset.X() << "," << std::endl;
      out << std::string(3, ' ') << "\"Emitter Y\":" << offset.Y() << "," << std::endl;

      out << std::string(3, ' ') << "\"Emitter Radius\":" << emitter->Radius() << "," << std::endl;

      out << std::string(3, ' ') << "\"Emitter Size\":" << emitter->Size() << "," << std::endl;

      out << std::string(3, ' ') << "\"Emitter Time\":" << emitter->Time() << "," << std::endl;

      out << std::string(3, ' ') << "\"Emitter Max\":" << emitter->MaxParticles() << "," << std::endl;

      out << std::string(3, ' ') << "\"Emitter Type\":" << int(emitter->Type()) << "," << std::endl;
      out << std::string(3, ' ') << "\"Emitter Behavior\":" << int(emitter->Behavior()) << "," << std::endl;
    }

	  if (square)
	  {
		  Vec2D dim = square->Dimensions();
		  out << std::string(3, ' ') << "\"Collider Width\":" << dim.X() << "," << std::endl;
		  out << std::string(3, ' ') << "\"Collider Height\":" << dim.Y() << "," << std::endl;
	  }

    if (door)
    {
      out << std::string(3, ' ') << "\"status\":" << door->StartState() << "," << std::endl;
      out << std::string(3, ' ') << "\"wall\":" << door->wall << "," << std::endl;

      out << std::string(3, ' ') << "\"area\":\"";
      if (!door->area.empty())
        out << door->area;
      
      out << "\"," << std::endl;

      out << std::string(3, ' ') << "\"id\":\"";
      if (!door->ID.empty())
        out << door->ID;
      
      out << "\"," << std::endl;
    }

    if (key)
    {
      // key is active
      out << std::string(3, ' ') << "\"isActive\":";
      if (key->IsActive())
        out << "true,\n";
      else
        out << "false,\n";

      // key ID
      out << std::string(3, ' ') << "\"id\":\"";
      if (!key->ID().empty())
        out << key->ID();

      out << "\"," << std::endl;
    }

    // need to be able to get trig type from object
    if (obj->isTrigger_)
    {
      Trigger *trig = dynamic_cast<Trigger*>(obj);
      trigType trigT = trig->GetTriggerType();
      out << std::string(3, ' ') << "\"trigger type\":" << trigT << "," << std::endl;

      if (trigT == OnCollision_)
      {
        std::string colType = GetStringFromResType(trig->GetCollisionType());
        out << std::string(3, ' ') << "\"collision type\":" << "\"" << colType << "\"" << "," << std::endl;
      }
      else if (trigT == OnRoomsComplete_)
      {
        std::vector<std::string>areas = trig->GetRoomsNeeded();

        out << std::string(3, ' ') << "\"areas\":{" << std::endl;
        for (std::string a : areas)
        {
          out << std::string(13, ' ') << "\"" << a << "\":0";
          if (a != areas.back())
            out << ",";
          out << std::endl;
        }

        out << std::string(11, ' ') << "}," << std::endl;
      }
      else if (trigT == OnTimer_)
      {
        out << std::string(3, ' ') << "\"timer max\":" << trig->Has(Timer)->MaxTime() << "," << std::endl;
      }

      if (obj->IsActive())
        out << std::string(3, ' ') << "\"isActive\":" << "true" << "," << std::endl;
      else
        out << std::string(3, ' ') << "\"isActive\":" << "false" << "," << std::endl;
    }

    if (aggro)
    {
      std::string idk = aggro->EnemyName();
      out << std::string(3, ' ') << "\"enemy name\":" << "\"" << aggro->EnemyName() << "\"" << "," << std::endl;
      if (aggro->Bool())
        out << std::string(3, ' ') << "\"aggro bool\":" << "true" << "," << std::endl;
      else
        out << std::string(3, ' ') << "\"aggro bool\":" << "false" << "," << std::endl;
    }

    if (music)
    {
      out << std::string(3, ' ') << "\"filename\":" << "\"" << music->Filename() << "\"" << "," << std::endl;
      if (music->Play())
        out << std::string(3, ' ') << "\"play\":" << "true" << "," << std::endl;
      else
        out << std::string(3, ' ') << "\"play\":" << "false" << "," << std::endl;
    }

    if (flash)
    {
      out << std::string(3, ' ') << "\"flash name\":" << "\"" << flash->ObjName() << "\"" << "," << std::endl;
      out << std::string(3, ' ') << "\"duration\":" << flash->Duration() << "," << std::endl;
    }

    if (inspect)
    {
      out << std::string(3, ' ') << "\"text\":" << "\"" << inspect->Text() << "\"" << "," << std::endl;
      out << std::string(3, ' ') << "\"inspection obj\":" << "\"" << inspect->ObjName() << "\"" << "," << std::endl;
      if (inspect->Length() != 100)
        out << std::string(3, ' ') << "\"length\":" << inspect->Length() << "," << std::endl;
      if (inspect->Height() != 25)
        out << std::string(3, ' ') << "\"height\":" << inspect->Height() << "," << std::endl;
      if (inspect->Time() != 2.0f)
        out << std::string(3, ' ') << "\"time\":" << inspect->Time() << "," << std::endl;
    }

    if (active)
    {
      out << std::string(3, ' ') << "\"active name\":" << "\"" << active->ObjName() << "\"" << "," << std::endl;
      if (active->Bool())
        out << std::string(3, ' ') << "\"active bool\":" << "true" << "," << std::endl;
      else
        out << std::string(3, ' ') << "\"active bool\":" << "false" << "," << std::endl;
    }

    if (setDoor)
    {
      out << std::string(3, ' ') << "\"door state\":" << "\"" << GetStringFromDoorState(setDoor->NewState()) << "\"" << "," << std::endl;
      out << std::string(3, ' ') << "\"door ID\":" << "\"" << setDoor->DoorID() << "\"" << "," << std::endl;
      out << std::string(3, ' ') << "\"screen name\":" << "\"" << setDoor->ScreenName() << "\"" << "," << std::endl;
    }

    if (setInvince)
    {
      out << std::string(3, ' ') << "\"invincible name\":" << "\"" << setInvince->ObjName() << "\"" << "," << std::endl;
      if (setInvince->Bool())
        out << std::string(3, ' ') << "\"invincible bool\":" << "true" << "," << std::endl;
      else
        out << std::string(3, ' ') << "\"invincible bool\":" << "false" << "," << std::endl;
    }

    if (setVis)
    {
      out << std::string(3, ' ') << "\"visible name\":" << "\"" << setVis->ObjName() << "\"" << "," << std::endl;
      if (setVis->Bool())
        out << std::string(3, ' ') << "\"visible bool\":" << "true" << "," << std::endl;
      else
        out << std::string(3, ' ') << "\"visible bool\":" << "false" << "," << std::endl;
    }

    if (spawn)
    {
      out << std::string(3, ' ') << "\"spawn x\":" << spawn->X() << "," << std::endl;
      out << std::string(3, ' ') << "\"spawn y\":" << spawn->Y() << "," << std::endl;
      out << std::string(3, ' ') << "\"spawn type\":" << "\"" << spawn->ObjType() << "\"" << "," << std::endl;
      if (spawn->ByName())
        out << std::string(3, ' ') << "\"byName\":" << "true" << "," << std::endl;
    }

    out << std::string(3, ' ') << "\"scale\":" << transform->MultScale() << std::endl;
    out << std::string(2, ' ') << "}";

    if (obj != objs.back())
      out << "," << std::endl;
  }

  out << "]" << std::endl; // closes object array
  out << "}" << std::endl; // ends json file
}

///-------------------------------------
/// Brief: save archetype object
void Data::SaveArchetype(Obj & object)
{
  ToJSON(object);
}

///-------------------------------------
/// Brief: helper function to get specified members of json file
///        returns void pointer to member (must know type)
///        returns null pointer if member not found
void* Data::getMember(void * doc, const char * name)
{
  Document *document = static_cast<Document*>(doc);
  Value::MemberIterator it;

  if (document->HasMember(name))
  {
    it = document->FindMember(name);

    return &(it->value);
  }

  return nullptr;
}
