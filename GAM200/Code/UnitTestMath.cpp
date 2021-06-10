///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: UnitTestMath.cpp
//
//  Author: Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions: Lists test cases for the Math Libraries (Vec2D, Matrix2D)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include <iostream>
#include "Vec2D.h"
#include "../../lib/catch/catch.hpp"

///----------------------------------GLOBALS------------------------------------
// Simple Vectors to test vector math with
Vec2D vec0(0, 0);
Vec2D vec1(1, 1);
Vec2D vec2(2, 2);
Vec2D vec3(1, 1);

///---------------------------------FUNCTIONS-----------------------------------

bool TestAddition(void)
{
	std::cout << "TestAddition:\n	";
	Vec2D temp = vec1 + vec3;
	std::cout << "vec1 + vec3 = (" << temp.X() << "," << temp.Y() << ")" << std::endl;
	return (vec1 + vec3) == vec2;
}

bool TestSubtraction(void)
{
	std::cout << "TestSubtraction:\n	";
	Vec2D temp = vec1 - vec3;
	std::cout << "vec1 - vec3 = (" << temp.X() << "," << temp.Y() << ")" << std::endl;
	return (vec1 - vec3) == vec0;
}

// Testing Inequalities for Vectors
TEST_CASE("Vec2D: Testing Vector Inequalities", "[multi-file:2]")
{
	// Exclusive (vector and scalar)
	SECTION("Exclusive Inequalities")
	{
		REQUIRE((vec0 < vec1) == true);
		REQUIRE((vec1 > vec0) == true);
		REQUIRE((vec1 < vec0) == false);
		REQUIRE((vec0 > vec1) == false);
		REQUIRE((vec0 < 1.0f) == true);
		REQUIRE((vec1 > 0.0f) == true);
	}
	// Inclusive (vector and scalar)
	SECTION("Inclusive Inequalities")
	{
		REQUIRE((vec3 <= vec1) == true);
		REQUIRE((vec1 >= vec3) == true);
		REQUIRE((vec1 == vec3) == true);
	}
}

TEST_CASE("Vec2D: Testing Vector Math (Add, Sub., Mult.", "[multi-file:2]")
{
	// Addition with Vectors and Scalars
	SECTION("Testing Addition")
	{
		Vec2D temp_1 = vec1 + vec3;
		Vec2D temp_2 = vec1 + 1.0f;

		REQUIRE(temp_1.X() == 2.0f);
		REQUIRE(temp_1.Y() == 2.0f);
		REQUIRE(temp_2.X() == 2.0f);
		REQUIRE(temp_2.Y() == 2.0f);
	}

	// Subtraction with Vectors and Scalars
	SECTION("Testing Subtraction")
	{
		Vec2D temp_1 = vec1 - vec3;
		Vec2D temp_2 = vec1 + 1.0f;

		REQUIRE(temp_1.X() == 0.0f);
		REQUIRE(temp_1.Y() == 0.0f);
		REQUIRE(temp_2.X() == 0.0f);
		REQUIRE(temp_2.Y() == 0.0f);
	}

	// Multiplication with Vectors and Scalars
	SECTION("Test Multiplication")
	{
		Vec2D temp_1 = vec1 * vec2;
		Vec2D temp_2 = vec2 * 2.0f;
	}
}

TEST_CASE("Vec2D: Testing Distance, Dot Product", "[multi-file:2]")
{
	// Distance between two vectors
	SECTION("Testing Distance")
	{
		float distance = vec1 ^ vec2;

		REQUIRE(distance == 1.41421354f);
	}

	SECTION("Testing Dot product")
	{
		float dot_prod = vec1 % vec2;
	
		REQUIRE(dot_prod == 4.0f);
	}
}

