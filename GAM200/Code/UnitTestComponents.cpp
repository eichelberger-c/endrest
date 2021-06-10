///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: UnitTestComponents.cpp
//
//  Author: Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------------------------------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Obj.h"
#include "Component.h"
#include "Audio.h"
#include "Animation.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Behavior.h"
#include "BehaviorScripts.h"
#include "Health.h"
#include "Physics.h"
#include "Transform.h"
#include "../../lib/catch/catch.hpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
TEST_CASE("Testing Game Object Components", "[multi-file:1]")
{
	SECTION("Base Component")
	{
		// Create a Component
		Component comp(Component::Invalid_);
		// Clone it
		Component* copy= comp.Clone();

		// test for type and that component is active
		REQUIRE(comp.Type() == Component::Invalid_);
		REQUIRE(comp.IsActive() == true);
		// Test that clone was successful
		REQUIRE(copy == NULL);
		REQUIRE(copy->Type() == Component::Invalid_);
	}
	SECTION("Audio Component")
	{
		// Creat Audio Component
		Audio* audio = new Audio();
		REQUIRE(audio != NULL);
	}
	SECTION("Animation Component")
	{
		// Create Animation
		Animation* animation = new Animation("test");
		REQUIRE(animation != NULL);
	}
	SECTION("Sprite/SpriteSource Component(s)")
	{
		// Create a Sprite
		Sprite* sprite = new Sprite();
		REQUIRE(sprite != NULL);

		// Test Alpha
		float adjust = -0.05f;
		sprite->SetAlpha(0.75f);
		sprite->AdjustAlpha(adjust);
		REQUIRE(sprite->GetAlpha() == 0.70f);

		// Create SpriteSource
		//SpriteSource* source = new SpriteSource("Data/Textures/default.png",2,2);
		//REQUIRE(source == NULL);	
	}
	SECTION("Behavior Component")
	{
		// Create Behavior Component
		Behavior* behavior = new Behavior();
		REQUIRE(behavior != NULL);
	}
	SECTION("Health Component")
	{
		// Create Health Component
		Health* health = new Health(100);
		REQUIRE(health != NULL);

		// Test setting hitpoints
		REQUIRE(health->MaxHP() == 100);
		health->HitPoints(75);
		REQUIRE(health->HitPoints() == 75);
		health->HitPoints(105);
		REQUIRE(health->HitPoints() <= 100);
		health->TakeDamage(10);
		REQUIRE(health->HitPoints() <= 90);
	}
	SECTION("Physics Component")
	{
		// Create Physics Component
		Physics* physics = new Physics();
		REQUIRE(physics != NULL);
	}
	SECTION("Transform Component")
	{
		Vec2D test(1.0f, 1.0f);
		// Create Transform Component
		Transform* transform = new Transform(1.0f,1.0f);
		REQUIRE(transform != NULL);
		transform->MultScale(2.0f);
		REQUIRE(transform->MultScale() == 2.0f);
		transform->Rotation(90.0f);
		REQUIRE(transform->Rotation() == 90.0f);
		transform->Translation(Vec2D(1.0f, 1.0f));
		Vec2D transVec = transform->Translation();
		REQUIRE(transVec.X() == test.X());
		REQUIRE(transVec.Y() == test.Y());
	}
}