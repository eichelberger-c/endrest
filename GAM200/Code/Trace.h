///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Trace.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Functions to send messages to traceLogs
//         set isCollision bool if debugging collisions
//         include filename as first part of message
//
//         i.e.
//         first parameter:   true/false
//         second parameter: "Filename -- Message"
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
#pragma once
typedef enum logType
{
  TraceLog,
  GraphicsLog,
  CollisionLog,
  None,
} logType;

void TraceInit();
void TraceMessage(logType type, const char *string, ...);
void TraceShutdown();