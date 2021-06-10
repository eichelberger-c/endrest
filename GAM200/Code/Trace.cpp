///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Trace.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
#include "stdafx.h"


static FILE *traceLog = nullptr;
static FILE *collisionLog = nullptr;
static FILE *currentLog = nullptr;
static FILE *graphicsLog = nullptr;

static bool logsOpen = false;

void TraceInit()
{
  // opens both general and collision specific trace logs
#ifdef _DEBUG
  traceLog = fopen("TraceLogs/traceLog.log", "w");
  collisionLog = fopen("TraceLogs/collisionLog.log", "w");
  graphicsLog = fopen("TraceLogs/GraphicsLog.log", "w");
#endif // _DEBUG

  if (traceLog && collisionLog && graphicsLog)
    logsOpen = true;
  else
  {
    printf("tracelogs not opened");
  }

}

// write message to trace logs
// isCollision = true to write to collision 
void TraceMessage(logType type, const char *string, ...)
{
#ifdef _DEBUG

  // if no tracelog is found then only printed to debug window
  if (!logsOpen)
    return;


  switch (type)
  {
    case GraphicsLog:
    {
      currentLog = graphicsLog;
      break;
    }
    case CollisionLog:
    {
      currentLog = collisionLog;
      break;
    }
    case TraceLog:
    {
      currentLog = traceLog;
      break;
    }
  }

  // writes string to correct log and debug window
  va_list arg_ptr;
  va_start(arg_ptr, string);

  if (currentLog)
  {
    vfprintf_s(currentLog, string, arg_ptr);
    fprintf_s(currentLog, "\n");
  }

  if (type == TraceLog)
  {
    vprintf(string, arg_ptr);
    printf("\n");
  }

  va_end(arg_ptr);

#endif // _DEBUG
}

// closes both trace logs
void TraceShutdown()
{
  if (traceLog)
    fclose(traceLog);
  if (collisionLog)
    fclose(collisionLog);
  if (graphicsLog)
    fclose(graphicsLog);
}