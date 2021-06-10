///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Input.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -Input() : System("InputSys") {};
//		-~Input()
//		-virtual void Initialize()
//		-virtual void Update(float dt)
//		-void UpdateMouse(GLFWwindow *window)
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------

#define _USE_MATH_DEFINES
#include <cmath>
#include "stdafx.h"
#include "Spaces.h"
#include "Engine.h"
#include "ObjManager.h"
#include "Graphics.h"
#include "Input.h"
#include "Player.h"
#include "Lantern.h"
#include "ColliderSys.h"
#include "PushBlockManager.h"
#include "PushBlock.h"
#include "Area.h"
#include "ScreenManager.h"

///----------------------------------GLOBALS-----------------------------------
static Graphics* graphicSys;
static ObjManager objMan;
static ColliderSys *Collision;
static ScreenManager *screenManager;
static Player *playerPointer;
static AudioEngine *aEngine;
static soundObject shot;
static soundObject notReady;


///---------------------------------FUNCTIONS-----------------------------------


Input::Input() : System("Input")
{
  isActive_ = true;
}

///-------------------------------------
/// Brief: deconstructor
Input::~Input()
{
}

///-------------------------------------
/// Brief: input initialize
void Input::Initialize()
{
  graphicSys = dynamic_cast<Graphics*>(EngineGetSystem("Graphics"));          // Get graphics system
  Collision = reinterpret_cast<ColliderSys*>(EngineGetSystem("ColliderSys")); // get collision system
  screenManager = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
  aEngine = reinterpret_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));
  shot = aEngine->GetSoundObject("Data/Audio/Shot1.wav");
  notReady = aEngine->GetSoundObject("Data/Audio/LanternNotReady.wav");
  playerPointer = &Space::GetPlayer();
}


///-------------------------------------
/// Brief: This function determines in a button is 
/// being pressed and responds accorcdingly. this
/// function may also need to tell the graphics engine 
/// where to render the lantern reticle.
void Input::Update(float dt)
{
  if (!ScreensActive())
    return;

  --timer_;

  if (timer_ <= 0)
  {
    isReady_ = true;
  }
  else
  {
    isReady_ = false;
  }
   CheckAttack(dt);



  GLFWwindow *window = graphicSys->Window(); //get current window
  const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int width = 1920;
  int height = 1080;
  UpdateMouse(window); //This should always be done; update mouse for lantern

  //Get Components
  ColliderCircle *collider = dynamic_cast<ColliderCircle*>(playerPointer->Has(Collider)); //Get parent objects collider component
  Physics *physics = playerPointer->Has(Physics);             //Get parent objects physics component
  Transform *transform = playerPointer->Has(Transform);           //Get parent transform component

  if (!collider || !physics || !transform)
  {
    //TraceMessage(false, "Input Update - nullptr");
    return;
  }

  //Get Vector to adjust velocity
  glm::vec2 Velocity = { 0.0f, 0.0f };
  Vec2D Translation = transform->Translation();
  Vec2D vel = physics->Velocity();

  if ((collider->Frames()) > 0) //If the collision frames are not cycled, don't take input
    return;

  //Orthogonal movement////////////////////////////////////////////////

  // NORTH
  if ((glfwGetKey(window, upMovement_) == GLFW_PRESS))/* || (contrDirection == StickUp_)*/
  {
    //TraceMessage(false, "Up key pressed");
    if (Translation.Y() < height / 3.0)
    {
      //Set velocity in Y direction to be positive
      Velocity.y = 1;
    }
    else
    {
      screenManager->BorderCheck(dt);
    }

  }

  // SOUTH
  else if ((glfwGetKey(window, downMovement_) == GLFW_PRESS))/* || (contrDirection == StickDown_)*/
  {
    //TraceMessage(false, "Down key pressed");
    //Set velocity in Y direction to be negative
    if (Translation.Y() > -height / 2.2)
    {
      Velocity.y = -1;
    }
    else
    {
      screenManager->BorderCheck(dt);
    }

  }

  else //(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
  {
    Velocity.y = 0.0f;
  }

  ////////////////////////////////////////////////////////////////////////////////////////

  // WEST
  if ((glfwGetKey(window, leftMovement_) == GLFW_PRESS)/* || (contrDirection == StickLeft_)*/)
  {
    //TraceMessage(false, "Left key pressed");
    //Set velocity in X direction to be negative
    if (Translation.X() > -width / 2.1)
    {
      Velocity.x = -1;
    }
    else
    {
      screenManager->BorderCheck(dt);
    }

  }

  // EAST
  else if ((glfwGetKey(window, rightMovement_) == GLFW_PRESS) /* || (contrDirection == StickRight_)*/)
  {
    //TraceMessage(false, "Right key pressed");
    //Set velocity in the X direction to be positive
    if (Translation.X() < width / 2.1)
    {
      Velocity.x = 1;
    }
    else
    {
      screenManager->BorderCheck(dt);
    }
    
  }
  else
  {
    Velocity.x = 0.0f;
  }
  if (Velocity.x != 0 && Velocity.y != 0)
    Velocity = glm::normalize(Velocity);

  Velocity.x *= playerPointer->Speed();
  Velocity.y *= playerPointer->Speed();
  physics->Velocity(Velocity);
}

///-------------------------------------
/// Brief: Calculates and returns mouse position
///	as a vector.
void Input::UpdateMouse(GLFWwindow * window)
{
  if (!isActive_)
    return;

  //Object pointer for any object mouse may be on
  Obj *object = nullptr;

  //Get Player Transform and Translation
  Transform *transform = playerPointer->Has(Transform);

  //Get Lantern's transform
  Transform *lanternTrans = Space::GetTransform("Lantern");
  Vec2D oldPosition = lanternTrans->Translation();
  Lantern *lantern = static_cast<Lantern*>((&Space::GetPlayer())->GetSubObject("Lantern"));

  //Temporary vectors to hold values
  glm::vec2 normalVec;
  glm::vec2 direction;
  glm::vec2 mouseVec;
  glm::vec2 oldPos;
  glm::vec3 tempMouseVec;
  glm::vec3 newDirection;

  //Mouse position and world position are private members of this class
  GLdouble cursorX = 0.0;
  GLdouble cursorY = 0.0;

  //Get cursor position and translate it to world coordinates
  glfwGetCursorPos(window, &cursorX, &cursorY);

  ////////////////////////////////////////////////////////////////
  //Convert mouse coordinates from screen to world
  mouseVec.x = (float)cursorX;
  mouseVec.y = (float)cursorY;
  mouseVec = Graphics::ScreenToWorld(mouseVec);
  //TraceMessage(TraceLog, "X:%f, Y:%f", mouseVec.x, mouseVec.y);
  //NEW CODE-------------------------------------------------------
  tempMouseVec.x = mouseVec.x;
  tempMouseVec.y = mouseVec.y;
  tempMouseVec.z = 0.0f;

  int zone = GetDirection(tempMouseVec);

  //CheckPuzzleInput(zone, window);
  //---------------------------------------------------------------

  ////////////////////////////////////////////////////////////////
  //Convert lantern coordinates from screen to world
  //Convert old position to glm vector
  oldPos.x = oldPosition.X();
  oldPos.y = oldPosition.Y();
  glm::vec2 size = transform->glmScale();
  ////////////////////////////////////////////////////////////////
  //oldPos.x -= size.x / 2;
  //oldPos.y -= size.y / 2;

  //Translate world position into vector, normalize it, and rotate it
  direction = mouseVec - oldPos;

  normalVec = glm::normalize(direction);
  newDirection.x = normalVec.x;
  newDirection.y = normalVec.y;
  newDirection.z = 1.0f;

  //update lantern's transform rotation to be facing cursor position
  lantern->Direction(newDirection);

  Vec2D velocity = playerPointer->Has(Physics)->Velocity();
  double angle = 0.0f;

  //if (controller.CheckConnection())
  //{
  //  angle = controller.GetRightAngle();
  //}
  //else
  //{
  angle = glm::atan(normalVec.y, normalVec.x);
  //}
  if (mouseVec.x > transform->glmTranslation().x)
  {
    transform->Scale(-144, transform->glmScale().y);
    lanternTrans->Scale(144, 144);
    if (angle > M_PI / 2.000001)
    {
      angle = M_PI / 2.000001;
    }
    if (angle < -M_PI / 2.000001)
    {
      angle = -M_PI / 2.000001;
    }
    lanternTrans->Rotation((float)angle);    glm::vec2 offset(transform->glmTranslation().x - transform->glmScale().x / 1.3, transform->glmTranslation().y);
    lanternTrans->Translation(offset); //Set translation to be player's
    lantern->Length(90);
  }
  else
  {
    transform->Scale(144, transform->glmScale().y);
    lanternTrans->Scale(144, 144);
    if (angle < M_PI_2 && angle > 0)
    {
      angle = M_PI_2;
    }
    if (angle > -M_PI_2 && angle < 0)
    {
      angle = -M_PI_2;
    }
    lanternTrans->Rotation((float)angle);
    glm::vec2 offset(transform->glmTranslation().x - transform->glmScale().x / 1.3, transform->glmTranslation().y);
    lanternTrans->Translation(offset); //Set translation to be player's
    lantern->Length(95);
  }
}

///-------------------------------------
/// Brief: 
Obj * Input::CheckMouse(glm::vec2 mousePos)
{
  // cj's changes by megan
  ObjectList objectList = screenManager->CurrentObjList();
  Transform *transform = nullptr;
  Vec2D position, scale;
  float mouseX = mousePos.x, mouseY = mousePos.y;
  float objX = 0.0f, objY = 0.0f;
  float scaleX = 0.0f, scaleY = 0.0f;
 
  for (Obj *obj : objectList)
  {
    if (obj->Name().find("Background") != -1)
      continue;

    transform = obj->Has(Transform);
    position = transform->Translation();
    scale = transform->Scale();
    objX = position.X();
    objY = position.Y();
    scaleX = scale.X() / 2.0f;
    scaleY = scale.Y() / 2.0f;
    
    if (mouseX < objX + scaleX && mouseX > objX - scaleX && mouseY < objY + scaleY && mouseY > objY - scaleY)
      return obj;
  }

  if (playerPointer)
  {
    transform = playerPointer->Has(Transform);
    position = transform->Translation();
    scale = transform->Scale();
    objX = position.X();
    objY = position.Y();
    scaleX = scale.X() / 2.0f;
    scaleY = scale.Y() / 2.0f;

    if (mouseX < objX + scaleX && mouseX > objX - scaleX && mouseY < objY + scaleY && mouseY > objY - scaleY)
      return playerPointer;
  }

  

  return nullptr;
}

///-------------------------------------
/// Brief: 
void Input::IsActive(bool toggle)
{
  isActive_ = toggle;
}

///-------------------------------------
/// Brief: 
bool Input::IsActive()
{
  return isActive_;
}

///-------------------------------------
/// Brief: 
int Input::GetDirection(glm::vec3 mouseVec)
{
  //Get necessary components
  Transform *playerTrans = Space::GetTransform("Player");
  Vec2D position = playerTrans->Translation();
  glm::vec3 convertPosition;
  convertPosition.x = position.X();
  convertPosition.y = position.Y();
  convertPosition.z = 0.0f;
  glm::vec3 direction = convertPosition - mouseVec;

  //Vector for Z axis
  glm::vec3 Zaxis;
  Zaxis.x = 0;
  Zaxis.y = 0;
  Zaxis.z = 1;

  //Diagonal vectors for each quadrant
  glm::vec3 Quadrant1;
  Quadrant1.x = position.X() + 5.0f;
  Quadrant1.y = position.Y() + 5.0f;
  Quadrant1.z = 0.0f;
  glm::vec3 Quadrant2;
  Quadrant2.x = position.X() - 5.0f;
  Quadrant2.y = position.Y() + 5.0f;
  Quadrant2.z = 0.0f;
  glm::vec3 Quadrant3;
  Quadrant3.x = position.X() - 5.0f;
  Quadrant3.y = position.Y() - 5.0f;
  Quadrant3.z = 0.0f;
  glm::vec3 Quadrant4;
  Quadrant4.x = position.X() + 5.0f;
  Quadrant4.y = position.Y() - 5.0f;
  Quadrant4.z = 0.0f;

  //Angles between direction vector and quadrant vectors
  float theta = 0.0f;
  float alpha1 = 0.0f;
  float alpha2 = 0.0f;
  float alpha3 = 0.0f;

  //Check angels between vectors and assign zone of block relative to player/////////////////

  //Get angles between vectors //THESE VECTORS MAY BE GETTING PASSED IN THE WRONG ORDER
  theta = glm::orientedAngle(glm::normalize(Quadrant1), glm::normalize(direction), Zaxis);
  alpha1 = glm::orientedAngle(glm::normalize(Quadrant2), glm::normalize(direction), Zaxis);
  alpha2 = glm::orientedAngle(glm::normalize(Quadrant3), glm::normalize(direction), Zaxis);
  alpha3 = glm::orientedAngle(glm::normalize(Quadrant4), glm::normalize(direction), Zaxis);

  //Put pointer to object in puzzle classes array
  if (theta > 0)
  {
    if (alpha1 < 0)
    {
      //Block is above player
      return Top_;
    }
    else if (alpha2 < 0)
    {
      //Block is to the left of the player
      return Left_;
    }
    else if (alpha3 < 0)
    {
      //Block is below player
      return Bottom_;
    }
    else
    {
      //Block is to the right of player
      return Right_;
    }
  }
  else if (theta < 0)
  {
    //Block is to the right of player
    return Right_;
  }

  return Nope_;

  ////Put pointer to object in puzzle classes array
  //if (theta > 0 && alpha < 0)
  //{
  //  //Block is above player
  //  return Top_;
  //}

  ////Get angle between vectors
  //theta = glm::orientedAngle(glm::normalize(Quadrant2), glm::normalize(direction), Zaxis);
  //alpha = glm::orientedAngle(glm::normalize(Quadrant3), glm::normalize(direction), Zaxis);

  ////Put pointer to object in puzzle classes array
  //if (theta > 0 && alpha < 0)
  //{
  //  //Block is to the left of the player
  //  return Left_;
  //}

  ////Get angle between vectors
  //theta = glm::orientedAngle(glm::normalize(Quadrant3), glm::normalize(direction), Zaxis);
  //alpha = glm::orientedAngle(glm::normalize(Quadrant4), glm::normalize(direction), Zaxis);

  ////Put pointer to object in puzzle classes array
  //if (theta > 0 && alpha < 0)
  //{
  //  //Block is below player
  //  return Bottom_;
  //}

  ////Get angle between vectors
  //theta = glm::orientedAngle(glm::normalize(Quadrant4), glm::normalize(direction), Zaxis);
  //alpha = glm::orientedAngle(glm::normalize(Quadrant1), glm::normalize(direction), Zaxis);

  ////Put pointer to object in puzzle classes array
  //if (theta > 0 && alpha < 0)
  //{
  //  //Block is to the right of player
  //  return Right_;
  //}


  //Cannot determine direction
  //return Nope_;
}

///-------------------------------------
/// Brief: 
//void Input::CheckPuzzleInput(int zone, GLFWwindow * window)
//{
//  PushBlockManager *blockManager = static_cast<PushBlockManager*>(EngineGetSystem("PushBlockManager"));
//  //ScreenManager *screenManager = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
//  Area *currentArea = static_cast<Area*>(screenManager->CurrentScreen());
//
//  if (currentArea == nullptr)
//    return;
//
//  PushBlock *pushBlock = currentArea->GetPushBlockPuzzle();
//
//  if (pushBlock == nullptr)
//    return;
//
//  Obj *block = blockManager->CheckBlock(zone);
//  if (!block)
//    return;
//  ColliderCircle *collider = dynamic_cast<ColliderCircle*>(block->Has(Collider));
//  Physics *physics = block->Has(Physics);
//
//  if (block)
//  {
//    //Check for input
//    if ((Push() == GLFW_PRESS))
//    {
//      //Set velocity
//      switch (zone)
//      {
//      case Top_:
//        physics->Velocity(0.0f, 3.0f);
//        break;
//      case Left_:
//        physics->Velocity(-3.0f, 0.0f);
//        break;
//      case Bottom_:
//        physics->Velocity(0.0f, -3.0f);
//        break;
//      case Right_:
//        physics->Velocity(3.0f, 0.0f);
//        break;
//      }
//    }
//
//    //Set push frames
//    collider->SetPushFrames(15);
//  }
//}

void Input::CheckAttack(float dt)
{
  Lantern* lantern = static_cast<Lantern*>((&Space::GetPlayer())->GetSubObject("Lantern"));
  bool f = false;

  //if (flashFrames_ > 0)
  //{

  //  if (lantern.IsActive())
  //  {
  //    --flashFrames_;

  //    if (flashFrames_ <= 0)
  //    {
  //      flashFrames_ = 5;
  //      isReady_ = false;
  //      PeepHole* hole = reinterpret_cast<PeepHole*>(Space::GetPlayer().GetPeepHole());
  //      hole->ScaleUp();
  //      timer_ = 60;
  //      lantern.IsActive(f);
  //    }

  //    return;
  //  }
  //}
  PeepHole* hole = reinterpret_cast<PeepHole*>(Space::GetPlayer().GetPeepHole());

  if (attackReady_ && hole->HoleType() != minSize_)
  {
	  bool t = true;
    lantern->IsActive(t);
    hole->ScaleDown();
	  hole->UpdateSize();
	  hole->Update(dt);
	  flashFrames_ = 10;
	  attackReady_ = false;
    aEngine->Play(shot);
  }
  else if (attackReady_ && hole->HoleType() == minSize_)
  {
	  if (!aEngine->IsPlaying(notReady))
		  aEngine->Play(notReady);
	  attackReady_ = false;
  }
  else
  {
	  flashFrames_--;
	if (flashFrames_ <= 0)
		lantern->IsActive(f);
  }
}

