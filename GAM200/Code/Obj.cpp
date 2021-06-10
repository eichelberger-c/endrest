///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Obj.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Obj.h"
#include "Spaces.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "Audio.h"
#include "Collider.h"
#include "SlowChase.h"
#include "FastChase.h"
#include "MimicPlayer.h"
#include "Input.h"
#include "Engine.h"
#include "Health.h"
#include "Utils.h"
#include "SpawnerManager.h"
#include "Stats.h"
#include "Timer.h"
#include "Score.h"
#include "Camera.h"
#include "Collect.h"
#include "Door.h"
#include "Debug.h"
#include "PeepHole.h"
#include "Aggro.h"
#include "Spawn.h"
#include "Debug.h"
#include "FlashSprite.h"
#include "Inspection.h"
#include "SetVisible.h"
#include "SetActive.h"
#include "SetDoor.h"
#include "ChangeMusic.h"
#include "SetInvincible.h"
#include "Necromancer.h"
#include "ToggleDark.h"

///----------------------------------GLOBALS-----------------------------------
#define defaultMap "Map1"
#define defaultType "Default"

static bool f = false;
static bool t = true;

///---------------------------------FUNCTIONS-----------------------------------

Obj::Obj(const char *name)
{
  name_ = name;
  killSwitch_ = false;
  color_ = NoColor_;
  mapName_ = defaultMap;
  type_ = defaultType;
  isActive_ = true;
  respawn_ = false;
  resT_ = Other_; // sets to general resolution on construction
  isTrigger_ = false;
  partsLast_ = false;
}

Obj::Obj(std::string name)
{
  name_ = name;
  killSwitch_ = false;
  color_ = NoColor_;
  mapName_ = defaultMap;
  type_ = defaultType;
  isActive_ = true;
  respawn_ = false;
  resT_ = Other_;
  isTrigger_ = false;
  partsLast_ = false;
}


// THIS SHOULDN'T BE CALLED ANYWHERE PLZ THANKS
Obj::Obj(const Obj& ogObj)
{
  //name_ = ogObj.name_ + "Copy";
  //killSwitch_ = ogObj.killSwitch_;
  //color_ = ogObj.color_;
  //type_ = ogObj.type_;
  //resT_ = ogObj.resT_;
  //isActive_ = ogObj.isActive_;
  //respawn_ = false;
  //int size = ogObj.components_.size();
  //for (int i = 0; i < size; ++i)
  //{
  //  Add(ogObj.components_[i]);
  //}
}

///-------------------------------------
/// Brief: adds sub object
void Obj::AddSubObj(Obj * object)
{
  subList_.push_back(object);
}

///-------------------------------------
/// Brief: returns subobjects
std::vector<Obj*> Obj::GetSubObjList()
{
  return subList_;
}

///-------------------------------------
/// Brief: returns specific object
Obj * Obj::GetSubObject(std::string filename)
{
  int size = subList_.size();
  for (int i = 0; i < size; ++i)
  {
    if (subList_[i]->Name() == filename)
      return subList_[i];
  }
  return nullptr;
}

///-------------------------------------
/// Brief: private constructor for serialization
Obj::Obj()
{
  name_ = "";
  killSwitch_ = false;
  color_ = NoColor_;
  mapName_ = defaultMap;
  type_ = defaultType;
  isActive_ = true;
  resT_ = Other_;
  respawn_ = false;
  isTrigger_ = false;
}


//COMP

///-------------------------------------
/// Brief: 
Obj::Obj(const Obj& ogObj, std::string name)
{
  name_ = name;
  killSwitch_ = ogObj.killSwitch_;
  color_ = ogObj.color_;
  type_ = ogObj.type_;
  isActive_ = ogObj.isActive_;
  respawn_ = false;
  resT_ = ogObj.resT_;
  isTrigger_ = ogObj.isTrigger_;
  partsLast_ = ogObj.partsLast_;

  int size = ogObj.components_.size();
  for (int i = 0; i < size; ++i)
  {
    Component::compType type = ogObj.components_[i]->Type();
    Component *newComp = nullptr;

    //static int counter = 0;

    Animation *ani;
    Audio *audio;
    SlowChase *slow;
    FastChase *fast;
    MimicPlayer *mimic;
    Necromancer *nec;
    Collect *col;
    Door *door;
    ColliderCircle *circle;
    ColliderSquare *square;
    Health *health;
    Key *key;
    ParticleEmitter *particles;
    Physics *physics;
    Sprite *sprite;
    Transform *transform;
    Stats *stat;
    Score *score;
    Timer *time;
    Debug* debug;

    Aggro *ag;
    ChangeMusic *cm;
    FlashSprite *fs;
    Inspection *inspec;
    SetActive *setA;
    SetDoor *setD;
    SetInvincible *setI;
    SetVisible *setV;
    Spawn *spawn;
    ToggleDark *dark;

    switch (type)
    {
    case Component::compType::Animation_:
      ani = dynamic_cast<Animation*>(ogObj.components_[i]);
      newComp = new Animation(*ani);
      Add(newComp);
      continue;

    case Component::compType::Audio_:
      audio = dynamic_cast<Audio*>(ogObj.components_[i]);
      newComp = new Audio(*audio);
      Add(newComp);
      continue;

      //BEHAV
    case Component::compType::Behavior_:
      slow = dynamic_cast<SlowChase*>(ogObj.components_[i]);
      fast = dynamic_cast<FastChase*>(ogObj.components_[i]);
      mimic = dynamic_cast<MimicPlayer*>(ogObj.components_[i]);
      col = dynamic_cast<Collect*>(ogObj.components_[i]);
      door = dynamic_cast<Door*>(ogObj.components_[i]);
      nec = dynamic_cast<Necromancer*>(ogObj.components_[i]);

      if (slow)
      {
        newComp = new SlowChase(*slow);
        Add(newComp);
        continue;
      }
      else if (fast)
      {
        newComp = new FastChase(*fast);
        Add(newComp);
        continue;
      }
      else if (mimic)
      {
        newComp = new MimicPlayer(*mimic);
        Add(newComp);
        continue;
      }
      else if (col)
      {
        newComp = new Collect(*col);
        Add(newComp);
        continue;
      }
      else if (door)
      {
        newComp = new Door(*door);
        Add(newComp);
        continue;
      }
      else if (nec)
      {
        newComp = new Necromancer(*nec);
        Add(newComp);
        continue;
      }
      continue;

    case Component::compType::Collider_:
      circle = dynamic_cast<ColliderCircle*>(ogObj.components_[i]);
      square = dynamic_cast<ColliderSquare*>(ogObj.components_[i]);

      if (circle)
      {
        newComp = new ColliderCircle(*circle);
        Add(newComp);
        continue;
      }
      else if (square)
      {
        newComp = new ColliderSquare(*square);
        Add(newComp);
        continue;
      }
      continue;

    case Component::compType::Health_:
      health = dynamic_cast<Health*>(ogObj.components_[i]);
      newComp = new Health(*health);
      Add(newComp);
      continue;

    case Component::compType::Key_:
      key = dynamic_cast<Key*>(ogObj.components_[i]);
      newComp = new Key(*key);
      Add(newComp);
      continue;

    case Component::compType::ParticleEmitter_:
      particles = dynamic_cast<ParticleEmitter*>(ogObj.components_[i]);
      newComp = new ParticleEmitter(*particles);
      Add(newComp);
      continue;

    case Component::compType::Physics_:
      physics = dynamic_cast<Physics*>(ogObj.components_[i]);
      newComp = new Physics(*physics);
      Add(newComp);
      continue;

    case Component::compType::Sprite_:
      sprite = dynamic_cast<Sprite*>(ogObj.components_[i]);
      newComp = new Sprite(*sprite);
      Add(newComp);
      continue;

    case Component::Timer_:
      time = dynamic_cast<Timer*>(ogObj.components_[i]);
      newComp = new Timer(*time);
      Add(newComp);
      continue;

    case Component::Stats_:
      stat = dynamic_cast<Stats*>(ogObj.components_[i]);
      newComp = new Stats(*stat);
      Add(newComp);
      continue;

    case Component::Score_:
      score = dynamic_cast<Score*>(ogObj.components_[i]);
      newComp = new Score(*score);
      Add(newComp);
      continue;

    case Component::compType::Transform_:
      transform = dynamic_cast<Transform*>(ogObj.components_[i]);
      newComp = new Transform(*transform);
      Add(newComp);
      continue;

    case Component::compType::Debug_:
      debug = dynamic_cast<Debug*>(ogObj.components_[i]);
      newComp = new Debug(*debug);
      Add(newComp);
      continue;

    case Component::Aggro_:
      ag = dynamic_cast<Aggro*>(ogObj.components_[i]);
      newComp = new Aggro(*ag);
      Add(newComp);
      continue;

    case Component::ChangeMusic_:
      cm = dynamic_cast<ChangeMusic*>(ogObj.components_[i]);
      newComp = new ChangeMusic(*cm);
      Add(newComp);
      continue;

    case Component::FlashSprite_:
      fs = dynamic_cast<FlashSprite*>(ogObj.components_[i]);
      newComp = new FlashSprite(*fs);
      Add(newComp);
      continue;

    case Component::Inspection_:
      inspec = dynamic_cast<Inspection*>(ogObj.components_[i]);
      newComp = new Inspection(*inspec);
      Add(newComp);
      continue;

    case Component::SetActive_:
      setA = dynamic_cast<SetActive*>(ogObj.components_[i]);
      newComp = new SetActive(*setA);
      Add(newComp);
      continue;

    case Component::SetDoor_:
      setD = dynamic_cast<SetDoor*>(ogObj.components_[i]);
      newComp = new SetDoor(*setD);
      Add(newComp);
      continue;

    case Component::SetInvincible_:
      setI = dynamic_cast<SetInvincible*>(ogObj.components_[i]);
      newComp = new SetInvincible(*setI);
      Add(newComp);
      continue;

    case Component::SetVisible_:
      setV = dynamic_cast<SetVisible*>(ogObj.components_[i]);
      newComp = new SetVisible(*setV);
      Add(newComp);
      continue;

    case Component::Spawn_:
      spawn = dynamic_cast<Spawn*>(ogObj.components_[i]);
      newComp = new Spawn(*spawn);
      Add(newComp);
      continue;

    case Component::ToggleDark_:
      dark = dynamic_cast<ToggleDark*>(ogObj.components_[i]);
      newComp = new ToggleDark(*dark);
      Add(newComp);
      continue;
    }
  }
}

///-------------------------------------
/// Brief: Deconstructor
Obj::~Obj()
{
  name_ = "Dead";
  killSwitch_ = true;

  /// mw -- sorry sonya
  while (!subList_.empty())
  {
    Obj * obj = *subList_.begin();
    subList_.erase(subList_.begin());
    delete obj;
    obj = nullptr;
  }

  while (!components_.empty())
  {
    Component* c = *components_.begin();
    components_.erase(components_.begin());
    delete c;
    c = nullptr;
  }
}

///-------------------------------------
/// Brief: Add a component to the object
void Obj::Add(Component *component)
{
  if (component == NULL)
    return;

  component->Parent(this); //set self as component's parent
  Space::AddComponent(component); //add component to appropriate map in Space
  components_.push_back(component); //add component pointer to obj's vector

  std::sort(components_.begin(), components_.end(), ComponentCompare); //sort vector to make access easier
}

bool Obj::HasComponent(Component::compType type)
{
  int size = components_.size();

  for (int i = 0; i < size; ++i)
    if (components_[i]->Type() == type)
      return true;

  return false;
}

void Obj::Swap(Component * newComponent)
{
  if (newComponent == NULL)
    return;

  Component::compType  type = newComponent->Type();
  if (HasComponent(type))
  {
    Component **compPP = GetPP(type);
    *compPP = newComponent;
  }
  else
    Add(newComponent);
}

///-------------------------------------
/// Brief: Get a component from the object
Component * Obj::Get(Component::compType type) const
{
  int size = components_.size();

  for (int i = 0; i < size; ++i)
    if (components_[i]->Type() == type)
      return components_[i];

  return NULL;
}

///-------------------------------------
/// Brief: Updates all the components attached to the object
void Obj::Update(float dt)
{
  int size = components_.size();

  for (int i = 0; i < size; ++i)
    components_[i]->Update(dt);

  ///Added by CJ
  for (Obj* obj : subList_)
  {
    obj->Update(dt);
  }
}

///-------------------------------------
/// Brief: Sets object for destruction
void Obj::Destroy()
{
  killSwitch_ = true;
}

///-------------------------------------
/// Brief: Checks if object is marked for death
bool Obj::MarkedForDeath() const
{
  return killSwitch_;
}

void Obj::ResolveCollision(Obj *otherObj)
{
  resType type = otherObj->ResolutionType();
  if (resT_ == Other_ || type == Other_ || !ScreensActive())
    return;

  Health *hp;
  Physics *p;
  ColliderCircle* circle = reinterpret_cast<ColliderCircle*>(Has(Collider));
  Sprite* s;
  /*int color = static_cast<int>(color_);*/

  switch (resT_) //resT belongs to this
  {
  case Player_:
    switch (type) //type belongs to otherObj
    {
    case Enemy_:
      hp = Has(Health);
      if (hp)
      {
        hp->TakeDamage(otherObj->Has(Stats)->Attack()); // player

        // PLAY A SOUND
        Has(Audio)->Play(Audio::Damage_);
      }
      s = Has(Sprite);

      if (!hp->Invincible())
      {
        if (!s->isFlash())
          s->Flash(glm::vec3(1, 0, 0), s->GetRGBA(), 5);
        circle->Push(reinterpret_cast<ColliderCircle*>(otherObj->Has(Collider)));
      }
      break;
    case Lantern_:
      break;
    case Catalyst_:
      break;
    case Wall_:
      circle->ResolveWall(reinterpret_cast<ColliderSquare*>(otherObj->Has(Collider)));

      //wall collision resolution goes here
      break;
    }
    break;
  case Enemy_:
    switch (type)
    {
    case Player_:
      break;
    case Lantern_:
      hp = Has(Health);
      s = Has(Sprite);

      if (!hp->Invincible())
      {
        hp->TakeDamage(5);
        if (!s->isFlash())
          s->Flash(glm::vec3(1, 0, 0), s->GetRGBA(), 5);
        p = Has(Physics);
        circle->LanternPush();
      }
      else
      {
        if (!s->isFlash())
          s->Flash(glm::vec3(0, 0, 0), s->GetRGBA(), 3);
        p = Has(Physics);
        circle->LanternPush();
        //Play sound
      }
      break;
    case Wall_:
      //wall collision resolution goes here?
      break;
    }
    break;
  case Lantern_:
    if (type == Enemy_)
    {
      //Color(otherObj->Color());
      //Has(Audio)->TriggerSFX(true);
    }
    break;
  }
}

void Obj::Color(colorFlag color)
{
  color_ = color;
}

colorFlag Obj::Color() const
{
  return color_;
}

std::string& Obj::Name()
{
  return name_;
}

void Obj::MapName(std::string newName)
{
  mapName_ = newName;
}

std::string Obj::MapName()
{
  return mapName_;
}

std::string& Obj::Type()
{
  return type_;
}

bool Obj::IsDead()
{
  return isDead_;
}

void Obj::IsDead(bool toggle)
{
  isDead_ = toggle;
  //IsActive(toggle);
}

void Obj::Kill()
{
  Sprite *sp = Has(Sprite);
  sp->SetVisible(f);
  sp->SetRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
  Has(Collider)->IsActive(false);

  bool f = false;
  Behavior *be = Has(Behavior);
  if (be)
    be->IsActive(false);
  IsActive(f);
}

void Obj::Resurrect()
{
  Has(Collider)->IsActive(true);
  Reset();
  Has(Physics)->Velocity(0.0f, 0.0f);

  bool t = true;
  IsActive(t);
}

void Obj::Respawn()
{
  respawn_ = true;
}

void Obj::Reset()
{
  Health *hp = Has(Health);
  if (hp)
    hp->Reset();
  Sprite *sp = Has(Sprite);
  sp->SetVisible(t);
  //sp->SetRGBA(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

  if (resT_ == Enemy_)
  {
    BehaviorScript *beha = dynamic_cast<BehaviorScript*>(Has(Behavior));
    bool b = false;
    beha->IsAggroed(b);
    Has(Physics)->Velocity(0.0f, 0.0f);
    //Has(Sprite)->SetAlpha(CalculateAlphaforHealth());
  }
}

/// mw - resets all components
void Obj::ResetAll()
{
  int size = components_.size();
  for (int i = 0; i < size; ++i)
    components_[i]->Reset();
}

bool Obj::IsActive()
{
  return isActive_;
}

void Obj::IsActive(bool& toggle)
{
  bool b = toggle;
  int size = components_.size();

  for (int i = 0; i < size; ++i)
    components_[i]->IsActive(b);

  isActive_ = b;

  if (isTrigger_ && b)
    Reset();

  ParticleEmitter *part = Has(ParticleEmitter);
  if (part && !isTrigger_)
    part->SetParticlesVisible(isActive_);
}

bool Obj::IsRespawn()
{
  return respawn_;
}

void Obj::IsRespawn(bool respawn)
{
  respawn_ = respawn;
}

bool Obj::IsTrigger()
{
  return isTrigger_;
}

void Obj::IsTrigger(bool set)
{
  isTrigger_ = set;
}

bool Obj::PartsLast()
{
  return partsLast_;
}

void Obj::PartsLast(bool set)
{
  partsLast_ = set;
}

void Obj::ResolutionType(resType type)
{
  resT_ = type;
}

// mw
// sets res type given a string
void Obj::ResolutionType(std::string type)
{

  if (type.compare("enemy") == 0)
    resT_ = Enemy_;
  else if (type.compare("wall") == 0)
    resT_ = Wall_;
  else if (type.compare("catalyst") == 0)
    resT_ = Catalyst_;
  else if (type.compare("item") == 0)
    resT_ = Item_;
  else if (type.compare("player") == 0)
    resT_ = Player_;
  else if (type.compare("lantern") == 0)
    resT_ = Lantern_;
  else if (type.compare("no") == 0)
    resT_ = No_;
  else if (type.compare("trigger") == 0)
    resT_ = Trigger_;
  else
    resT_ = Other_;
}

// mw
// returns restype as a string
std::string Obj::ResolutionStringType()
{
  switch (resT_)
  {
  case Enemy_:
    return "enemy";
  case Wall_:
    return "wall";
  case Catalyst_:
    return "catalyst";
  case Item_:
    return "item";
  case Player_:
    return "player";
  case Other_:
    return "other";
  case No_:
    return "no";
  case Trigger_:
    return "trigger";
  }

  return std::string();
}

resType Obj::ResolutionType()
{
  return resT_;
}

void Obj::Hide(bool toggle)
{
  hide(toggle);
}

void Obj::Initialize()
{
  /*hide += EventZone::EventHandler::Bind(&Sprite::SetVisible, Has(Sprite));*/

  //Stats *stat = Has(Stats);
  //if(stat)
  //  if (stat->Class() < NotEnemy_)
  //  {
  //    Lantern& lant = Space::GetLantern();
  //    stat->light += EventZone::EventHandler::Bind(&Lantern::ChangeColor, &lant);
  //  }
}

void Obj::Shutdown()
{
  /*hide -= EventZone::EventHandler::Bind(&Sprite::SetVisible, Has(Sprite));*/

  //Stats *stat = Has(Stats);
  //if (stat)
  //  if (stat->Class() < NotEnemy_)
  //  {
  //    Lantern& lant = Space::GetLantern();
  //    stat->light -= EventZone::EventHandler::Bind(&Lantern::ChangeColor, &lant);
  //  }
}

Component ** Obj::GetPP(Component::compType type)
{
  int size = components_.size();

  for (int i = 0; i < size; ++i)
    if (components_[i]->Type() == type)
      return &(components_[i]);

  return NULL;
}

float Obj::CalculateKill()
{
  Health *health = Has(Health);
  if (health == nullptr)
    return 420.0f;

  float hp = (float)health->HitPoints();

  return (100.0f / hp) / 100.0f;
}

float Obj::CalculateAlphaforHealth()
{
  Health *health = Has(Health);
  if (health == nullptr)
    return 420.0f;

  float hp = (float)health->HitPoints();
  float max = (float)health->MaxHP();

  return (hp / max);
}

void Obj::Render()
{
  // If the object does not exist or is diabled dont draw
  if (!IsActive() && !isTrigger_)  //uncomment this and comment the if below to make triggers visible
    return;

  //if (isTrigger_ || !IsActive()) //this makes the trigger objs stay invisible even when active
  //  return;

  Sprite *objSprite = Has(Sprite);
  Transform *trans = Has(Transform);

  // If the object does not have the right components do not draw
  if (objSprite == nullptr || trans == nullptr)
    return;

  if ((objSprite->GetVisible()))
  {
    Shader* shader = objSprite->GetShader();

    if (!shader)
      return;

    // Get the mesh ids and sprite varaibles needed to send to shader
    SpriteSource* spriteSource = objSprite->GetSpriteSource();
    Mesh * objMesh = &spriteSource->GetMesh();
    GLuint VAO = objMesh->MeshGetID();
    glm::vec2 uv = spriteSource->GetUV(objSprite->GetFrameIndex());
    glm::vec4 RGBA = objSprite->GetRGBA();
    glm::mat3 *m = trans->TransMatrix();

    //Bind the opengl vao and textures to draw
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, spriteSource->GetId());

    shader->UseShader();

    glm::mat3 viewMat = Camera::ViewMat() * *m;

    // Send everything needed to the shader
    shader->SendInt("textId", 0);
    shader->SendVec2("uv", uv[0], uv[1]);
    shader->SendMat3("transform", viewMat);
    shader->SendVec4("rgba", RGBA[0], RGBA[1], RGBA[2], RGBA[3]);
    shader->SendMat4("OrthoView", Camera::ProjectionMat());

    //Tell OpenGL to draw the object
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    shader->Unuse();
  }

  if (!partsLast_)
  {
    // added by megan -- draws sub objects
    std::vector<Obj*>subList = GetSubObjList();
    for (Obj* obj : subList)
      obj->Render();
  }
}

resType GetResTypeFromString(std::string resString)
{
  resType type = No_;

  if (resString.compare("Player") == 0)
    type = Player_;
  else if (resString.compare("Lantern") == 0)
    type = Lantern_;
  else if (resString.compare("Other") == 0)
    type = Other_;
  else if (resString.compare("Item") == 0)
    type = Item_;
  else if (resString.compare("Catalyst") == 0)
    type = Catalyst_;
  else if (resString.compare("Wall") == 0)
    type = Wall_;
  else if (resString.compare("Enemy") == 0)
    type = Enemy_;
  //else if (resString.compare("No") == 0)
  //  type = No_;

  return type;
}

std::string GetStringFromResType(resType type)
{
  switch (type)
  {
  case Enemy_:
    return "Enemy";
  case Wall_:
    return "Wall";
  case Catalyst_:
    return "Catalyst";
  case Item_:
    return "Item";
  case Lantern_:
    return "Lantern";
  case Player_:
    return "Player";
  case Other_:
    return "Other";
  case Trigger_:
    return "Trigger";
  default:
    return "";
  }
}
