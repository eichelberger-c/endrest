#pragma once
///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Editor.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"

#include <GLFW\glfw3.h>
#include "Obj.h"
#include "Animation.h"
#include "Sprite.h"
#include "Transform.h"
#include "Physics.h"
#include "Behavior.h"
#include "Audio.h"
#include "System.h"
#include "Health.h"
#include "Lighting.h"
#include "ParticleEmitter.h"
#include "ImGuiafx.h"


///----------------------------------GLOBALS------------------------------------
typedef enum headerType
{
  Invalid_ = -1,
  Collapsing_,
  Tree_,
} headerType;

///----------------------------------CLASSES------------------------------------
class Editor : public System
{
public:
  Editor();
  ~Editor();
  virtual void Initialize();

  virtual void Update(float dt);
  static void NewFrame();
  static void Render();
  static void Draw();

  void ObjectMenu(std::vector<Obj*> objects);
  static float MainMenu();
  void ArchetypeMenu(std::vector<Obj*> objects);
  static void SettingMenu();
  static void TilesMenu(float offsetY);
  static void ObjectEditor(float offsetY);
  static void ScreenEditor(float offsetY);
  static void SelectedObjectMenu(float offsetY);
  static void ComponentsMenu(float offsetY);

  void Copy();
  void Paste();
  static void Save();
  static void UpdatePos();

  void CreateComps();
  void DeleteComps();

  inline static bool GetDelete() { return delete_; }
  inline static void SetDelete(bool del) { delete_ = del; }

  inline static bool GetSave() { return save_; }
  inline static void SetSave(bool save) { save_ = save; }

  void SetActive(bool);
  bool GetActive();
  void SetSelectedObject();
  static void DeleteObj();

  static bool Paused();

  // Adds a Popup to be drawn
  static void AddPopup(PopupParam& popup);
  // Renders any Popups created in the frame
  static void DrawPopups(void);
private:
  // Active menus/editor windows
  static bool editorActive_;
  static bool objectMenuActive_;
  static bool spawnMenuActive_;
  static bool archeTypeMenuActive_;
  static bool settingsMenuActive_;
  static bool screenEditorActive_;

  static bool delete_;
  static bool save_;
  int gridSnap_ = 20;

  static std::vector<Component*> components_;
  static Obj* selectedObj_;
  static std::string* clipBoard_;
};

///---------------------------------FUNCTIONS-----------------------------------
void InitImGui(GLFWwindow *window);

void ImGuiCleanUp();

void ImGuiLightingMenu(Lighting* light, headerType type);

void ImGuiParticleMenu(ParticleEmitter* particles, headerType type);

void ImGuiSpriteMenu(Sprite* sprite, headerType Bold);

void ImGuiAnimationMenu(Animation* animation, headerType Bold);

void ImGuiPhysicsMenu(Physics* physics, headerType Bold);

void ImGuiTransformMenu(Transform* transform, headerType Bold);

void ImGuiBehaviorMenu(Behavior* behavior, headerType Bold);

void ImGuiHealthMenu(Health* health, headerType Bold);

void ImGuiAudioMenu(Audio* audio, headerType type);

void ImGuiKeyMenu(Key* key, headerType type);

void ImGuiComponentMenu(Component* comp);

void ImGuiSetPay(Component* comp);

void ImGuiColliderMenu(Collider* col, headerType tpye);

const ImGuiPayload* ImGuiObjectMenu(Obj* obj);