/******************************************************************************
filename    stdafx.h
author      Matthew Picioccio

Brief Description:
This file includes standard system include files, or project-specific 
include files that are used frequently, but are changed infrequently.

******************************************************************************/
#pragma once

/* disable warnings for single-line comments, since Microsoft's stdlib, etc use those extensively */
#pragma warning(push)
#pragma warning(disable : 4001)

#include "targetver.h" /* required by Windows */

#include <stdlib.h> /* NULL, malloc/free */
#include <stdio.h> /* printf, fgets */
#include <stdbool.h> /* bool */
#include <string.h> /* strcmp, strcpy_s, strtok_s */
#include <cstdarg> 

//Klang headers
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <Windows.h>

#include "Trace.h"
#include "Events.h"

/* 
 * Used to clean /W4 unused parameters for functions that must match a function-pointer type 
 * NOTE: copied from winnt.h, but we don't want to include that here, otherwise 
 */
#define UNREFERENCED_PARAMETER(P) (P)

/* pop the warning-disable */
#pragma warning(pop)