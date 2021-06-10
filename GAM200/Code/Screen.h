///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Screen.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: general game screen, holds objects and background
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "Obj.h"
#include "Behavior.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class Screen
{
 public:
   Screen(std::string name);
   ~Screen(){};

   typedef enum
   {
     Default_,
     Area_,
     Overlay_,
   }ScreenType;
      
   std::string GetOGName();
   std::string GetName();
   void SetName(std::string name);
   ScreenType GetType();
   void SetType(ScreenType type);
   std::vector<Obj*> GetObjectList();
   std::string GetBackground();
   void SetBackground(std::string filename); // won't change until project reloads

   void AddNewObject(std::string archetype); // use this instead of copying archetypes!

   void AddObject(Obj* object); // adds object to screen object list
   void RemoveObject(Obj* object); // removes object from screen object list

   int IncreaseObjectCount(std::string type);

   bool IsStartScreen();
   void IsStartScreen(bool start);

   bool AllEnemiesDead(); //Added by Sonya

   void KillAll();

   // used for object name
   char GetID();

   // TODO:
   Behavior* FindDoor(std::string doorID);

   // light stuff
   // for light
   //std::vector<glm::vec2&> LightPos();
   //std::vector<float&> LightScales();

 private:
   std::string background_; // separate object from the rest
   ScreenType type_; // screen type

   std::string name_;   // current name
   std::string ogName_; // original name

   std::vector<Obj*> objects_; // objects in screen

   bool startScreen_ = false;

   char ID_; // set automatically
   std::map<std::string, int> objCount_;

   // stores doors
   std::vector<Behavior*>doors_;

   //// light stuff
   //std::vector<glm::vec2&> lightPos_;
   //std::vector<float&> lightScales_;
};

///---------------------------------FUNCTIONS-----------------------------------


