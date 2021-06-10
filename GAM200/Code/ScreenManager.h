///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ScreenManager.h
//
//  Author: Megan Works
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief: Holds and manages game screens
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "System.h"
#include "Area.h"
#include "SetDoor.h"

///----------------------------------GLOBALS------------------------------------

///----------------------------------CLASSES------------------------------------
class ScreenManager : public System
{
  public: 
    ScreenManager();
    ~ScreenManager();
    virtual void Initialize();
    virtual void Update(float dt);

    void BorderCheck(float dt); // checks borders

    typedef enum
    {
      North_,
      South_,
      East_,
      West_,
    } direction;

    // returns current screen
    Screen* CurrentScreen();

    // changes current screen (give screen or name)
    void ChangeCurrentScreen(Screen * screen);
    void ChangeCurrentScreen(std::string screenname);
    void ChangeCurrentScreen(std::string screenname, direction dir);

    // returns current object list
    std::vector<Obj*> CurrentObjList();

    // creates a screen/area but does not attach to anything
    Screen* CreateNewScreen(std::string name);
    Area* CreateNewArea(std::string areaName);

    // creates a screen AND attaches to current screen
    // DO NOT USE IF CURRENT SCREEN IS NOT AN AREA
    // or else
    // more useful for engine most likely
    Area* CreateNewArea(std::string areaName, direction dir);

    // adds screen to map -- mostly for serialization
    void AddScreen(std::string name, Screen *screen);

    // holds/changes what screen loads first
    void StartScreen(Screen * screen);
    Screen* StartScreen();

    Screen* FindScreen(std::string name);

      //Added by Sonya
    bool ScreenIsComplete(std::string screenName);
    void SetDoor(DoorData& data);

    // use to reset screen
    void ResetArea(Area * area);
    void ForceGameReset(); // resets ALL objects, events, enemies

    // music stuff
    void BackgroundMusic(std::string filename);
    void MuteBackground(bool mute);
    void StopBackground();
    void PauseBackground(bool pause);

    std::string BackgroundMusic();
    std::string OGBackgroundMusic();

    void StartScreens();
    void StopScreens();
    bool ScreensStarted();

    void AddAggro();
    void RemoveAggro();

    void KillAllEnemies();

  private:
    bool begin_ = false;
    Screen * current_; // active screen
    Screen * next_;

    Screen * start_ = nullptr;

    std::string ogBackground_ = "Data/Audio/EndRestMusic/EndRestRegular.wav";
    std::string currentBackground_ = "Data/Audio/EndRestMusic/EndRestRegular.wav";
    std::string idleBackground_ = "Data/Audio/EnemyIdle.wav";

    int numaggro_;
    
    typedef std::map<std::string, Screen*>ScreenMap;
    ScreenMap screens_;
};

///---------------------------------FUNCTIONS-----------------------------------
void ScreensActive(bool active);
bool ScreensActive();
