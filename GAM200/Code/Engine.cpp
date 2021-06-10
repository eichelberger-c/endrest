///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Engine.cpp
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Engine.h"
#include "Spaces.h"
#include "GameStateManager.h"
#include "AudioEngine.h"
#include "Graphics.h"
#include "ColliderSys.h"
#include "PhysicsSys.h"
#include "Input.h"
#include "Utils.h"
#include "FrameRateController.h"
#include "ObjFactory.h"
#include "ObjManager.h"
#include "SpawnerManager.h"
//#include "ScreenManager.h"
#include "Serialization.h"
#include "Editor.h"
#include "PushBlockManager.h"
#include "DebugSystem.h"
#include "EventManager.h"
#include "TextureSys.h"
#include "ShaderSys.h"
#include "Menu.h"

///----------------------------------GLOBALS-----------------------------------
static Engine engine;
FrameRateManager FPSController(60.0f);
///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: initialize the game engine
void EngineInit()
{
  // initialize the Tracelog
  TraceInit();

  TraceMessage(TraceLog, "Engine: Init");

  UtilsInit(); //initialize the game timer

  engine.AddSystem(new AudioEngine());
  engine.AddSystem(new GameStateManager());
  engine.AddSystem(new ColliderSys());
  engine.AddSystem(new PhysicsSys());
  engine.AddSystem(new Input());
  engine.AddSystem(new SpawnerManager());
  engine.AddSystem(new ScreenManager());
  engine.AddSystem(new Graphics());
  engine.AddSystem(new ObjFactory());
  engine.AddSystem(new PushBlockManager());
  engine.AddSystem(new TextureSys());
  engine.AddSystem(new ShaderSys());
  engine.SystemsInit();

  engine.AddSystem(new EventManager()); //can't be Init'ed here

  Space::SpaceInit();


  // needs to be added and initialized after space is initialized
  engine.AddSystem(new Data());
  (dynamic_cast<Data*>(EngineGetSystem("Data")))->Initialize();

  engine.AddSystem(new Editor());
  (dynamic_cast<Editor*>(EngineGetSystem("Editor")))->Initialize();

  engine.AddSystem(new DebugSystem());
  (dynamic_cast<DebugSystem*>(EngineGetSystem("DebugSystem")))->Initialize();

  (dynamic_cast<EventManager*>(EngineGetSystem("EventManager")))->Initialize();
}

///-------------------------------------
/// Brief: update engine/call game loop
void EngineUpdate(float dt)
{
  TraceMessage(TraceLog, "Engine: Game loop started.");

  engine.GameLoop(dt);
}

///-------------------------------------
/// Brief: shutdown the game engine
void EngineShutdown()
{
  TraceMessage(TraceLog, "Engine: Shutdown");
  FPSController.ReportFrameStats();
  engine.DestroySystems();
  // Shutdown the Tracing/Logging module.
  TraceShutdown();
}

///-------------------------------------
/// Brief: call like this: AddSystem(new SystemName());
void Engine::AddSystem(System * system)
{
  if (system == NULL)
  {
    TraceMessage(TraceLog, "Engine: System pointer invalid. Did not add %s.", system->Name());
    return;
  }

  systems_[system->Name()] = system;
  TraceMessage(TraceLog, "Engine: System %s added.", system->Name().c_str());
}

///-------------------------------------
/// Brief: deletes all the systems in the engine
void Engine::DestroySystems()
{
  std::map<std::string, System*>::iterator it;
  for (it = systems_.begin(); it != systems_.end(); ++it)
  {
if ((*it).first != "Debug")
delete it->second;
  }
}

///-------------------------------------
/// Brief: initializes all the systems in the engine (calls their init function)
void Engine::SystemsInit()
{
	std::map<std::string, System*>::iterator it;
	for (it = systems_.begin(); it != systems_.end(); ++it)
		it->second->Initialize();
}

///-------------------------------------
/// Brief: main game loop
void Engine::GameLoop(float dt)
{
	while (dynamic_cast<GameStateManager*>(systems_["GameStateManager"])->ManagerIsRunning())
	{
		// Check if a close event has occured and handle it
		glfwPollEvents();
		if (glfwWindowShouldClose(Graphics::Window()))
			dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(GsQuit);

		CheckMinimized();

		/* time_ += 1.0f / float(Graphics::FPS());
		 if (time_ < 1 || !(int(glfwGetTime()) % int(time_)) || paused_)
		   return;*/
		dt = 1.0f / float(Graphics::FPS());
		if ((dt > 1 || dt < 0 || Editor::Paused() || MenuIsOpen()) && !paused_)
			dt = 0;

		delay_ -= dt;
		if ((delay_ > 0 || paused_) && dt > 0)
			continue;

		engine.UpdateSystems(dt);
	}
}

///-------------------------------------
/// Brief: updates all the systems in the engine (calls their update function)
void Engine::UpdateSystems(float dt)
{
	std::map<std::string, System*>::iterator it;
	for (it = systems_.begin(); it != systems_.end(); ++it)
		it->second->Update(dt);

	float Duration = 0.f;
	float fps = float(Graphics::FPS());
	if (fps > MAX_FPS)
		Duration = fps / MAX_FPS;
	else
		Duration = 1.f;
	delay_ = Duration * dt;
}

///-------------------------------------
/// Brief: retrieves a system from the engine
System* EngineGetSystem(std::string name)
{
	std::map<std::string, System*>::iterator it;

	it = engine.systems_.find(name); // searches for system by name

	if (it == engine.systems_.end())
		return NULL; // object not found, null pointer returned

	return it->second; // returns pointer to system object
}

void PlayerInputIsActive(bool toggle)
{
	dynamic_cast<Input*>(engine.systems_["Input"])->IsActive(toggle);
}

std::string EngineGetCurrentScreen()
{
	ScreenManager *man = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

	return man->CurrentScreen()->GetName();
}

void Pause()
{
	engine.paused_ = true;
	if(!MenuIsOpen())
		Menu();
}

void UnPause()
{
	engine.paused_ = false;
}

void Engine::CheckMinimized()
{
  static std::vector<soundObject>sounds;
  if (glfwGetWindowAttrib(Graphics::Window(), GLFW_FOCUSED) && paused_)
  {
    UnPause();
    AudioEngine* ae = dynamic_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));

    for (soundObject s : sounds)
      ae->Pause(s, false);
  }
		

  if (!glfwGetWindowAttrib(Graphics::Window(), GLFW_FOCUSED) && paused_ == false)
  {
    Pause();
    sounds = dynamic_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Pause();
  }
	    
}


void SoundEvent(SoundData & data)
{
  AudioEngine *xxx = dynamic_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));

  if (data.play)
    xxx->Play(data.filename, sfx_, true);
  else
    xxx->Stop(xxx->GetSoundObject(data.filename));
}
