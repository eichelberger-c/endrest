///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Audio.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Manages audio component of object
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "AudioEngine.h"
#include "Timer.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Audio : public Component
{
public:
  Audio();
  Audio(const Audio& ogAudio);
  Audio(Component *audio);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset() {}

  typedef enum
  {
    Invalid_ = -1,
    Default_,
    Collision_,
    Attack_,
    Damage_,
    Death_,
    Aggro_,
    None_,
  }type;

  void Play(type sound);
  bool isPlaying(type t);
  //float Volume(type t);
  void Volume(type t, float volume);

  void AddSound(type t, std::string filename); // need path currently
  std::string GetAudioFile(type t); // returns with path

private:
  // we only storing strings now yay
  std::map<type,std::string>sounds_;

  //static ConstructPtr constructor_; <- this is sonya's, might need later?
};

///---------------------------------FUNCTIONS-----------------------------------



