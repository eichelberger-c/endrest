///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: UnitTestCatch.h
//
//  Author: Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions: Acts as the main() for unit tests: prints results to
//			   the console window
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
#include <iostream>
#include "UnitTest.h"

///----------------------------------INCLUDES-----------------------------------
#define CATCH_CONFIG_RUNNER
#include "../../lib/catch/catch.hpp"

///----------------------------------GLOBALS------------------------------------

///---------------------------------FUNCTIONS-----------------------------------
TEST_CASE("1: All test cases reside in other .cpp files", "[multi-file:1]") 
{
}

int main(int argc, char* argv[]) {
	
	int result = 0;

	// Run Catch session
	result = Catch::Session().run(argc, argv);

	// Show Test output in the event of a failure
	if (result != 0)
	{
		std::cout << "TEST OUTPUT\n===============================================================================\n";
		//TestAddition();
		//TestSubtraction();
	}

 	std::cout << "Press ENTER to exit" << std::endl;
	getchar();


	return result;
}