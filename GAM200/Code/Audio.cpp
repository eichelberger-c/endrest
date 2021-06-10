///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Audio.cpp
//
//  Author: Megan Works
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
#include "Audio.h"
#include "Engine.h"
#include "Spaces.h"
#include "Component.h"

///----------------------------------GLOBALS-----------------------------------
static AudioEngine *engine = nullptr;


///---------------------------------FUNCTIONS-----------------------------------
///-------------------------------------
/// Brief: constructs audio component
Audio::Audio() : Component(Audio_)
{
  if (!engine)
    engine = dynamic_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));

  //Space::AddAudio(this);
}

///-------------------------------------
/// Brief: copy constructor
Audio::Audio(const Audio& ogAudio) : Component(Audio_)
{
  IsActive(ogAudio.IsActive());

  sounds_ = ogAudio.sounds_;

  //Space::AddAudio(this);
}

///-------------------------------------
/// Brief: thingy 
Audio::Audio(Component * audio) : Component(Audio_)
{

  IsActive(audio->IsActive());
}

///-------------------------------------
/// Brief:clones audio comp 
Component* Audio::Clone() const
{
  Audio *clone = new Audio();

  if (!clone)
    return nullptr;

  //clone->IsActive(IsActive());

  return clone;
}

///-------------------------------------
/// Brief: updates audio component
void Audio::Update(float dt)
{
  if (!IsActive())
    return;
}

void Audio::Play(type t)
{
  std::string filename = GetAudioFile(t);

  if (filename.empty())
    return;

  engine->Play(filename);
}

bool Audio::isPlaying(type t)
{
  std::string filename = GetAudioFile(t);

  if (filename.empty())
    return false;

  return engine->IsPlaying(engine->GetSoundObject(filename));
}

void Audio::Volume(type t, float volume)
{
  engine->Volume(engine->GetSoundObject(GetAudioFile(t)), volume);
}



void Audio::AddSound(type t, std::string filename)
{
  if (t <= Invalid_ || t >= None_)
    return;

  sounds_[t] = filename;
}

std::string Audio::GetAudioFile(type t)
{
  if (t <= Invalid_ || t >= None_)
    return "";

  std::map<type, std::string>::iterator it = sounds_.find(t);
  if (it == sounds_.end())
    return "";

  return it->second;
}
