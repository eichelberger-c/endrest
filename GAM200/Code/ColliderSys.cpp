///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ColliderSys.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -ColliderSys() : System("Collider") {};
//		  -~ColliderSys()
//		  -virtual void Initialize()
//	  	-virtual void Update(float dt)
//	  	-Vec2D GetPosition(std::string name)
//	  	-void LightCollision(std::string player, std::string other)
//	  	-bool CircleCircleCollision(std::string firstName, std::string secondName, ColliderType type)
//	  	-bool CheckSquareCollision(std::string firstName, std::string secondName)
//		  -int GetZone(Vec2D obj1Pos, Vec2D obj2pos)
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ColliderSys.h"
#include "ColliderCircle.h"
#include "ColliderSquare.h"
#include "ObjManager.h"
#include "Spaces.h"
#include "Engine.h"
#include "Graphics.h"
#include "Area.h"
#include <glm/glm.hpp>
#include "Lantern.h"
#include "PushBlockManager.h"
#include "PushBlock.h"
#include "EventManager.h"
#include "ScreenManager.h"
#include "Trigger.h"

// mw - this thing has to be defined if you're going to use these last two includes
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/vector_angle.hpp>
//#include <glm/gtx/projection.hpp>




///----------------------------------GLOBALS-----------------------------------
static std::string firstName;	 //First objects name
static std::string secondName; //Second objects name
ObjectList objectList;         //Get all objects
CircleColliderList CircleList; //Get circle colliders
SpriteList sprites;
const float LanternScale = 210;
bool lightCheck = false;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Destructor
ColliderSys::~ColliderSys()
{
}

///-------------------------------------
/// Brief: Initializer
void ColliderSys::Initialize()
{

}

///-------------------------------------
/// Brief: Updates collider components found in ColliderList vector
void ColliderSys::Update(float dt)
{
  //TraceMessage(false, "Collider System Update");

  ScreenManager *screenManager = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
  objectList = screenManager->CurrentObjList();            //Get all objects
  Lantern *lantern = static_cast<Lantern*>((&Space::GetPlayer())->GetSubObject("Lantern"));

  unsigned size = objectList.size();
  for (unsigned i = 0; i < size; ++i)
  {
    for (unsigned j = i + 1; j < size; ++j)
    {
      Collider* collider1 = objectList[i]->Has(Collider);
      Collider* collider2 = objectList[j]->Has(Collider);

      ColliderCircle* circle1 = reinterpret_cast<ColliderCircle*>(collider1);
      ColliderCircle* circle2 = reinterpret_cast<ColliderCircle*>(collider2);

	    ColliderSquare* square1 = reinterpret_cast<ColliderSquare*>(collider1);
	    ColliderSquare* square2 = reinterpret_cast<ColliderSquare*>(collider2);

      if (circle1 && circle2)
      {
        if(circle1->IsActive() && circle2->IsActive())
          CircleCircleCollision(*circle1, *circle2);
      }
      else if (square1 && square2)
      {
        if (collider1->IsActive() && collider2->IsActive())
          SquareSquareCollision(*square1, *square2);
      }
      else if (circle1 && square2)
      {
		 CircleSquareCollision(*circle1, *square2);
      }
      else if(circle2 && square1)
      {
		  CircleSquareCollision(*circle2, *square1);
      }
    }
  }

  Collider* collider2 = Space::GetPlayer().Has(Collider);
  for (unsigned int i = 0; i < size; ++i)
  {
    if (lantern->IsActive())
    {
      lightCheck = true;
      LightCollision("Lantern", objectList[i]->Name());
    }
    Collider* collider1 = objectList[i]->Has(Collider);
	  ColliderCircle* circle = dynamic_cast<ColliderCircle*>(collider1);
	  ColliderSquare* square = dynamic_cast<ColliderSquare*>(collider1);


    if (collider1 && collider2)
    {
		if (collider1->IsActive() && collider2->IsActive())
		{
			if(circle)
				CircleCircleCollision(*reinterpret_cast<ColliderCircle*>(collider2), *circle);
			else if (square)
				CircleSquareCollision(*reinterpret_cast<ColliderCircle*>(collider2), *square);
		}
    }
  }
}


///-------------------------------------
/// Brief: Get's position of given object
Vec2D ColliderSys::GetPosition(std::string &name)
{
  //get object's transform
  Transform *transform = Space::GetTransform(name);

  //Create a Vec2D
  Vec2D position = transform->Translation();

  //return new Vec2D
  return position;
}

///-------------------------------------
/// Brief: First object relative to second object
Vec2D ColliderSys::GetPoint(std::string &firstName, std::string &secondName)
{
  Transform *firstTrans = Space::GetTransform(firstName);
  Transform *secondTrans = Space::GetTransform(secondName);
  ColliderSquare *square = Space::GetSquareCollider(secondName);
  Vec2D squareDim = square->Dimensions();
  Vec2D firstPos = firstTrans->Translation();
  Vec2D secondPos = secondTrans->Translation();
  Vec2D point = 0;

  if (firstPos.X() < (secondPos.X() - squareDim.X() / 2)) //First is left of second
  {
    point.Set(secondPos.X() - squareDim.X() / 2, secondPos.Y());
  }
  else if (firstPos.X() > (secondPos.X() + squareDim.X() / 2)) //First is right of second
  {
    point.Set(secondPos.X() + squareDim.X() / 2, secondPos.Y());
  }

  if (firstPos.Y() < (secondPos.Y() - squareDim.Y() / 2)) //First is below second
  {
    point.Set(secondPos.X(), secondPos.Y() - squareDim.Y() / 2);
  }
  else if (firstPos.Y() > (secondPos.Y() + squareDim.Y() / 2))//First is above second
  {
    point.Set(secondPos.X(), secondPos.Y() + squareDim.Y() / 2);
  }

  return point;
}

///-------------------------------------
/// Brief: Checks collisions with circle colliders
void ColliderSys::CircleCircleCollision(ColliderCircle& collider1, ColliderCircle& collider2)
{
  Obj* obj1 = collider1.Parent();
  Obj* obj2 = collider2.Parent();

  std::string& firstName = obj1->Name();
  std::string& secondName = obj2->Name();

  if (obj1->IsDead() || obj2->IsDead() || !obj1->IsActive() || !obj2->IsActive())
    return;

  //TraceMessage(true, "Collider Check Circle Collision");

  //Get positions of objects
  Vec2D obj1Pos = GetPosition(firstName);
  Vec2D obj2Pos = GetPosition(secondName);

  float distance = obj1Pos ^ obj2Pos;
  //double dist = obj1Pos.Distance(obj2Pos);
  //distance -= 2.0f;

  //If either object does not have a collider
  //if (collider1 == nullptr || collider2 == nullptr)
  //  return;

  //If either of the objects have collided do not let them collide with anything else until after the refractory period
  if (collider1.Collided() || collider2.Collided())
    return;

  float sum;

  sum = collider1.Radius() + collider2.Radius();

  //If the sum of the radii is greater than the distance then they must be colliding
  if (sum >= distance)
  {
    //Get the orientation of a push block to the player
    //if (collider2.isBlock())
      //SetZone(firstName, secondName); //CALL THIS WHEN MEGAN IS FINISHED WITH HER PART OF IMPLEMENTATION

    //Collision resolution
    collider1.CircleCircleResolution(obj2);
    collider2.CircleCircleResolution(obj1);

    obj1->ResolveCollision(obj2);
    obj2->ResolveCollision(obj1);

    EventManager *man = dynamic_cast<EventManager*>(EngineGetSystem("EventManager"));
    Trigger *trig;

      //Check for Trigger collision
    if (obj1->IsTrigger())
    {
      trig = static_cast<Trigger*>(obj1);
      ColliderCircle *circ2 = &collider2;
      Collider *col2 = dynamic_cast<Collider*>(circ2);
      trig->Collide(*col2);
    }
    else if (obj2->IsTrigger())
    {
      trig = static_cast<Trigger*>(obj2);
      ColliderCircle *circ1 = &collider1;
      Collider *col1 = dynamic_cast<Collider*>(circ1);
      trig->Collide(*col1);
    }
      //check for collision with Player
    else if (obj1->Type().compare("Player") == 0 && obj2->Has(Behavior))
      man->SendPlayerCollisionEvent(obj2->Name());
    else if (obj2->Type().compare("Player") == 0 && obj1->Has(Behavior))
      man->SendPlayerCollisionEvent(obj1->Name());
      //check for events in object's behaviors
    else
    {
      if (obj1->Has(Behavior))
      {
        ColliderCircle *circ2 = &collider2;
        Collider *col2 = dynamic_cast<Collider*>(circ2);
        man->SendCollisionEvent(obj1->Name(), *col2);
      }
      if (obj2->Has(Behavior))
      {
        ColliderCircle *circ1 = &collider1;
        Collider *col1 = dynamic_cast<Collider*>(circ1);
        man->SendCollisionEvent(obj2->Name(), *col1);
      }
    }
  }
}

void ColliderSys::SquareSquareCollision(ColliderSquare & collider1, ColliderSquare & collider2)
{
  //Transforms & translations
  Transform *transform1 = Space::GetTransform(collider1.Parent()->Name());
  Transform *transform2 = Space::GetTransform(collider2.Parent()->Name());
  Physics *physics1 = Space::GetPhysics(collider1.Parent()->Name());
  Physics *physics2 = Space::GetPhysics(collider2.Parent()->Name());
  Vec2D translation1 = transform1->Translation();
  Vec2D dimension1 = collider1.Dimensions();
  Vec2D translation2 = transform2->Translation();
  Vec2D dimension2 = collider2.Dimensions();

  /**********************Corners***************************************************/
  //Object 1
  Vec2D TR1(translation1 + dimension1);
  Vec2D TL1(translation1.X() - dimension1.X(), translation1.Y() + dimension1.Y());
  Vec2D BL1(translation1 - dimension1);
  Vec2D BR1(translation1.X() + dimension1.X(), translation1.Y() - dimension1.Y());

  //Object 2
  Vec2D TR2(translation2 + dimension2);
  Vec2D TL2(translation2.X() - dimension2.X(), translation2.Y() + dimension2.Y());
  Vec2D BL2(translation2 - dimension2);
  Vec2D BR2(translation2.X() + dimension2.X(), translation2.Y() - dimension2.Y());
  /*********************************************************************************/

  //AABB collision exclusion check
  if (!(TR1 > BL2) ||                            //TOP RIGHT TO BOTTOM LEFT
    !(TL1.X() < BR2.X() && TL1.Y() > BR2.Y()) || //TOP LEFT TO BOTTOM RIGHT
    !(BL1 < TR2) ||                              //BOTTOM LEFT TO TOP RIGHT
    !(BR1.X() > TL2.X() && BR1.Y() < TL2.Y()))   //BOTTOM RIGHT TO TOP LEFT
    return;
  else
  {
    if (physics1->Velocity() > 0.0f && physics2->Velocity() == 0.0f)
      collider1.StaticResolution(collider2.Parent()->Name());
    else if (physics1->Velocity() == 0.0f && physics2->Velocity() > 0.0f)
      collider2.StaticResolution(collider1.Parent()->Name());
  }
}

///-------------------------------------
/// Brief: Checks if a point in inside a circle
bool ColliderSys::PointCircle(Vec2D point, Collider *circle)
{
  //Radius
  float radius = (reinterpret_cast<ColliderCircle*>(circle))->Radius();
  //float radiusSquare = radius * radius;

  //Distance
  float distance = glm::sqrt((point.X() * point.X()) + (point.Y() * point.Y()));

  if (distance <= radius)
    return true;
  else
    return false;
}

///-------------------------------------
/// Brief: Checks if a circle intersects a line
bool ColliderSys::LineCircle(Vec2D start, Vec2D end, Collider *circle, Transform *transform)
{
  float radius = (reinterpret_cast<ColliderCircle*>(circle))->Radius();
  Vec2D circlePosition = transform->Translation();
  Vec2D temp1 = start - circlePosition;
  Vec2D temp2 = end - circlePosition;

  //If either point is in the circle
  if (PointCircle(temp1, circle) || PointCircle(temp2, circle))
    return true;

  Vec2D line = end - start;

  /*if (lightCheck)
  {
    glm::vec2 temp(line.X(), line.Y());
    temp = glm::normalize(temp);

    line.X(temp.x);
    line.Y(temp.y);
    line *= LanternScale;

    lightCheck = false;
  }*/

  Vec2D distanceVec = circlePosition - start;
  float length = glm::sqrt((line.X() * line.X()) + (line.Y() * line.Y()));

  float dot = ((distanceVec.X() * line.X()) + (distanceVec.Y() * line.Y())) / (length * length);

  //Beginning of line + projection
  Vec2D closestPoint;
  closestPoint.X(start.X() + (dot * (end.X() - start.X())));
  closestPoint.Y(start.Y() + (dot * (end.Y() - start.Y())));

  //If point is not on the line then return false
  if (!PointLine(start, end, closestPoint))
    return false;

  Vec2D circleLineVec = closestPoint - circlePosition;
  float distance = glm::sqrt((circleLineVec.X() * circleLineVec.X()) + (circleLineVec.Y() * circleLineVec.Y()));

  if (distance <= radius)
    return true;
  else
    return false;
}

///-------------------------------------
/// Brief: Checks if a point is on a line
bool ColliderSys::PointLine(Vec2D start, Vec2D end, Vec2D point)
{
  Vec2D line = end - start;
  Vec2D distance1 = point - start;
  Vec2D distance2 = point - end;
  float lengthLine = glm::sqrt((line.X() * line.X()) + (line.Y() * line.Y()));
  float lengthDistance1 = glm::sqrt((distance1.X() * distance1.X()) + (distance1.Y() * distance1.Y()));
  float lengthDistance2 = glm::sqrt((distance2.X() * distance2.X()) + (distance2.Y() * distance2.Y()));

  if ((lengthDistance1 + lengthDistance2) == lengthLine)
    return true;
  else
    return false;
}

void ColliderSys::CircleSquareCollision(ColliderCircle& circle, ColliderSquare& square)
{
	Transform* circleTrans = circle.Parent()->Has(Transform);
	Transform* squareTrans = square.Parent()->Has(Transform);

	Vec2D circleLoc = circleTrans->Translation();
	Vec2D squareLoc = squareTrans->Translation();
	Vec2D dimensions = square.Dimensions();
	float rad = circle.Radius();

	float circleX = circleLoc.X();
	float circleY = circleLoc.Y();

	float height = dimensions.Y();
	float width = dimensions.X();

	float squareX = squareLoc.X() - width / 2;
	float squareY = squareLoc.Y() - height / 2;


	float edgeX = circleLoc.X();
	float edgeY = circleLoc.Y();

	if (circleX < squareX)
	  edgeX = squareX;
	else if (circleX > squareX + width)
	  edgeX = squareX + width;

	if (circleY < squareY)
	  edgeY = squareY;
	else if (circleY > squareY + height)
	  edgeY = squareY + height;

	float distX = circleX - edgeX;
	float distY = circleY - edgeY;
	float dist = glm::sqrt((distX*distX) + (distY*distY));

	if (dist <= rad) 
  {
		circle.Parent()->ResolveCollision(square.Parent());
		square.Parent()->ResolveCollision(circle.Parent());

    //Stop moving, maybe this can be called in behavior at some point
    circle.StaticResolution(square.Parent()->Name());
	}

}

///-------------------------------------
/// Brief: Checks for collision with main mechanic
void ColliderSys::LightCollision(std::string LanternX, std::string other)
{
  if (other.compare("Player") == 0)
  {
    return;
  }

  Obj *Object = ObjManagerGetObject(other.c_str());
  resType t = Object->ResolutionType();
  if (Object == nullptr || Object->IsDead() || !Object->IsActive() /*|| (Object->ResolutionType() != Enemy_ || Object->ResolutionType() != Trigger_)*/)
    return;

  if (t != Enemy_)
    if (t != Trigger_)
      return;

  Collider *ObjCollider = Object->Has(Collider); //Object's collider
  //EVENT SYSTEM?
  if (!ObjCollider)
    return;

  Graphics *graphicSys = dynamic_cast<Graphics*>(EngineGetSystem("Graphics")); //Get graphics component
  GLFWwindow *window = graphicSys->Window();

  //Lantern data
  Lantern* lantern = static_cast<Lantern*>((&Space::GetPlayer())->GetSubObject("Lantern"));
  Transform *lanternTransform = lantern->Has(Transform);
  Vec2D lanternPos = lanternTransform->Translation();
  Transform *objTransform = Space::GetTransform(other);  //Object's transform
  Vec2D objLoc = objTransform->Translation();

  //Mouse Data--------------------------------------------------------
  glm::vec2 mouseVec;
  GLdouble cursorX = 0.0;
  GLdouble cursorY = 0.0;

  //Get cursor position and translate it to world coordinates
  glfwGetCursorPos(window, &cursorX, &cursorY);

  //Convert mouse coordinates from screen to world
  mouseVec.x = (float)cursorX;
  mouseVec.y = (float)cursorY;
  mouseVec = Graphics::ScreenToWorld(mouseVec);
  //-------------------------------------------------------------------

  //-------------------------------------------------------------------
  //MUST CHECK RANGE -> RADIUS OF HOLE

  //-------------------------------------------------------------------

  Vec2D start = lanternPos; //lantern position
  Vec2D end;                //mouse position
  end.X(mouseVec.x);
  end.Y(mouseVec.y);
  /////////////////////////////////////////////

  

  if (LineCircle(start, end, ObjCollider, objTransform))
  {
    lantern->ResolveCollision(Object);
    Object->ResolveCollision(lantern);

    if (Object->IsTrigger())
    {
      Trigger *trig = static_cast<Trigger*>(Object);
      trig->Collide(Lantern_);
    }

    //REMOVE WHEN EVENT TRIGGERS/COMPONENTS ARE DONE
    ColliderCircle* objCollider = reinterpret_cast<ColliderCircle*>(ObjCollider);
    if (ObjCollider)
    {
	  /*Vec2D push = objLoc - lanternPos;
      objCollider->Collided(true);

      Physics *physics = Space::GetPhysics(Object->Name());
      physics->Velocity(push);
      objCollider->SetFrames(15);*/
    }
  }
}

///-------------------------------------
/// Brief: Checks orthogonal direction of block and sets it in push block class
void ColliderSys::SetZone(std::string firstName, std::string secondName)
{
  //This only concerns the player and push blocks
  if (firstName != "Player")
    return;

  //Get necessary components
  PushBlockManager *blockManager = static_cast<PushBlockManager*>(EngineGetSystem("PushBlockManager"));
  ScreenManager *screenManager = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
  Area *currentArea = static_cast<Area*>(screenManager->CurrentScreen());

  if (currentArea == nullptr)
    return;

  PushBlock *pushBlock = currentArea->GetPushBlockPuzzle();

  if (pushBlock == nullptr)
    return;

  Obj *obj = ObjManagerGetObject(secondName.c_str()); //This may need to become getting pushblocks from "pushBlocks"
  objectList = screenManager->CurrentObjList();
  Transform *playerTrans = Space::GetTransform("Player");
  Transform *blockTrans = Space::GetTransform(secondName);
  Vec2D position = playerTrans->Translation();
  Vec2D blockPosition = blockTrans->Translation();
  Vec2D dir = position - blockPosition;

  //Vector for Z axis
  glm::vec3 Zaxis;
  Zaxis.x = 0;
  Zaxis.y = 0;
  Zaxis.z = 1;

  //Convert to glm
  glm::vec3 direction;
  direction.x = dir.X();
  direction.y = dir.Y();
  direction.z = 0.0f;

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
      blockManager->SetBlock(obj, Top_);
      return;
    }
    else if (alpha2 < 0)
    {
      //Block is to the left of the player
      blockManager->SetBlock(obj, Left_);
      return;
    }
    else if (alpha3 < 0)
    {
      //Block is below player
      blockManager->SetBlock(obj, Bottom_);
      return;
    }
    else
    {
      //Block is to the right of player
      blockManager->SetBlock(obj, Right_);
      return;
    }
  }
  else if (theta < 0)
  {
    //Block is to the right of player
    blockManager->SetBlock(obj, Right_);
    return;
  }

  return;
}