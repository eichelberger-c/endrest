///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ParticleEmitter.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - ParticleEmitter
//      - ParticleEmitter (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "Sprite.h"
#include "Timer.h"
///----------------------------------GLOBALS------------------------------------

typedef enum EmitterType
{
  Point_,
  Line_,
  Circle_,
} EmitterType;

typedef enum EmitterBehavior
{
  Breath_,
  Flying_,
  Stationary_,
  Trailing_,
  Random_,
} EmitterBehavior;
///----------------------------------CLASSES------------------------------------

class ParticleEmitter : public Component
{
public:
  ParticleEmitter(unsigned maxParticles = 1, secs time = 1, int radius = 1, float size = 1, 
                  glm::vec4 color = glm::vec4(1,1,1,1), EmitterType eType = Line_, EmitterBehavior ebehavior = Flying_);
  ParticleEmitter(const ParticleEmitter & temp);
  ParticleEmitter(const ParticleEmitter * temp);
  ParticleEmitter(const Component* ogComponent);
  ~ParticleEmitter();
  virtual Component * Clone() const;


  virtual void Update(float dt);
  void FlyingUpdate(float dt);
  void RandomUpdate(float dt);
  void BreathUpdate(float dt);
  void StationaryUpdate(float dt);
  void TrailingUpdate(float dt);

  inline void Type(EmitterType eType) { eType_ = eType; }
  inline EmitterType Type() const { return eType_; }

  inline void Behavior(EmitterBehavior eBehavior) { eBehavior_ = eBehavior; }
  inline EmitterBehavior Behavior() const { return eBehavior_; }

  inline void Radius(int radius) { radius_ = radius; }
  inline int Radius() const { return radius_; }

  void MaxParticles(unsigned maxParticles);
  inline unsigned MaxParticles() const { return maxParticles_; }

  void Time(secs time);
  inline secs Time() const { return time_; }

  void Size(float size);
  inline float Size() const { return size_; }

  inline glm::vec4 Color() { return color_; }
  void Color(glm::vec4 color);
  void Color(float r, float g, float b, float a);

  inline Vec2D Offset() { return offset_; }
  inline void Offset(Vec2D offset) { offset_ = offset; }
  inline void Offset(glm::vec2 offset) { offset_.X(offset.x); offset_.Y(offset.y); }

  inline std::vector<std::pair<Obj*, bool>> Particles() const {return particles_; }

  void SetParticlesVisible(bool vis); //Added by Sonya for events w/particles

private:
  EmitterType eType_;
  EmitterBehavior eBehavior_;
  int radius_;
  int speed_ = 20;
  unsigned maxParticles_; 
  secs time_;
  float size_;
  glm::vec4 color_;
  bool init_= true;
  Vec2D offset_ = Vec2D(0, 0);

  std::vector<std::pair<Obj*, bool>> particles_;
};

///---------------------------------FUNCTIONS-----------------------------------


