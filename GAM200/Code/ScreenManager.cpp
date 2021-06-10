///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ScreenManager.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - ScreenManager
//      - ~ScreenManager
//      - Initialize
//      - Update
//      - CurrentScreen
//      - ChangeCurrentScreen
//      - CurrentObjList
//      - CreateNewScreen
//      - CreateNewArea
//      - AddScreen
//      - StartScreen
//      - FindScreen
//      - ResetArea
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Spaces.h"
#include "Graphics.h"
#include "Menu.h"
#include "ScreenManager.h"

///----------------------------------GLOBALS-----------------------------------
static AudioEngine* aEngine = nullptr;
static soundObject idle = nullptr;
static soundObject music = nullptr;
static soundObject pauseMuse = nullptr;
static soundObject aggro = nullptr;

 std::vector<soundObject>pausedSounds;
static bool gamePaused = false;
static bool screensActive = false;
static bool aggroed = false;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: constructor
ScreenManager::ScreenManager() : System("ScreenManager")
{
  aEngine = reinterpret_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));
}

///-------------------------------------
/// Brief: deletes all screens
ScreenManager::~ScreenManager()
{
  ScreenMap::iterator itEnd = screens_.end();
  for (ScreenMap::iterator it = screens_.begin(); it != itEnd; it++)
    delete it->second;

  screens_.clear();
}

///-------------------------------------
/// Brief: initializes screens
void ScreenManager::Initialize()
{
  music = aEngine->GetSoundObject(ogBackground_, stream_);
  idle = aEngine->GetSoundObject(idleBackground_, sfx_);
  pauseMuse = aEngine->GetSoundObject("Data/Audio/EndRestMusic/EndRestTitle.wav", stream_, 0.1f);
  aggro = aEngine->GetSoundObject("Data/Audio/EndRestMusic/EndRestTrouble.wav", stream_, 0.55f);
}

///-------------------------------------
/// Brief: updates screens
void ScreenManager::Update(float dt)
{
  static bool test = false;

  // switches screens
  if (next_ != current_)
  {
    numaggro_ = 1;
    RemoveAggro();

    Area *temp = reinterpret_cast<Area*>(current_);
    current_ = next_;

    // resets areas if needed
    if (temp)
      ResetArea(temp);

    // plays enemy sounds
    if (next_->AllEnemiesDead() == false && !aEngine->IsPlaying(idle) && screensActive && begin_)
    {
      aEngine->Play(idle, true);
      test = true;
    }
  }

  if (current_ == nullptr)
    return;

  // turns off enemy sounds
  if (test == true && current_->AllEnemiesDead())
  {
    test = false;
    idle->channel->stop();
  }

  // pause menu audio stuff
  if (gamePaused == false && MenuIsOpen())
  {
    pausedSounds = aEngine->Pause(); // pauses all audio
    gamePaused = true;
    screensActive = false;

    aEngine->Play(pauseMuse);
  }
  else if (gamePaused == true && !MenuIsOpen())
  {
    aEngine->Stop(pauseMuse);
    
    // unpauses paused audio
    for (soundObject s : pausedSounds)
      aEngine->Pause(s, false);

    gamePaused = false;
    screensActive = true;
    pausedSounds.clear();
  }

  // if other background has stopped (not paused/muted)
  // switch back to regular track
  if (!aEngine->IsPlaying(music) && current_->GetName() != "ClockRoom")
  {
    music = aEngine->GetSoundObject(ogBackground_, stream_);
    aEngine->Pause(music, false);
  }
}

///-------------------------------------
/// Brief: checks for player at screen borders
void ScreenManager::BorderCheck(float dt)
{
  Area *temp = reinterpret_cast<Area*>(current_);

  float width = 1920.0f;
  float height = 1080.0f;

  float west = width / (-2.0f);
  float east = width / 2.0f;
  float north = height / 2.0f;
  float south = height / (-2.2f);

  if (temp)
  {
    Player &player = Space::GetPlayer();
    Transform *transform = player.Has(Transform);
    Physics *phy = player.Has(Physics);
    Vec2D vel = phy->Velocity();
    Vec2D trans = transform->Translation();
    float x = trans.X();
    float y = trans.Y();

    // east
    // check for at borders
    if (x >= width / 2.1f)
    {
      if (temp->GetEastArea().empty())
      {
        // east border
        vel.X(0.0);
        phy->Velocity(vel);
        phy->Update(dt);
      }
      else if (x >= east)
      {
        // change screens & reposition player
        ChangeCurrentScreen(temp->GetEastArea());
        trans.X(width / -2.1f);
        transform->Translation(trans);
      }
      else
      {
        vel.X(player.Speed());
        phy->Velocity(vel);
        phy->Update(dt);
      }
    }

    // west
    if (x <= width / -2.1f)
    {
      if (temp->GetWestArea().empty())
      {
        // west border
        vel.X(0.0);
        phy->Velocity(vel);
        phy->Update(dt);
      }
      else if (x <= west)
      {
        // change screens & reposition player
        ChangeCurrentScreen(temp->GetWestArea());
        trans.X(width / 2.1f);
        transform->Translation(trans);
      }
      else
      {
        vel.X(-player.Speed());
        phy->Velocity(vel);
        phy->Update(dt);
      }
    }

    // north
    if (y >= height / 3.0f)
    {
      if (temp->GetNorthArea().empty())
      {
        // north border
        vel.Y(0.0);
        phy->Velocity(vel);
      }
      else if (y >= north)
      {
        // change screens & reposition player
        ChangeCurrentScreen(temp->GetNorthArea());
        trans.Y(height / -2.21f);
        transform->Translation(trans);
      }
      else
      {
        vel.Y(0.0);
        phy->Velocity(vel);
      }
    }

    // south
    if (y <= height / -2.2f)
    {
      if (temp->GetSouthArea().empty())
      {
        // south border
        vel.Y(0.0);
        phy->Velocity(vel);
      }
      else if (y < south)
      {
        // change screens & reposition player
        ChangeCurrentScreen(temp->GetSouthArea());
        trans.Y(height / 3.1f);
        transform->Translation(trans);
      }
      else
      {
        vel.Y(-player.Speed());
        phy->Velocity(vel);
      }
    }
  }
}

///-------------------------------------
/// Brief: returns pointer to current screen
Screen * ScreenManager::CurrentScreen()
{
  return current_;
}

///-------------------------------------
/// Brief: changes to new screen, assumes that screen already exists
///        currently nonfunctioning
void ScreenManager::ChangeCurrentScreen(Screen * screen)
{
  next_ = screen;

  if (screen->GetName().find("ClockRoom") != -1)
    aEngine->Pause(music, true);
  else if (screen->GetName().find("TheChamber") != -1)
    BackgroundMusic("Data/Audio/EndRestMusic/EndRestBoss.wav");
  else if (aEngine->isPaused(music))
    aEngine->Pause(music, false);
}

///-------------------------------------
/// Brief: changes screen
void ScreenManager::ChangeCurrentScreen(std::string screenname)
{
  ChangeCurrentScreen(FindScreen(screenname));
}

///-------------------------------------
/// Brief: changes screen
void ScreenManager::ChangeCurrentScreen(std::string screenname, direction dir)
{
  Player &player = Space::GetPlayer();
  Transform *transform = player.Has(Transform);
  Vec2D trans = transform->Translation();
  float x = trans.X();
  float y = trans.Y();
  float width = 1920.0f;
  float height = 1080.0f;

  ChangeCurrentScreen(FindScreen(screenname));
  switch (dir)
  {
  case North_:
    trans.Y(height / -3.35f);
    transform->Translation(trans);
    break;
  case South_:
    trans.Y(height / 3.50f);
    transform->Translation(trans);
    break;
  case East_:
    trans.X(width / -2.50f);
    transform->Translation(trans);
    break;
  case West_:
    trans.X(width / 2.50f);
    transform->Translation(trans);
    break;
  }
}

///-------------------------------------
/// Brief: returns current screen's object list
std::vector<Obj*> ScreenManager::CurrentObjList()
{
  if (!current_)
    return std::vector<Obj*>();

  return current_->GetObjectList();
}

///-------------------------------------
/// Brief: Creates new screen
Screen* ScreenManager::CreateNewScreen(std::string name)
{
  Screen* screen = new Screen(name);

  if (!screen)
    exit(-33);

  screens_[name] = screen;

  return screen;
}

///-------------------------------------
/// Brief: Creates new area screen and adds to map
Area* ScreenManager::CreateNewArea(std::string areaName)
{
  Area* area = new Area(areaName);

  if (!area)
    exit(-33);

  screens_[areaName] = area;

  return area;
}

///-------------------------------------
/// Brief: creates new area and attaches to give direction of CURRENT screen
/// CANNOT USE THIS IF CURRENT SCREEN IS NOT AN AREA
Area* ScreenManager::CreateNewArea(std::string areaName, direction dir)
{
  Area* temp = reinterpret_cast<Area*>(current_);

  if (!temp)
  {
    TraceMessage(TraceLog, "ScreenManager: CreateNewArea: current screen is not an area!!");
    exit(-33);
  }

  Area* area = CreateNewArea(areaName);

  switch (dir)
  {
  case North_:
    temp->AttachNorthArea(areaName);
    area->AttachSouthArea(temp->GetName());
    break;
  case South_:
    temp->AttachSouthArea(areaName);
    area->AttachNorthArea(temp->GetName());
    break;
  case East_:
    temp->AttachEastArea(areaName);
    area->AttachWestArea(temp->GetName());
    break;
  case West_:
    temp->AttachWestArea(areaName);
    area->AttachEastArea(temp->GetName());
    break;
  }

  return temp;
}

///-------------------------------------
/// Brief: Adds screen to screen map
void ScreenManager::AddScreen(std::string name, Screen * screen)
{
  screens_[name] = screen;

  if (screen->IsStartScreen() == true)
    start_ = screen;
}

///-------------------------------------
/// Brief: sets start screen
void ScreenManager::StartScreen(Screen * screen)
{
  // unsets old start screen
  if (start_)
  {
    start_->IsStartScreen(false);
  }
  
  start_ = screen;
  screen->IsStartScreen(true);
}

///-------------------------------------
/// Brief: returns start screen
Screen * ScreenManager::StartScreen()
{
  return start_;
}

///-------------------------------------
/// Brief: searches map for screen by name
Screen * ScreenManager::FindScreen(std::string name)
{
  return screens_[name];
}

///-------------------------------------
/// Brief: checks if puzzle is complete
bool ScreenManager::ScreenIsComplete(std::string screenName)
{
  Area *a = reinterpret_cast<Area*>(screens_[screenName]);
  if (a)
  {
    PushBlock *b = a->GetPushBlockPuzzle();
    if (b)
      return b->isSolved();
  }

  return false;
}

///-------------------------------------
/// Brief: sets door
void ScreenManager::SetDoor(DoorData & data)
{
  Screen *s = FindScreen(data.screen);
  Behavior *b = s->FindDoor(data.doorID);
  b->StateNext(data.newState);
}

///-------------------------------------
/// Brief: calls all area reset functions
void ScreenManager::ResetArea(Area * area)
{
  if (!area)
    return;

  area->ResetPuzzles();
  area->ResetEnemies();
}

///-------------------------------------
/// Brief: resets ALL areas, used to restart game
void ScreenManager::ForceGameReset()
{
  ScreenMap::iterator it;
  ScreenMap::iterator itEnd = screens_.end();

  for (it = screens_.begin(); it != itEnd; ++it)
  {
    Area *area = reinterpret_cast<Area*>(it->second);

    if (area)
      area->ForceResetAll();
  }
}

///-------------------------------------
/// Brief: changes background to diff music
void ScreenManager::BackgroundMusic(std::string filename)
{
  if (music->filename == ogBackground_)
    aEngine->Pause(music); // pauses reg music
  else
    aEngine->Stop(music);

  currentBackground_ = filename;
  music = aEngine->GetSoundObject(filename, stream_);
  aEngine->Play(music);
}

///-------------------------------------
/// Brief: mutes current music
void ScreenManager::MuteBackground(bool mute)
{
  aEngine->Mute(music, mute);
}

///-------------------------------------
/// Brief: stops current background 
void ScreenManager::StopBackground()
{
  aEngine->Stop(music);
}

///-------------------------------------
/// Brief: pauses current background
void ScreenManager::PauseBackground(bool pause)
{
  aEngine->Pause(music, pause);
}

///-------------------------------------
/// Brief: returns background filename
std::string ScreenManager::BackgroundMusic()
{
  return currentBackground_;
}

///-------------------------------------
/// Brief: returns og background filename
std::string ScreenManager::OGBackgroundMusic()
{
  return ogBackground_;
}

///-------------------------------------
/// Brief: starts screens
void ScreenManager::StartScreens()
{
  screensActive = true;
  begin_ = true;

  bool t = false;

  (&Space::GetPlayer())->GetSubObject("HealthBar")->IsActive(begin_);
  (&Space::GetPlayer())->GetSubObject("Lantern")->IsActive(t);
}

void ScreenManager::StopScreens()
{
  screensActive = false;
  begin_ = false;

  (&Space::GetPlayer())->GetSubObject("HealthBar")->IsActive(begin_);
}

///-------------------------------------
/// Brief: returns if started
bool ScreenManager::ScreensStarted()
{
  return begin_;
}

///-------------------------------------
/// Brief: adds to num aggroed
void ScreenManager::AddAggro()
{
  ++numaggro_;

  if (!aEngine->IsPlaying(aggro) && current_->GetName().compare("TheChamber") != 0)
    BackgroundMusic(aggro->filename);
}

///-------------------------------------
/// Brief: decrements num aggroed
void ScreenManager::RemoveAggro()
{
  --numaggro_;

  if (numaggro_ < 0)
  {
    numaggro_ = 0;
    return;
  }
  else if (numaggro_ == 0)
  {
    if (aEngine->IsPlaying(aggro))
      aEngine->Stop(aggro);
  }
}

void ScreenManager::KillAllEnemies()
{
  if (current_)
    current_->KillAll();
}

///-------------------------------------
/// Brief: sets screens active
void ScreensActive(bool active)
{
  screensActive = active;
}

///-------------------------------------
/// Brief: returns if screens are active
bool ScreensActive()
{
  return screensActive;
}
