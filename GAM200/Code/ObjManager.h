#pragma once

///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ObjManager.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This is the interface for the GameObjectManager, it should be 
//			be responsible for all of the objects in the array.
//      
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
//#define Has(type) GetComp<type>(type)

#include <vector>
#include "Obj.h"
//#include "Grid.h"
#include "CollisionManager.h"
#include "Stats.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class ObjManager
{
public:
	/*
	These are Game Object Manager Functions and should remain
	*/
	ObjManager();
	~ObjManager();
	void AddObj(Obj *gameObject);
	void UpdateObjs(float dt);
	Obj* GetObj(const char *name);
	bool CheckName(const char *name, int index);
	Obj* GetNextObj(int index);
	int GetListSize();
  bool ObjIsDead(std::string& name);
  bool ObjIsActive(std::string& name);
  std::vector<Obj*> GetObjsInMap(std::string mapName);
  Obj * FindFirstDead(std::string& type);
  Obj * FindFirstDeadofMine(eClass clss, char id);

    //These are for finding currently !isActive objs to spawn
  Obj * FindObjContaining(std::string text);
  Obj * GetObjCurrentScreen(std::string type);
  Obj * GetObjCurrScreenByName(std::string name);

  Obj * GetObjThisScreen(std::string type, std::string screenName);
  int NumofEnemyInMap(eClass type, std::string mapName);
  int NumDeadofTypeInMap(eClass type, std::string mapName);
  int NumDeadofType(eClass type);
  int NumDeadInMap(std::string mapName);

  static ObjManager * GetInstance();

	/*
	These are Grid functions and should later be abstracted if they can be
	*/

	/*typedef enum GridCode {
		Empty_,
		Player_,
		Enemy_,
		StaticObj_,
	}GridCode;*/

	//Grid();
	//~Grid();
	/*void CreateGrid();
	void DestroyGrid();
	int GetIndex(float x, float y);
	void GetWidth();
	void GetHeight();
	int SetRows();
	int SetColumns();
	int GetSize();
	void UpdateGrid();*/

private:

	/*float GridWidth_;
	float GridHeight_;
	int GridSize_;
	int rows_;
	int columns_;
	int Grid_[][];*/
  int place_;
};


///---------------------------------FUNCTIONS-----------------------------------
Obj * ObjManagerGetObject(const char *name);
void ObjManagerAddObject(Obj *obj);
ObjManager& GetObjManager();
bool ObjManagerObjIsDead(std::string& name);
bool ObjManagerObjIsActive(std::string& name);
Obj * ObjManagerFindContaining(std::string text);
