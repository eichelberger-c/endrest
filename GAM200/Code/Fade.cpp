///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Fade.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Does some graphic stuff man, Frankenstein style
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

#include "Fade.h"
#include "Sprite.h"
#include "Transform.h"
#include "Timer.h"
#include "Lighting.h"
#include "GameStateManager.h"
#include "Spaces.h"
#include "Menu.h"
#include "EndGs.h"

static AudioEngine* aud_ = nullptr;
ScreenManager* screenman = nullptr;

Fade::Fade() : Obj("Fade")
{
}

void Fade::Initialize()
{
	Sprite* sprite = new Sprite(std::string("Data/Textures/default.png"), std::string(""),
		glm::vec2(1, 1), std::string("Shaders/defaultVert.glsl"), std::string("Shaders/defaultFrag.glsl"), true);
	sprite->SetRGBA(glm::vec4(0, 0, 0, 1));
	Transform* backgroundTransform = new Transform(0, 0);
	backgroundTransform->Scale(1920, 1080);
	Timer* timer = new Timer(2, CountUp_, 2);
  aud_ = static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));
  screenman = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

	Add(sprite);
	Add(timer);
	Add(backgroundTransform);
}

void Fade::Update(float dt)
{
  if (screenman->ScreensStarted())
    return;

	if (reinterpret_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->GetCurrentGameState() != GameGs)
		return;
	Obj::Update(dt);

  static soundObject s1 = nullptr;
	if (init_)
	{
    // plays carriage sound
    if (type_ == Intro_)
      s1 = aud_->GetSoundObject("Data/Audio/HorseCarriage.wav");
    else if (type_ = Outro_)
    {
      screenman->KillAllEnemies();
      screenman->PauseBackground(true);
      s1 = aud_->GetSoundObject("Data/Audio/EndDialogue.wav");
    }
    else
      s1 = nullptr;

    // plays
    aud_->Play(s1);

	  init_ = false;
	}
  else if (aud_->IsPlaying(s1))
  {
    return;
  }
	else
	{
    // starts screens
    static bool once = true;
    if (once)
    {
      screenman->ChangeCurrentScreen(screenman->StartScreen());
      aud_->Play(screenman->OGBackgroundMusic(), stream_);
      once = false;
    }

		frames_--;
		Sprite* spritz = Has(Sprite);
		Timer* timer = Has(Timer);
		Lighting* light = Space::GetPlayer().Has(Lighting);

		glm::vec4 lightA = light->Color();
		glm::vec4 rgba = spritz->GetRGBA();

		if (frames_ <= 0)
		{ 
      if (type_ == Intro_)
      {
        rgba.a -= .1f;
        if (lightA.a <= .22f)
          lightA.a += .01f;
        if (rgba.a <= 0)
        {
          bool f = false;
          IsActive(f);

          CanPause(true);
          screenman->StartScreens();
        }
      }
      else if (type_ == Outro_)
      {
        rgba.a += .1f;
        if (lightA.a >= 0)
          lightA.a -= .01f;
        if (rgba.a >= 1.2)
        {
          screenman->StopScreens();
          screenman->ChangeCurrentScreen("EntranceToEndrest");
          screenman->BackgroundMusic("Data/Audio/EndRestMusic/EndRestCredits.wav");

          SetEnding(Win);
          static_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(EndGs);
        }
      }
			timer->Reset();
			if (rgba.a <= 0)
			{
				bool f = false;
				IsActive(f);
        
        CanPause(true);
        screenman->StartScreens(); 
			}
			spritz->SetRGBA(rgba);
			light->Color(lightA);
			frames_ = 15;
		}
	}
}

void Fade::SetOutro()
{
  bool t = true;
  IsActive(t);
  init_ = true;
  Has(Sprite)->SetRGBA(glm::vec4(1, 1, 1, 0.001));
  type_ = Outro_;

  screenman->StopScreens();
}
