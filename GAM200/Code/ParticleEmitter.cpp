///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ParticleEmitter.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - ParticleEmitter
//      - ParticleEmitter (copy constructor)
//      - ParticleEmitter (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//      - 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ParticleEmitter.h"
#include "Transform.h"
#include "Obj.h"
#include "Physics.h"
#include "Utils.h"
#include "Spaces.h"
///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------


ParticleEmitter::ParticleEmitter(unsigned maxParticles, secs time, int radius, float size, glm::vec4 color,
EmitterType eType, EmitterBehavior eBehavior) : Component(ParticleEmitter_),
maxParticles_(maxParticles), time_(time), radius_(radius), size_(size), 
color_(color), eType_(eType), eBehavior_(eBehavior)
{
  for (unsigned i = 0; i < maxParticles_; ++i)
  {
    Obj* obj = new Obj(std::string("Particle" + i));
    Sprite* sprite = new Sprite("Data/Textures/particle.png");
    sprite->SetRGBA(color);
    obj->Add(sprite);
    obj->Add(new Timer(time_, CountUp_, time_));
    obj->Add(new Physics());
    Transform* trans = new Transform(0,0);
    trans->Scale(size, size);
    obj->Add(trans);
    std::pair<Obj*, bool> temp(obj, true);
    particles_.push_back(temp);
  }
}

ParticleEmitter::ParticleEmitter(const Component * ogComponent) : Component(ParticleEmitter_)
{
  const ParticleEmitter* temp = dynamic_cast<const ParticleEmitter*>(ogComponent);
  maxParticles_ = temp->maxParticles_;
  radius_ = temp->radius_;
  eType_ = temp->eType_;
  time_ = temp->time_;
  size_ = temp->size_;
  eBehavior_ = temp->eBehavior_;
  color_ = temp->color_;
  IsActive(true);
  init_ = true;
  int size = temp->particles_.size();
  for (int i = 0; i < size; ++i)
  {
    std::pair<Obj*, bool> pair(new Obj(*temp->particles_[i].first, temp->particles_[i].first->Name()), true);
    particles_.push_back(pair);
  }
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter & temp) : Component(ParticleEmitter_)
{
  maxParticles_ = temp.maxParticles_;
  radius_ = temp.radius_;
  eType_ = temp.eType_;
  eBehavior_ = temp.eBehavior_;
  time_ = temp.time_;
  size_ = temp.size_;
  color_ = temp.color_;
  IsActive(true);
  init_ = true;
  int size = temp.particles_.size();
  for (int i = 0; i < size; ++i)
  {
    std::pair<Obj*, bool> pair(new Obj(*temp.particles_[i].first, "Particle"), true);
    particles_.push_back(pair);
  }
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter * temp) : Component(ParticleEmitter_)
{
  maxParticles_ = temp->maxParticles_;
  radius_ = temp->radius_;
  eType_ = temp->eType_;
  eBehavior_ = temp->eBehavior_;
  time_ = temp->time_;
  size_ = temp->size_;
  color_ = temp->color_;
  IsActive(true);
  init_ = true;
  int size = temp->particles_.size();
  for (int i = 0; i < size; ++i)
  {
    std::pair<Obj*, bool> pair(new Obj(*temp->particles_[i].first, "Particle"), true);
    particles_.push_back(pair);
  }
}

Component * ParticleEmitter::Clone() const
{
  return new ParticleEmitter(*this);
}

ParticleEmitter::~ParticleEmitter()
{
  int size = particles_.size();
  for (int i = 0; i < size; ++i)
  {
    delete particles_[i].first;
  }
}

void ParticleEmitter::Update(float dt)
{
  if (init_)
  {
    for (auto elem : particles_)
    {
      Parent()->AddSubObj(elem.first);
      if (eBehavior_ == Random_)
        elem.first->Has(Transform)->Translation(float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))),
          float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))));
      else
        elem.first->Has(Transform)->Translation(Parent()->Has(Transform)->Translation());
    }
    init_ = false;
  }

  switch (eBehavior_)
  {
    case Flying_:
    {
      FlyingUpdate(dt);
      break;
    }
    case Random_:
    {
      RandomUpdate(dt);
      break;
    }
    case Breath_:
    {
      BreathUpdate(dt);
      break;
    }
    case Stationary_:
    {
      StationaryUpdate(dt);
      break;
    }
    case Trailing_:
    {
      TrailingUpdate(dt);
      break;
    }
  }      
}

void ParticleEmitter::FlyingUpdate(float dt)
{
  Transform* trans = Parent()->Has(Transform);
  for (unsigned i = 0; i < maxParticles_; ++i)
  {
    Timer* timer = particles_[i].first->Has(Timer);
    Transform* partTrans = particles_[i].first->Has(Transform);
    Physics* partPhys = particles_[i].first->Has(Physics);
    if (timer->Beep())
    {
      Vec2D loc = offset_ + trans->Translation();
      switch (eType_)
      {
        case Line_:
        {
          loc += Vec2D(float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))), 0);
          break;
        }
        case Circle_:
        {
          int flip = rand() % 2 + 1;
          int randomY = 1;

          if (flip != 1)
            randomY = -1;

          float x = float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1)));
          float y = sqrt((radius_ * radius_ - x * x)) * randomY;

          loc += Vec2D(x, y);
          break;
        }
      }
      partTrans->Translation(loc);
      partPhys->Velocity(0, 0);
      particles_[i].second = true;
      timer->Reset();
    }
    else
    {
      if (particles_[i].second)
      {
        Vec2D vel;
        switch (eType_)
        {
          case Line_:
          {
            vel = Vec2D(0, float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))));
            break;
          }
          case Point_:
          {
            vel = Vec2D(float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))),
              float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))));
            break;
          }
          case Circle_:
          {
            vel = Vec2D(0, float(0 + (rand() * 10) % (radius_ - (0 - 1))));
            break;
          }
        }
        partPhys->Velocity(vel);
        particles_[i].second = false;
      }
    }
    particles_[i].first->Update(dt);
  }
}

void ParticleEmitter::BreathUpdate(float dt)
{
  Transform* trans = Parent()->Has(Transform);
  for (unsigned i = 0; i < maxParticles_; ++i)
  {
    Timer* timer = particles_[i].first->Has(Timer);
    Transform* partTrans = particles_[i].first->Has(Transform);
    Physics* partPhys = particles_[i].first->Has(Physics);
    if (timer->Beep())
    {
      Vec2D vel = partPhys->Velocity();
      vel *= -1;
      partPhys->Velocity(vel);
      particles_[i].second = false;
      timer->Reset();
    }
    else
    {
      if (particles_[i].second)
      {
        Vec2D vel;
        switch (eType_)
        {
        case Line_:
        {
          vel = Vec2D(0, float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))));
          break;
        }
        case Point_:
        {
          vel = Vec2D(float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))),
            float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))));
          break;
        }
        case Circle_:
        {
          vel = Vec2D(0, float(0 + (rand() * 10) % (radius_ - (0 - 1))));
          break;
        }
        }
        partPhys->Velocity(vel);
        particles_[i].second = false;
      }
    }
    particles_[i].first->Update(dt);
  }
}

void ParticleEmitter::StationaryUpdate(float dt)
{
  Transform* trans = Parent()->Has(Transform);
  for (unsigned i = 0; i < maxParticles_; ++i)
  {
    Timer* timer = particles_[i].first->Has(Timer);
    Transform* partTrans = particles_[i].first->Has(Transform);
    Physics* partPhys = particles_[i].first->Has(Physics);
    if (timer->Beep())
    {
      Vec2D loc = offset_ + trans->Translation();
      switch (eType_)
      {
        case Line_:
        {
          loc += Vec2D(float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))), 0);
          break;
        }
        case Circle_:
        {
          int flip = rand() % 2 + 1;
          int randomY = 1;

          if (flip != 1)
            randomY = -1;

          float x = float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1)));
          float y = sqrt((radius_ * radius_ - x * x)) * randomY;

          loc += Vec2D(x, y);
          break;
        }
        case Point_:
        {
          loc += Vec2D(float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))), 
                       float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))));
          break;
        }
      }
      partTrans->Translation(loc);
      partPhys->Velocity(0, 0);
      particles_[i].second = true;
      timer->Reset();
    }
    particles_[i].first->Update(dt);
  }
}

void ParticleEmitter::TrailingUpdate(float dt)
{
  static unsigned i = 0;
  if (i >= particles_.size())
    i = 0;
  Transform* trans = Parent()->Has(Transform);
  Physics *phys = Parent()->Has(Physics);

  Timer* timer = particles_[i].first->Has(Timer);
  if (!timer->Beep())
    return;
  else
    timer->Reset();

  Player *player = &Space::GetPlayer();
  Vec2D playerPos;
  Vec2D diff;

  float x = phys->Velocity().X();
  float y = phys->Velocity().Y();
  float rad = (float)radius_;

  Transform* partTrans = particles_[i].first->Has(Transform);
  Physics* partPhys = particles_[i].first->Has(Physics);
  Vec2D loc = offset_ + trans->Translation()/* + RandomVec2D(-rad, rad)*/;

  switch (eType_)
  {
  case Point_:
    playerPos = player->Has(Transform)->Translation();
    diff = GetFollow(trans->Translation(), playerPos, x + rad * 1.5f);
    x = diff.X();
    y = diff.Y();
    rad *= 0.1f;
    x += RandomFloat(-rad, rad);
    y += RandomFloat(-rad, rad);
    x = -x;
    y = -y;
    break;
  case Circle_:
    loc += RandomVec2D(-rad, rad);
    break;
  case Line_:
    //loc.Y(loc.Y() + RandomFloat(0.0f, rad));
    //loc += RandomVec2D2((-rad * 0.8f), (rad * 0.8f), 0.0f, rad);
    x += RandomFloat((-rad * 0.2f), (rad * 0.2f));
    loc.X(loc.X() + RandomFloat((-rad * 0.4f), (rad * 0.4f)));
    //y += RandomFloat(0.0f, rad);
    y += rad / 2.0f;
    y += (rad * RandomFloat(-0.8f, 0.3f));
    y = -y;
    break;
  }

  partTrans->Translation(loc);
  partPhys->Velocity(-x, -y);
  particles_[i].first->Update(dt);
  i++;

}

void ParticleEmitter::RandomUpdate(float dt)
{
  Transform* trans = Parent()->Has(Transform);
  for (unsigned i = 0; i < maxParticles_; ++i)
  {
    Timer* timer = particles_[i].first->Has(Timer);
    Transform* partTrans = particles_[i].first->Has(Transform);
    Physics* partPhys = particles_[i].first->Has(Physics);
    if (timer->Beep())
    {
      Vec2D vel = partPhys->Velocity();
      switch (eType_)
      {
        case Line_:
        {
          vel *= -1;
          break;
        }
        case Circle_:
        {
          vel *= -1;
          break;
        }
      }
	  if (partTrans->Translation().X() > radius_ || partTrans->Translation().Y() > radius_ || partTrans->Translation().X() < -radius_ || partTrans->Translation().Y() < -radius_)
		  partTrans->Translation(trans->Translation());
      partPhys->Velocity(vel);
      particles_[i].second = true;
      timer->Reset();
    }
    else
    {
      if (particles_[i].second)
      {
        Vec2D vel;
        switch (eType_)
        {
        case Line_:
        {
          vel = Vec2D(0, float(-radius_ + (rand() * 10) % (radius_ - (-radius_ - 1))));
          break;
        }
        case Point_:
        {
          vel = Vec2D(float(-speed_ + (rand() * 10) % (speed_ - (-speed_ - 1))),
            float(-speed_ + (rand() * 10) % (speed_ - (-speed_ - 1))));
          break;
        }
        case Circle_:
        {
          vel = Vec2D(0, float(0 + (rand() * 10) % (radius_ - (0 - 1))));
          break;
        }
        }
        partPhys->Velocity(vel);
        particles_[i].second = false;
      }
    }
    particles_[i].first->Update(dt);
  }
}

void ParticleEmitter::MaxParticles(unsigned maxParticles)
{
  while (particles_.size() < maxParticles)
  {
    Obj* obj = new Obj(std::string("Particle"));
    Sprite* sprite = new Sprite("Data/Textures/particle.png");
    sprite->SetRGBA(color_);
    obj->Add(sprite);
    obj->Add(new Timer(time_, CountUp_, time_));
    obj->Add(new Physics());
    Transform* trans = new Transform(0, 0);
    trans->Scale(size_, size_);
    obj->Add(trans);
    std::pair<Obj*, bool> temp(obj, true);
    particles_.push_back(temp);
    Parent()->AddSubObj(obj);
  }
  maxParticles_ = maxParticles;
}

void ParticleEmitter::Time(secs time)
{
  for (auto elem : particles_) 
  {
    Timer* timer = elem.first->Has(Timer);
    timer->MaxTime(time); 
    timer->ResetVal(time);
  }
  time_ = time;
}

void ParticleEmitter::Size(float size)
{
  for (auto elem : particles_)
  {
    Transform* trans = elem.first->Has(Transform);
    trans->Scale(size, size);
  }
  size_ = size;
}

void ParticleEmitter::Color(glm::vec4 color)
{
  for (auto elem : particles_)
  {
    Sprite* sprite = elem.first->Has(Sprite);
    sprite->SetRGBA(color);
  }
  color_ = color;
}

void ParticleEmitter::Color(float r, float g, float b, float a)
{
  Color(glm::vec4(r, g, b, a));
}

void ParticleEmitter::SetParticlesVisible(bool vis)
{
  for (auto part : particles_)
    part.first->Has(Sprite)->SetVisible(vis);
}
