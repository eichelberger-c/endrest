///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ObjInput.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2018 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ObjInput.h"
#include "Spaces.h"
#include "Physics.h"
#include "Collider.h"
#include "Obj.h"
#include "Transform.h"
#include "System.h"
#include "Engine.h"
#include "GLFW/glfw3.h"

///----------------------------------GLOBALS-----------------------------------
Graphics* graphicSys;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: 
ObjInput::ObjInput() : Component(Component::ObjInput_)
{
	//isTriggered_ = false;
}

///-------------------------------------
/// Brief: 
ObjInput::ObjInput(const Component & ogComponent) : Component(Component::ObjInput_)
{
}

///-------------------------------------
/// Brief: 
Component * ObjInput::Clone() const
{
	return nullptr;
}

///-------------------------------------
/// Brief: This function determines in a button is 
/// being pressed and responds accorcdingly. this
/// function may also need to tell the graphics engine 
/// where to render the lantern reticle.
void ObjInput::Update(float dt)
{
	/*
	As of right now, the movement can be handled with simple velocity manipulating position without need for 
	a normalized vector with rotation calculated
	*/
	graphicSys = dynamic_cast<Graphics*>(EngineGetSystem("Graphics"));
	
	GLFWwindow *window = graphicSys->GetWindow();

	UpdateMouse(window); //This should always be done

	Collider *collider = reinterpret_cast<Collider*>(Parent()->Get(Component::Collider_)); //Get parent objects collider component
	Physics *physics = reinterpret_cast<Physics*>(Parent()->Get(Component::Physics_)); //Get parent objects physics component
	Transform *transform = reinterpret_cast<Transform*>(Parent()->Get(Component::Transform_)); //Get parent transform component
	
	//Create a vector for the new velocity
	Vec2D newVelocity;

	//if (collider->frames_ > 0) //If the collision frames are not cycled, don't take input
	//	return;
	
	//Create a vector to hold current velocity
	Vec2D oldVelocity = physics->Velocity();

	int upKey  = glfwGetKey(window, GLFW_KEY_UP);
	int leftKey = glfwGetKey(window, GLFW_KEY_LEFT);
	int downKey = glfwGetKey(window, GLFW_KEY_DOWN);
	int rightKey = glfwGetKey(window, GLFW_KEY_RIGHT);

	//Orthogonal movement////////////////////////////////////////////////
	if (upKey == GLFW_PRESS) // Rotation = 90
	{
		//Set velocity in Y direction to be positive
		
		newVelocity.Set(oldVelocity.X(), 3.0f);
		physics->Velocity(newVelocity);
		transform->Rotation(90.0f);
		return;
		
	}
	else if (leftKey == GLFW_PRESS) // Rotation = 180
	{
		//Set velocity in X direction to be negative
		
		newVelocity.Set(-3.0f, oldVelocity.Y());
		physics->Velocity(newVelocity);
		transform->Rotation(180.0f);
		return;
		
		
	}
	else if (downKey == GLFW_PRESS) // Rotation = 270
	{
		//Set velocity in Y direction to be negative
		
		newVelocity.Set(oldVelocity.X(), -3.0f);
		physics->Velocity(newVelocity);
		transform->Rotation(270.0f);
		return;
		
	}
	else if (rightKey == GLFW_PRESS) // Rotation = 0
	{
		//Set velocity in the X direction to be positive
		/*
		newVelocity.Set(3.0f, oldVelocity.Y());
		physics.Velocity(newVelocity);
		Transform.Rotation(0.0f);
		return;
		*/
	}
	//Diagonal movement/////////////////////////////////////
	if (upKey == GLFW_PRESS && rightKey == GLFW_PRESS) // Rotation = 45
	{
		//Set velocity in Y and X direction to be positive
		/*
		newVelocity.Set(3.0f, 3.0f);
		physics.Velocity(newVelocity);
		Transform.Rotation(45.0f);
		return;
		*/
	}
	else if (upKey == GLFW_PRESS && leftKey == GLFW_PRESS) // Rotation = 135
	{
		//Set velocity in X direction to be negative and Y direction to be positive
		/*
		newVelocity.Set(-3.0f, 3.0f);
		physics->Velocity(newVelocity);
		Transform.Rotation(135.0f);
		return;
		*/

	}
	else if (downKey == GLFW_PRESS && leftKey == GLFW_PRESS) // Rotation = 225
	{
		//Set velocity in Y direction to be negative and X direction to be negative
		/*
		newVelocity.Set(-3.0f, -3.0f);
		physics.Velocity(newVelocity);
		Transform.Rotation(225.0f);
		return;
		*/
	}
	else if (downKey == GLFW_PRESS && rightKey == GLFW_PRESS) // Rotation = 315
	{
		//Set velocity in the X direction to be positive and Y direction to be negative
		/*
		newVelocity.Set(3.0f, -3.0f);
		physics.Velocity(newVelocity);
		Transform.Rotation(315.0f);
		return;
		*/
	}
	else
		;//Set velocity in both directions to 0
		 /*
		 newVelocity.Set(0.0f, 0.0f);
		 physics.Velocity(newVelocity);
		 return;
		 */
	
}

///-------------------------------------
/// Brief: Calculates and returns mouse position
///	as a vector.
void ObjInput::UpdateMouse(GLFWwindow * window)
{
	//This function should act much like the CS230 project 3 GameState 2 
	//function that calculates the mouse position
	//Subsequently sets the mouse position private member
	//A combat type component will likely have to exist at some point and be friends with
	//this class in order to use the mouse position
	//Custom cursor?
	//http://www.glfw.org/docs/latest/input_guide.html#input_mouse_button


	//CS230 Code - Translate!!!
	////Get Transform, Translation,  and Physics components
	//// C++->get transform and physics components
	//TransformPtr transformP = GameObjectGetTransform(GameObjectP);
	//PhysicsPtr physicsP = GameObjectGetPhysics(GameObjectP);
	//const Vector2D *newTranslation = TransformGetTranslation(transformP);

	////Temporary vectors to hold values
	//Vector2D normalVec = { 0, 0 };
	//Vector2D newVelocity = { 0, 0 };
	//Vector2D direction = { 0, 0 };

	////Mouse position
	//s32 mousePosX = 0;
	//s32 mousePosY = 0;

	////World position
	//float newPosX = 0.0f;
	//float newPosY = 0.0f;
	//Vector2D worldPos = { 0, 0 };

	//if (transformP && physicsP)
	//{
	//	//Get cursor position and translate it to world coordinates
	//	AEInputGetCursorPosition(&mousePosX, &mousePosY);
	//	AEGfxConvertScreenCoordinatesToWorld((float)mousePosX, (float)mousePosY, &newPosX, &newPosY);

	//	//Translate world position into vector, normalize it, and rotate it
	//	Vector2DSet(&worldPos, newPosX, newPosY);
	//	Vector2DSub(&direction, &worldPos, newTranslation);
	//	Vector2DNormalize(&normalVec, &direction);
	//	TransformSetRotation(transformP, atan2f(normalVec.y, normalVec.x));

	//	//Calculate and set new velocity
	//	newVelocity.x = normalVec.x * spaceshipSpeed;
	//	newVelocity.y = normalVec.y * spaceshipSpeed;
	//	PhysicsSetVelocity(physicsP, &newVelocity);
	//}
}


void ObjInput::CalcVelocity()
{

	/*
	This is CS230 code to translate for calculating the player's position 
	based on keyboard input, calls to this function should be placed inside 
	of update function
	*/

	//PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);
	//TransformPtr transform = GameObjectGetTransform(behavior->parent);

	//if (physics && transform)
	//{
	//	float rotation = TransformGetRotation(transform);              //Get old rotation
	//	const Vector2D *oldVelocity = PhysicsGetVelocity(physics);     //Get old velocity
	//	Vector2D velocity = { 0 };                                     //New velocity vector
	//	Vector2D rotateVector;                                         //New direction vector
	//	Vector2D rotateUnitVector;                                     //Unit vector for new direction

	//	velocity.x = oldVelocity->x;                                   //Set to old velocity
	//	velocity.y = oldVelocity->y;                                   //Set to old velocity

	//	Vector2DFromAngleRad(&rotateVector, rotation);                 //Get direction
	//	Vector2DNormalize(&rotateUnitVector, &rotateVector);           //Normalize direction

	//	velocity.x += rotateUnitVector.x * spaceshipAcceleration * dt; //Re-calculate velocity
	//	velocity.y += rotateUnitVector.y * spaceshipAcceleration * dt; //Re-calculate velocity

	//	float speed = Vector2DLength(&velocity);                       //Calculate speed

	//	if (speed > spaceshipSpeedMax)
	//	{
	//		velocity.x *= (spaceshipSpeedMax / speed);                 //re-calculate velocity again
	//		velocity.y *= (spaceshipSpeedMax / speed);                 //re-calculate velocity again
	//	}

	//	PhysicsSetVelocity(physics, &velocity);                        //Set new velocity
	//}
}
