///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: AudioEngine.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Creates and manages audio system
//         Available functions:
//                             
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
#pragma once

#include "stdafx.h"
#include <fmod.hpp>
#include <fmod_errors.h>
//#include <fmod_studio.hpp>
#include "System.h"
#include "Utils.h"

///-------------------------------------
/// sound is either sound effect (SFX) or background music (stream)
typedef enum SoundType
{
  sfx_,    // created during game object initialization
  stream_, // created during gamestate load 
  all_,
}SoundType;


///-------------------------------------
/// Sound Object containing info needed in audio system
typedef struct SoundObject
{
  std::string filename;   // file path
  FMOD::Sound *sound;      // FMOD sound class
  int channelID;  // stores channel id
  FMOD::Channel *channel;
  SoundType soundType;  // sound type
  float volume; // sound object's volume
}SoundObject;

typedef struct SoundObject *soundObject; // sound object pointer

///-------------------------------------
/// handles calls to FMOD API
class FMODImplementation
{
  FMODImplementation();
  ~FMODImplementation();

  void SystemUpdate();

  // FMOD sound system
  FMOD::System *system;
  unsigned int version;
  void *extradatadriver;
  int currentChannelID;

  typedef std::pair<SoundType, std::pair<FMOD::Channel*,float>> channelObject;
  typedef std::map<std::string, soundObject> SoundMap;
  typedef std::map<int, channelObject> ChannelMap;

  SoundMap sounds;     // sound container
  ChannelMap channels; // channel container

  friend class AudioEngine; // gives audio engine access to implementation class
};

///-------------------------------------
/// handles audio logic
class AudioEngine : public System
{
public:
  AudioEngine() : System("AudioEngine") {};
  ~AudioEngine();
  virtual void Initialize(); // creates and initializes sound system -- called once at engine initialization
  virtual void Update(float dt);     // updates sound system -- called every game loop


private:
  // name is the filename with no path or extension
  soundObject CreateSoundObject(std::string filename, SoundType type = sfx_, float volume = 1.0f);
  void DestroySoundObject(soundObject soundObject);
  void LoadSound(soundObject object, bool loop = false);
  void ReleaseSound(soundObject object);
  void ReleaseSound(int channelID);

public:
  soundObject GetSoundObject(std::string name, SoundType type = sfx_, float volume = 1.0f);

  // plays sound
  int Play(soundObject object, bool loop = false);
  int Play(std::string filename, SoundType type = sfx_, bool loop = false);

  // pause controls
  void Pause(bool pause); // pauses/unpauses all
  void Pause(SoundType type, bool pause);
  void Pause(soundObject object, bool pause);
  bool isPaused(soundObject object);

  std::vector<soundObject> Pause(); // pauses all currently playing/unpaused & returns channel IDs

  // mute controls
  void Mute(bool mute); // mutes/unmutes all
  void Mute(SoundType type, bool pause);
  void Mute(soundObject object, bool mute);

  // stop controls
  void Stop(); // stops all
  void Stop(SoundType type);
  void Stop(soundObject object);

  // volume controls
  void Volume(float volume); // sets master volume
  void Volume(SoundType type, float volume); // 0.0 - 1.0, 
  void Volume(soundObject object, float volume);
  // float Volume(FMOD::Channel* channel);

  bool IsPlaying(soundObject object);
  bool IsPlaying(int channelID);

  // error checking for FMOD functions
	static void ERRCHECK(FMOD_RESULT result, const char * function);

private:
  // channel related functions
  void Stop(int channelID);
  void SetChannelVolume(int channelID, float volume);
  FMOD::Channel* Channel(soundObject object);
  FMOD::Channel* Channel(std::string filename);

  bool IsPlaying(FMOD::Channel* channel);
  bool isPaused(FMOD::Channel* channel);

  // different volumes -- these cap at 1.0
  float masterVolume_ = 0.8f;
  float sfxVolume_ = 0.8f;
  float streamVolume_ = 0.8f;
  bool allMuted_ = false;
};
