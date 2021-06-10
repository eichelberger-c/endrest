///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: AudioEngine.cpp
//
//  Author: Megan Works
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief: Creates and manages audio system
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
#include "stdafx.h"
#include <algorithm>
#include "FileUtils.h"
#include "AudioEngine.h"

static int MAX_CHANNELS = 64;

static FMODImplementation *fmodImp = nullptr;
static FMOD_RESULT result;

///-------------------------------------
/// Brief: creates FMOD sound system
FMODImplementation::FMODImplementation() : system(NULL), version(0), extradatadriver(0), currentChannelID(0)
{
  // creates sound system
  result = FMOD::System_Create(&system);
  AudioEngine::ERRCHECK(result, "1FMODImplementation");

  // checks fmod version
  result = system->getVersion(&version);
  AudioEngine::ERRCHECK(result, "2FMODImplementation");

  // initializes fmod low-level api
  result = system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, &extradatadriver);
  AudioEngine::ERRCHECK(result, "3FMODImplementation");
}

///-------------------------------------
/// Brief: shutsdown the sound system
FMODImplementation::~FMODImplementation()
{
  result = system->close();
  AudioEngine::ERRCHECK(result, "~FMODImplementation");

  result = system->release();
  AudioEngine::ERRCHECK(result, "~FMODImplementation");
}

///-------------------------------------
/// Brief: updates the sound system
void FMODImplementation::SystemUpdate()
{
  std::vector<ChannelMap::iterator> stopped;
  ChannelMap::iterator itEnd = channels.end();
  
  // checks for channels that aren't playing and adds to vec
  for (ChannelMap::iterator it = channels.begin(); it != itEnd; it++)
  {
    if (it == itEnd)
      break;

    bool isPlaying = false;

    result = it->second.second.first->isPlaying(&isPlaying);
    AudioEngine::ERRCHECK(result, "SystemUpdate");

    if (!isPlaying)
      stopped.push_back(it);
  }

  // removes stopped channels
  for (ChannelMap::iterator it : stopped)
    channels.erase(it);

  stopped.clear();
  
  // updates fmod system
  result = system->update();
  AudioEngine::ERRCHECK(result, "SystemUpdate"); 
}

///-------------------------------------
/// Brief: shuts down audio system
AudioEngine::~AudioEngine()
{
  // release all sound objects if not already done
  TraceMessage(TraceLog, "Audio Engine: Shutdown");

  FMODImplementation::SoundMap::iterator it;
  FMODImplementation::SoundMap::iterator itEnd = fmodImp->sounds.end();

  for (it = fmodImp->sounds.begin(); it != itEnd; it++)
    DestroySoundObject(it->second);

  fmodImp->sounds.clear();

  if (fmodImp)
    delete fmodImp;
}

///-------------------------------------
/// Brief: initializes audio engine
void AudioEngine::Initialize()
{
  fmodImp = new FMODImplementation;
}

///-------------------------------------
/// Brief: updates audio engine
void AudioEngine::Update(float dt)
{
  // updates sound system
  fmodImp->SystemUpdate();
}

///-------------------------------------
/// Brief: checks for errors in audio system
void AudioEngine::ERRCHECK(FMOD_RESULT result, const char * function)
{
  if (result != FMOD_OK)
    TraceMessage(TraceLog, "Error in Audio System: Error Code "
                 "%d - %s\nCheck function %s\n", result, FMOD_ErrorString(result), function);
}

///-------------------------------------
/// Brief: creates a new sound object
soundObject AudioEngine::CreateSoundObject(std::string filename, SoundType type, float volume)
{
  soundObject object = new SoundObject; // allocates memory for sound object

  if (!object)
    return nullptr;

  object->filename = filename.c_str();
  object->channelID = 0;
  object->channel = nullptr;
  object->sound = nullptr;
  object->soundType = type;
  object->volume = volume;

  fmodImp->sounds[object->filename] = object; // adds sound object to sound object container

  return object; // returns pointer to sound object
}

///-------------------------------------
/// Brief: destroys a sound object
void AudioEngine::DestroySoundObject(soundObject object)
{
  // release sound of sound object
  ReleaseSound(object);

  if (object)
    delete object;
}

///-------------------------------------
/// Brief: retrieves an existing sound object
soundObject AudioEngine::GetSoundObject(std::string name, SoundType type, float volume)
{
  FMODImplementation::SoundMap::iterator it;

  it = fmodImp->sounds.find(name); // searches soundmap for object

  if (it == fmodImp->sounds.end())
    return CreateSoundObject(name, type, volume);

  return it->second; // returns pointer to sound object
}

///-------------------------------------
/// Brief: loads sound into fmod system
void AudioEngine::LoadSound(soundObject object, bool loop)
{
  if (object->soundType == stream_) // sound type is stream, auto loops
  {
    result = fmodImp->system->createStream(object->filename.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &(object->sound));
    ERRCHECK(result, "LoadSound");
  }

  else // sound type is effect
  {
    if (!loop)
      result = fmodImp->system->createSound(object->filename.c_str(), FMOD_LOOP_OFF | FMOD_2D, nullptr, &(object->sound));
    else
      result = fmodImp->system->createSound(object->filename.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &(object->sound));
    ERRCHECK(result, "LoadSound");
  }
}

///-------------------------------------
/// Brief: unloads a sound
void AudioEngine::ReleaseSound(soundObject object)
{
  if (object->sound)
  {
    result = object->sound->release();
    ERRCHECK(result, "Release Sound");
    object->sound = nullptr;
    object->channel = nullptr;
  }
}

///-------------------------------------
/// Brief: releases sound based on ID
void AudioEngine::ReleaseSound(int channelID)
{
  soundObject sound = nullptr;
  FMODImplementation::SoundMap::iterator it;
  FMODImplementation::SoundMap::iterator itEnd = fmodImp->sounds.end();

  for (it = fmodImp->sounds.begin(); it != itEnd; ++it)
  {
    if (it->second->channelID == channelID)
      ReleaseSound(sound);
  }
}


///-------------------------------------
/// Brief: Plays sound
int AudioEngine::Play(soundObject obj, bool loop)
{
  if (!obj)
    return -6; // object doesn't exist

  if (!obj->sound) // sound not loaded
    LoadSound(obj, loop);

  // new channel
  fmodImp->currentChannelID++;
  
  // plays sound on next available channel
  result = fmodImp->system->playSound(obj->sound, nullptr, true, &obj->channel);
  ERRCHECK(result, "PlaySound");

  if (!obj->channel)
  {
    TraceMessage(TraceLog, "Audio Engine: PlaySounds - channel not allocated");
    return -6;
  }

  std::pair<FMOD::Channel*, float> cPair(obj->channel, obj->volume);
  FMODImplementation::channelObject channelObj(obj->soundType, cPair);

  // gets correct volume for sound type
  float volume = 0.0;
  if (obj->soundType == sfx_)
    volume = masterVolume_ * sfxVolume_;
  else
    volume = masterVolume_ * streamVolume_;

  // sets if all muted
  result = obj->channel->setMute(allMuted_);
  ERRCHECK(result, "PlaySound");

  // sets channel volume and unpauses
  result = obj->channel->setVolume(volume * obj->volume);
  ERRCHECK(result, "PlaySound");

  result = obj->channel->setPaused(false);
  ERRCHECK(result, "PlaySound");

  // new channel yay
  fmodImp->channels[fmodImp->currentChannelID] = channelObj;

  return fmodImp->currentChannelID; // new channel ID
}

///-------------------------------------
/// Brief: Plays sound
int AudioEngine::Play(std::string filename, SoundType type, bool loop)
{
  soundObject obj = GetSoundObject(filename, type);

  if (!obj)
    obj = CreateSoundObject(filename, type);

  if (IsPlaying(obj->channelID))
    return obj->channelID;

  return Play(obj, loop);
}

///-------------------------------------
/// Brief: pauses/unpauses ALL channels (all sfx and streams)
void AudioEngine::Pause(bool pause)
{
  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    result = it->second.second.first->setPaused(pause);
    ERRCHECK(result, "Pause");
  }
}

///-------------------------------------
/// Brief: pauses/unpauses ALL channels of certain type
void AudioEngine::Pause(SoundType type, bool pause)
{
  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    if (it->second.first == type)
      result = it->second.second.first->setPaused(pause);
    ERRCHECK(result, "Pause");
  }
}


///-------------------------------------
/// Brief: Pauses single sound (most recent channel only)
void AudioEngine::Pause(soundObject object, bool pause)
{
  object->channel->setPaused(pause);
}

///-------------------------------------
/// Brief: checks if sound object paused
bool AudioEngine::isPaused(soundObject object)
{
  return isPaused(object->channel);
}

///-------------------------------------
/// Brief: checks if channel paused
bool AudioEngine::isPaused(FMOD::Channel * channel)
{
  bool paused = false;
  channel->getPaused(&paused);
  return paused;
}

///-------------------------------------
/// Brief: pauses all active sounds & returns channel IDs of paused streams
std::vector<soundObject> AudioEngine::Pause()
{
  std::vector<soundObject> temp;

  FMODImplementation::SoundMap::iterator it;
  FMODImplementation::SoundMap::iterator itEnd = fmodImp->sounds.end();

  for (it = fmodImp->sounds.begin(); it != itEnd; it++)
  {
    if (IsPlaying(it->second->channel) && !isPaused(it->second->channel))
    {
      Pause(it->second, true);
      temp.push_back(it->second);
    }
  }

  return temp;
}

///-------------------------------------
/// Brief: mutes/unmutes all channels
void AudioEngine::Mute(bool mute)
{
  allMuted_ = mute;

  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    result = it->second.second.first->setMute(mute);
    ERRCHECK(result, "mute");
  }
}

///-------------------------------------
/// Brief: mutes all channels of given type
void AudioEngine::Mute(SoundType type, bool mute)
{
  if (type == all_)
    Mute(mute);

  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    if (it->second.first == type)
    {
      result = it->second.second.first->setMute(mute);
      ERRCHECK(result, "mute");
    }
  }
}

///-------------------------------------
/// Brief: mutes single sound (most recent channel only)
void AudioEngine::Mute(soundObject object, bool mute)
{
  object->channel->setMute(mute);
}

///-------------------------------------
/// Brief: stops all channels
void AudioEngine::Stop()
{
  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    result = it->second.second.first->stop();
    ERRCHECK(result, "Stop");

    if (it->second.first == stream_)
      ReleaseSound(it->first);
  }
}

///-------------------------------------
/// Brief: stops all channels of given type
void AudioEngine::Stop(SoundType type)
{
  if (type == all_)
    Stop();

  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    if (it->second.first == type)
    {
      result = it->second.second.first->stop();
      ERRCHECK(result, "Stop");

      if (type == stream_)
        ReleaseSound(it->first);
    }
  }
}

///-------------------------------------
/// Brief: stops single channel (only latest channel)
void AudioEngine::Stop(soundObject object)
{
  object->channel->stop();
  
  if (object->soundType == stream_)
  {
    ReleaseSound(object);
  }
}

///-------------------------------------
/// Brief: changes master volume
void AudioEngine::Volume(float volume)
{
  if (volume < 0.0f || volume > 1.0f)
    return;

  masterVolume_ = volume;

  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    if (it->second.first == sfx_)
      result = it->second.second.first->setVolume(masterVolume_ * sfxVolume_ * it->second.second.second);
    else
      result = it->second.second.first->setVolume(masterVolume_ * streamVolume_ * it->second.second.second);
    
    ERRCHECK(result, "volume");
  }
}

///-------------------------------------
/// Brief: changes volume of sfx or stream
void AudioEngine::Volume(SoundType type, float volume)
{
  if (volume < 0.0f || volume > 1.0f)
    return;  
  
  if (type == all_)
    Volume(volume);
  else if (type == sfx_)
    sfxVolume_ = volume;
  else if (sfx_ == stream_)
    streamVolume_ = volume;

  // updates channels
  FMODImplementation::ChannelMap::iterator it;
  FMODImplementation::ChannelMap::iterator itEnd = fmodImp->channels.end();

  for (it = fmodImp->channels.begin(); it != itEnd; it++)
  {
    if (it->second.first == type)
    {
      result = it->second.second.first->setVolume(masterVolume_ * volume * it->second.second.second);
      ERRCHECK(result, "volume");
    }
  }
}

///-------------------------------------
/// Brief: changes soundobject base volume (doesn't change until played next)
void AudioEngine::Volume(soundObject object, float volume)
{
  if (volume < 0.0f || volume > 1.0f)
    return;

  object->volume = volume;
}

///-------------------------------------
/// Brief: stops audio channel (makes available for reuse)
void AudioEngine::Stop(int channelID)
{
  FMODImplementation::ChannelMap::iterator it = fmodImp->channels.find(channelID);

  if (it == fmodImp->channels.end())
    return;

  result = it->second.second.first->stop();
  ERRCHECK(result, "StopChannel");
}


///-------------------------------------
/// Brief: sets channel volume
void AudioEngine::SetChannelVolume(int channelID, float volume)
{
  FMODImplementation::ChannelMap::iterator it = fmodImp->channels.find(channelID);

  if (it == fmodImp->channels.end())
    return;

  result = it->second.second.first->setVolume(volume);
  ERRCHECK(result, "SetChannelVolume");
}

FMOD::Channel * AudioEngine::Channel(soundObject object)
{
  return object->channel;
}

FMOD::Channel * AudioEngine::Channel(std::string filename)
{
  soundObject obj = GetSoundObject(filename);

  if (!obj)
    return nullptr;

  return obj->channel;
}

///-------------------------------------
/// Brief: returns if channel is playing
bool AudioEngine::IsPlaying(int channelID)
{
  FMODImplementation::ChannelMap::iterator it = fmodImp->channels.find(channelID);

  if (it == fmodImp->channels.end())
    return false;

  return IsPlaying(it->second.second.first);
}

///-------------------------------------
/// Brief: returns if channel is playing
bool AudioEngine::IsPlaying(FMOD::Channel* channel)
{
  if (!channel)
    return false;

  bool playing = false;

  result = channel->isPlaying(&playing);
  ERRCHECK(result, "IsPlaying");

  return playing;
}

///-------------------------------------
/// Brief: returns if object is playing
bool AudioEngine::IsPlaying(soundObject object)
{
  if (!object)
    return false;

  return IsPlaying(object->channel);
}
