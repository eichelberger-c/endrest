///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Editor.cpp
//
//  Author: Charles Eichelberger, Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Stuff for the in-game editor.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ScreenManager.h"
#include "Serialization.h"
#include "ObjFactory.h"
#include "Engine.h"
#include "Graphics.h"
#include "GameStateManager.h"
#include "SlowChase.h"
#include "Input.h"
#include "ObjManager.h"
#include "Input.h"
#include "Menu.h"
#include "Editor.h"
#include "Peephole.h"
#include "FrameRateController.h"
#include "CallBacks.h"
#include "PeepHole.h"
#include "Menu.h"
#include "Spaces.h"
///----------------------------------GLOBALS-----------------------------------

extern FrameRateManager FPSController;
static GameStateManager *GSM;
ArchetypeList* archList;
static int numCopy = 0;
bool Editor::editorActive_ = false;
bool Editor::objectMenuActive_ = true;
bool Editor::spawnMenuActive_ = true;
bool Editor::archeTypeMenuActive_ = true;
bool Editor::settingsMenuActive_ = true;
bool Editor::screenEditorActive_ = false;
std::vector<Component*> Editor::components_;
Obj* Editor::selectedObj_ = nullptr;
std::string* Editor::clipBoard_ = nullptr;

// Private values 
static bool has_changed = true;
static bool door_changed = true;
static bool scale_changed = true;
bool Editor::save_ = false;
bool Editor::delete_ = false;
static bool key_changed = true;
static int type_index = 0;
static float dt_ = 1.0f / 60.0f;

static std::vector<PopupParam> Popups;
///---------------------------------FUNCTIONS-----------------------------------

void InitImGui(GLFWwindow *window)
{
  ImGui::CreateContext();
  ImGui_ImplOpenGL3_Init("#version 400");
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  Graphics::SetCallbacks();
  ImGui::StyleColorsDark();
  //ImGui::PushStyleColor(0, ImVec4(.33,.33,.33,1));
}

void ImGuiCleanUp()
{
  if (Graphics::Window())
  {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
  }
}

const ImGuiPayload* ImGuiObjectMenu(Obj* obj)
{
  if (obj && obj->Name().c_str())
  {
	// Get this object's components
    Sprite* sprite = obj->Has(Sprite);
    Animation* animation = obj->Has(Animation);
    Transform* transform = obj->Has(Transform);
    Behavior* behavior = obj->Has(Behavior);
    Physics* physics = obj->Has(Physics);
    Health* health = obj->Has(Health);
    Audio* audio = obj->Has(Audio);
    Lighting* light = obj->Has(Lighting);
    ParticleEmitter* particles = obj->Has(ParticleEmitter);
	Key* key = obj->Has(Key);
	Collider* col = obj->Has(Collider);
    if (ImGui::CollapsingHeader(obj->Name().c_str()))
    {
      const ImGuiPayload* payLoad = NULL;
      if (ImGui::BeginDragDropTarget())
      {
        payLoad = ImGui::AcceptDragDropPayload("Comp");
        ImGui::EndDragDropTarget();
      }
      ImGuiPhysicsMenu(physics, Tree_);
      ImGuiSpriteMenu(sprite, Tree_);
      ImGuiLightingMenu(light, Tree_);
      ImGuiParticleMenu(particles, Tree_);
      ImGuiAnimationMenu(animation, Tree_);
      ImGuiTransformMenu(transform, Tree_);
      ImGuiBehaviorMenu(behavior, Tree_);
      ImGuiHealthMenu(health, Tree_);
      ImGuiAudioMenu(audio, Tree_);
	  ImGuiKeyMenu(key, Tree_);
	  ImGuiColliderMenu(col, Tree_);
      ImGui::NewLine();
      return payLoad;
    }
    else
    {
      const ImGuiPayload* payLoad = NULL;
      if (ImGui::BeginDragDropTarget())
      {
        payLoad = ImGui::AcceptDragDropPayload("Comp");
        ImGui::EndDragDropTarget();
      }
      return payLoad;
    }
  }
  return NULL;
}

void ImGuiLightingMenu(Lighting* light, headerType type)
{
  if (light)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Lighting");
    else if (type == Tree_)
      open = ImGui::TreeNode("Lighting");
    if (open)
    {
      glm::vec4 color = light->Color();
      ImGui::ColorPicker4("Color", &color[0], ImGuiColorEditFlags_AlphaBar);
      light->Color(color);
      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiParticleMenu(ParticleEmitter* particles, headerType type)
{
  if (particles)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Particles");
    else if (type == Tree_)
      open = ImGui::TreeNode("Particles");
    if (open)
    {
      ImGui::Button("Type");
      if (ImGui::BeginPopupContextItem("Type", 0))
      {
        if (ImGui::MenuItem("Point"))
          particles->Type(Point_);
        else if (ImGui::MenuItem("Circle"))
          particles->Type(Circle_);
        else if (ImGui::MenuItem("Line"))
          particles->Type(Line_);
        ImGui::EndPopup();
      }

      ImGui::SameLine();
      ImGui::Text("%*s", 6, "");
      ImGui::SameLine();

      ImGui::Button("Behavior");
      if (ImGui::BeginPopupContextItem("Behavior", 0))
      {
        if (ImGui::MenuItem("Flying"))
          particles->Behavior(Flying_);
        else if (ImGui::MenuItem("Breath"))
          particles->Behavior(Breath_);
        else if (ImGui::MenuItem("Stationary"))
          particles->Behavior(Stationary_);
        else if (ImGui::MenuItem("Trailing"))
          particles->Behavior(Trailing_);
        else if (ImGui::MenuItem("Random"))
          particles->Behavior(Random_);
        ImGui::EndPopup();
      }

      int time = int(particles->Time());
      ImGui::InputInt("Reset\nTime", &time);
      particles->Time(secs(time));

      float size = particles->Size();
      ImGui::InputFloat("Size", &size);
      particles->Size(size);

      int num = int(particles->MaxParticles());
      ImGui::InputInt("Num of\nParticles", &num);
      particles->MaxParticles(num);

      int rad = particles->Radius();
      ImGui::InputInt("Radius", &rad);
      particles->Radius(rad);

      Vec2D offset = particles->Offset();
      glm::vec2 temp(offset.X(), offset.Y());
      ImGui::InputFloat2("Offset\nFrom\nCenter", &temp[0], 2);
      particles->Offset(temp);

      glm::vec4 color = particles->Color();
      ImGui::ColorEdit4("Color", &color[0] , ImGuiColorEditFlags_AlphaBar);
      particles->Color(color);

      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiSpriteMenu(Sprite* sprite, headerType type)
{
  if (sprite)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Sprite");
    else if (type == Tree_)
      open = ImGui::TreeNode("Sprite");
    if (open)
    {
      //std::string name = sprite->GetSpriteSource()->Name();
      //strcpy(buffer, name.c_str());
      glm::vec4 RGBA = sprite->GetRGBA();
      bool isvisible = sprite->GetVisible();
      ImGui::ColorPicker4("RGBA", &RGBA[0], ImGuiColorEditFlags_AlphaBar);
      ImGui::Checkbox("visible", &isvisible);
      //ImGui::InputText("Image Name", buffer, 1000000);
      //sprite->GetSpriteSource()->Name(buffer);
      sprite->SetVisible(isvisible);
      sprite->SetRGBA(RGBA);
      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiAnimationMenu(Animation* animation, headerType type)
{
  if (animation)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Animation");
    else if (type == Tree_)
      open = ImGui::TreeNode("Animation");
    if (open)
    {
      bool isAnimating = animation->GetAnimating();
      float animationSpeed = animation->GetFrameDur();
      ImGui::Checkbox("Animating", &isAnimating);
      ImGui::SliderFloat("Animation\nSpeed", &animationSpeed, 1.f, 60.0f);
      animation->SetAnimating(isAnimating);
      animation->SetFrameDur(animationSpeed);
      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiPhysicsMenu(Physics* physics, headerType type)
{
  if (physics)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Physcis");
    else if (type == Tree_)
      open = ImGui::TreeNode("Physics");
    if (open)
    {
      glm::vec2 velocity = physics->GlmVelocity();
      ImGui::Text("Speed:");
      ImGui::InputFloat("Speed X", &velocity[0], 0.0f, 1000.f, 3);
      ImGui::InputFloat("Speed Y", &velocity[1], 0.0f, 1000.f, 3);
      physics->Velocity(velocity);
      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiTransformMenu(Transform* transform, headerType type)
{
  if (transform)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Transform");
    else if (type == Tree_)
      open = ImGui::TreeNode("Transform");
    if (open)
    {
      glm::vec2 Translation = transform->glmTranslation();
      //glm::vec2 size = transform->glmScale();
	  static float scale = 0.0f;
	  if (scale_changed)
	  {
		  scale_changed = false;
		  scale = transform->MultScale();
	  }

      ImGui::Text(" Translation:");
      ImGui::InputFloat("Loc X", &Translation[0], 0.0f, 1000.f, 3);
      ImGui::InputFloat("Loc Y", &Translation[1], 0.0f, 1000.f, 3);
	  ImGui::InputFloat(" MultiScale", &scale, 0.0f, 1000.f, 3);
      transform->Translation(Translation);

	  if (ImGui::Button("APPLY MULTSCALE"))
	  {
		transform->MultScale(scale);
	  }
	  
      //transform->Scale(size.x, size.y);
      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiBehaviorMenu(Behavior* behavior, headerType type)
{
  if (behavior)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Behavior");
    else if (type == Tree_)
      open = ImGui::TreeNode("Behavior");
    if (open)
    {
      SlowChase* temp = static_cast<SlowChase*>(behavior);
      ImGui::Text(" Behaviors:");
      ImGui::InputFloat("Speed", &temp->speed, 1.f, 1.f, 6);
	  // see if there's a behavior script to this behavior
	  BehaviorScript* script = reinterpret_cast<BehaviorScript*>(behavior);
	  // If it's a door behavior, Do Door stuff!!
	  if (script && script->Name() == "Door")
	  {
		  const char* items[] = { "Open", "Locked", "Unlocked", "ItsATrap" };
		  const char* directions[] = {"North", "South", "East", "West"};
		  // Char buffers for ID/Area Strings
		  static char IDbuffer[200] = "";
		  static char AREAbuffer[200] = "";
		  static int doorType = 0;
		  static int direction = 0;
		  static const char* current_item = items[0];
		  // Cast the behavior script as a door
		  Door* door = reinterpret_cast<Door*>(script);

		  // If the door has changed
		  if (door_changed)
		  {
			  // Get ID
			  const char* char_name = door->ID.c_str();
			  strcpy_s(IDbuffer, char_name);
			  // Get AREA
			  const char* AREA = door->area.c_str();
			  strcpy_s(AREAbuffer, AREA);
			  // Get Type
			  doorType = door->StartState();
			  // Get direction
			  direction = door->wall;
			  // This door is updated, set bool to false
			  door_changed = false;
		  }

		  // ImGui InputTexts
		  ImGui::InputText("AREA", AREAbuffer, 200);
		  ImGui::InputText("ID", IDbuffer,200);
		  // Door Type
		  ImGui::Combo("State ", &doorType, items, IM_ARRAYSIZE(items));
		  // Direction
		  ImGui::Combo("Wall", &direction, directions, IM_ARRAYSIZE(directions));
		  // Save the Door stats
		  ImGui::TextWrapped("Remember: save the room this door is in!");
		  if (ImGui::Button("SAVE DOOR"))
		  {
			  //Assign the New ID/AREA
			  door->ID = IDbuffer;
			  door->area = AREAbuffer;
			  door->StartState( ( (dState)(doorType) ) );
			  door->wall = (ScreenManager::direction)direction;
			  // The door has now changed
			  door_changed = true;
		  }
	  }
      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiHealthMenu(Health* health, headerType type)
{
  if (health)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Health");
    else if (type == Tree_)
      open = ImGui::TreeNode("Health");
    if (open)
    {
      int Max = health->MaxHP();
      int current = health->HitPoints();
      ImGui::InputInt("Max HP", &Max, 1, 1, 0);
      ImGui::InputInt("Current HP", &current, 1, 1, 0);
      health->MaxHP(Max);
      health->HitPoints(current);
      if (type == Tree_)
        ImGui::TreePop();
    }
  }
}

void ImGuiAudioMenu(Audio* audio, headerType type)
{
  if (audio)
  {
    bool open;
    if (type == Collapsing_)
      open = ImGui::CollapsingHeader("Audio");
    else if (type == Tree_)
      open = ImGui::TreeNode("Audio");
    if (open)
    {
      //std::string file = audio->GetAudioFile();
      //char buffer[200];
      //float vol = audio->Volume();
      //strcpy_s(buffer, file.c_str());
      //ImGui::InputText("File Name", buffer, 200);
      //ImGui::SliderFloat("Volume", &vol, 0.0f, 100.0f);
      //audio->Volume(vol);
      //if (type == Tree_)
      //  ImGui::TreePop();
    }
  }
}

void ImGuiKeyMenu(Key* key, headerType type)
{
	// static buffer for Key's ID
	static char IDbuffer[200] = "";
	// if there is a key component
	if (key)
	{
		// open a window depending on the type given
		bool open;
		if (type == Collapsing_)
			open = ImGui::CollapsingHeader("Key");
		else if (type == Tree_)
			open = ImGui::TreeNode("Key");
		// if the window is open
		if (open)
		{
			// if the Key's been changed
			if (key_changed)
			{
				// Get ID
				std::string ID_str = key->ID();
				const char* ID = ID_str.c_str();
				strcpy_s(IDbuffer, ID);
				// This door is updated, set bool to false
				key_changed = false;
			}

			// Key ID
			ImGui::InputText("Key ID", IDbuffer, 200);

			ImGui::TextWrapped("Remember: save the room this key is in!");
			if (ImGui::Button("SAVE KEY"))
			{
				//Assign the New ID
				key->ID(IDbuffer);
				// The door has now changed
				key_changed = true;
			}

			// End the Tree if we've opened with type Tree_
			if (type == Tree_)
				ImGui::TreePop();
		}
	}
}

void ImGuiComponentMenu(Component* comp)
{
  if (comp)
  {
    switch (comp->Type())
    {
    case Component::Animation_:
      ImGuiAnimationMenu(static_cast<Animation*>(comp), Collapsing_);
      break;
    
    case Component::ParticleEmitter_:
      ImGuiParticleMenu(static_cast<ParticleEmitter*>(comp), Collapsing_);
      break;

    case Component::Transform_:
      ImGuiTransformMenu(static_cast<Transform*>(comp), Collapsing_);
      break;

    case Component::Physics_:
      ImGuiPhysicsMenu(static_cast<Physics*>(comp), Collapsing_);
      break;

    case Component::Behavior_:
      ImGuiBehaviorMenu(static_cast<Behavior*>(comp), Collapsing_);
      break;

      /*case Component::Collider_:
      ImGuiColl(static_cast<Collider*>(elem));
      break;*/

    case Component::Health_:
      ImGuiHealthMenu(static_cast<Health*>(comp), Collapsing_);
      break;

    case Component::Sprite_:
      ImGuiSpriteMenu(static_cast<Sprite*>(comp), Collapsing_);
      break;

    case Component::Audio_:
      ImGuiAudioMenu(static_cast<Audio*>(comp), Collapsing_);
      break;
    }
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::SetTooltip("Drag this componentnselected object's Menu\n to add it to the object");
      ImGui::EndTooltip();
    }
  }
}

void ImGuiSetPay(Component* comp)
{
  if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
  {
    switch (comp->Type())
    {
    case Component::Animation_:
      ImGui::SetDragDropPayload("Comp", new Animation(comp), sizeof(Animation));
      ImGui::CollapsingHeader("Animation");
      break;
    
    case Component::ParticleEmitter_:
      ImGui::SetDragDropPayload("Comp", new ParticleEmitter(comp), sizeof(ParticleEmitter));
      ImGui::CollapsingHeader("ParticleEmitter");
      break;

    case Component::Transform_:
      ImGui::SetDragDropPayload("Comp", new Transform(comp), sizeof(Transform));
      ImGui::CollapsingHeader("Transform");

      break;

    case Component::Physics_:
      ImGui::SetDragDropPayload("Comp", new Physics(comp), sizeof(Physics));
      ImGui::CollapsingHeader("Physics");
      break;

    case Component::Behavior_:
      ImGui::SetDragDropPayload("Comp", new Behavior(comp), sizeof(Behavior));
      ImGui::CollapsingHeader("Behavior");
      break;

      /*case Component::Collider_:
      ImGuiColl(static_cast<Collider*>(elem));
      break;*/

    case Component::Health_:
      ImGui::SetDragDropPayload("Comp", new Health(comp), sizeof(Health));
      ImGui::CollapsingHeader("Health");
      break;

    case Component::Sprite_:
      ImGui::SetDragDropPayload("Comp", new Sprite(comp), sizeof(Sprite));
      ImGui::CollapsingHeader("Sprite");
      break;

    case Component::Audio_:
      ImGui::SetDragDropPayload("Comp", new Audio(comp), sizeof(Audio));
      ImGui::CollapsingHeader("Audio");
      break;
    }
    //ImGui::CollapsingHeader("test");
    ImGui::EndDragDropSource();
  }
}

void ImGuiColliderMenu(Collider * col, headerType type)
{
	if (col)
	{
		ColliderCircle* circle = dynamic_cast<ColliderCircle*>(col);
		ColliderSquare* square = dynamic_cast<ColliderSquare*>(col);
		bool open;
		if (type == Collapsing_)
			open = ImGui::CollapsingHeader("Collider");
		else if (type == Tree_)
			open = ImGui::TreeNode("Collider");
		if (open)
		{
			if (square)
			{
				Vec2D dim = square->Dimensions();
				float x = dim.X();
				float y = dim.Y();
				ImGui::InputFloat("Width", &x, 1, 1, 0);
				ImGui::InputFloat("Height", &y, 1, 1, 0);
				square->Dimensions(Vec2D(x, y));
			}
			if (type == Tree_)
				ImGui::TreePop();
		}
	}
}

Editor::Editor() : System("Editor")
{  
}

void Editor::Initialize()
{
  GLFWwindow* window = Graphics::Window();
  InitImGui(window);
  GSM = dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"));

  archList = &Space::GetArchetypeList();
  CreateComps();
}

void Editor::Update(float dt)
{
  //Draw();
  dt_ = dt;
}

Editor::~Editor()
{
  ImGuiCleanUp();
  DeleteComps();
}

void Editor::SetSelectedObject()
{
  if (ImGui::GetIO().WantCaptureMouse)
  {
    return;
  }
  double cursorX;
  double cursorY;
  //Detect if mouse cursor is on an object
  glfwGetCursorPos(Graphics::Window(), &cursorX, &cursorY);

  ////////////////////////////////////////////////////////////////
  //Convert mouse coordinates from screen to world
  glm::vec2 mouseVec;
  mouseVec.x = (float)cursorX;
  mouseVec.y = (float)cursorY;
  mouseVec = Graphics::ScreenToWorld(mouseVec);
  selectedObj_ = Input::CheckMouse(mouseVec);
}

void Editor::UpdatePos() 
{
  if (ImGui::GetIO().WantCaptureMouse)
  {
    return;
  }
  double cursorX;
  double cursorY;
  //Detect if mouse cursor is on an object
  glfwGetCursorPos(Graphics::Window(), &cursorX, &cursorY);

  ////////////////////////////////////////////////////////////////
  //Convert mouse coordinates from screen to world
  glm::vec2 mouseVec;
  mouseVec.x = (float)cursorX;
  mouseVec.y = (float)cursorY;
  mouseVec = Graphics::ScreenToWorld(mouseVec);
  if (selectedObj_)
  {
    if (!ImGui::GetIO().WantCaptureMouse)
    {
      if (glfwGetMouseButton(Graphics::Window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
      {
        //2q  if(int(mouseVec.x) % gridSnap_ == 0 && int(mouseVec.y) % gridSnap_)
        selectedObj_->Has(Transform)->Translation(mouseVec);
        selectedObj_->Has(Transform)->OGTrans(mouseVec);
      }
    }
  }
}

float Editor::MainMenu()
{
  float height = 0.f;
  if (ImGui::BeginMainMenuBar())
  {
    height = ImGui::GetWindowHeight();
    SettingMenu();
    if (ImGui::BeginMenu("Windows"))
    {
      ImGui::Checkbox("Settings", &settingsMenuActive_);
      ImGui::Checkbox("Menu", &objectMenuActive_);
      ImGui::Checkbox("ArcheTypeMenu", &archeTypeMenuActive_);
	    ImGui::Checkbox("ScreenEditor", &screenEditorActive_);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  return height;
}

void Editor::Draw()
{
  NewFrame();
  if (editorActive_)
  {
    //std::string test = "SQUEEP!";
    static_cast<Input*>(EngineGetSystem("Input"))->IsActive(false);
    UpdatePos();
    //NewFrame();
    float offsetY = MainMenu();
    ObjectEditor(offsetY);
    TilesMenu(offsetY);
    ScreenEditor(offsetY);
    //PopupMenu(test, nullptr, 200, 50);
    //ImGui::ShowDemoWindow();

    //ImGui::ShowStyleEditor();
    DeleteObj();
    Save();
    //Render();
  }
  else if (MenuIsOpen())
  {
    static_cast<Input*>(EngineGetSystem("Input"))->IsActive(false);
    UpdatePos();
    GameMenu();
  }
  else
    static_cast<Input*>(EngineGetSystem("Input"))->IsActive(true);

  DrawPopups();

  Render();
}

void Editor::Copy()
{
  if (selectedObj_)
  {
    clipBoard_ = &selectedObj_->Type();
  }
}

void Editor::Paste()
{
  if (clipBoard_)
  {
    reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentScreen()->AddNewObject(*clipBoard_);
  }
}

void Editor::SettingMenu()
{
  if (editorActive_)
  {
    if (settingsMenuActive_)
    {
      if (ImGui::BeginMenu("Settings"))
      {
        if (ImGui::TreeNode("Theme"))
        {
          if (ImGui::Button("Dark"))
          {
            ImGui::StyleColorsDark();
          }
          if (ImGui::Button("Light"))
          {
            ImGui::StyleColorsLight();
          }
          if (ImGui::Button("Clasic"))
          {
            ImGui::StyleColorsClassic();
          }
          ImGui::TreePop();
        }
        if (ImGui::TreeNode("Display Mode"))
        {
          if (ImGui::Button("FullScreen"))
          {
            Graphics::SetWindowMode(FullScreen);
          }
          if (ImGui::Button("Windowed"))
          {
            Graphics::SetWindowMode(Maximized);
          }
          if (ImGui::Button("Secret"))
          {
            Graphics::SetWindowMode(Secret);
          }
          ImGui::TreePop();
        }
        ImGui::EndMenu();
      }
    }
  }
}

void Editor::ComponentsMenu(float offsetY)
{
  for (auto elem : components_)
  {
    if (elem)
    {
      int width;
      int height;
      glfwGetFramebufferSize(Graphics::Window(), &width, &height);
      ImGui::SetNextWindowPos(ImVec2(width - width / 8.f, offsetY));
      if (selectedObj_)
      {
        ImGui::SetNextWindowSize(ImVec2(width / 8.f, height / 2.f));
      }
      else
      {
        ImGui::SetNextWindowSize(ImVec2(width / 8.f, height - offsetY));
      }
      if (ImGui::Begin("Components List"))
      {
        ImGuiComponentMenu(elem);
        ImGuiSetPay(elem);
      }
      ImGui::End();
    }
  }
}

void Editor::SelectedObjectMenu(float offsetY)
{
  if (selectedObj_)
  {
    int width;
    int height;
    glfwGetFramebufferSize(Graphics::Window(), &width, &height);
    ImGui::SetNextWindowSize(ImVec2(width / 8.f, height / 2.f - offsetY));
    ImGui::SetNextWindowPos(ImVec2(width - width / 8.f, (height / 2.f) + offsetY));
    if (ImGui::Begin("Selected Object"))
    {
      const ImGuiPayload* payLoad = ImGuiObjectMenu(selectedObj_);
      if (payLoad)
      {
        void* data = payLoad->Data;
        Component::compType type = static_cast<Component*>(data)->Type();
        switch (type)
        {
        case Component::Animation_:
          selectedObj_->Add(new Animation(static_cast<Animation*>(data)));
          break;

        case Component::ParticleEmitter_:
          selectedObj_->Add(new ParticleEmitter(static_cast<ParticleEmitter*>(data)));
          break;

        case Component::Transform_:
          selectedObj_->Add(new Transform(static_cast<Transform*>(data)));
          break;

        case Component::Physics_:
          selectedObj_->Add(new Physics(static_cast<Physics*>(data)));
          break;

        case Component::Behavior_:
          selectedObj_->Add(new Behavior(static_cast<Behavior*>(data)));
          break;
		    case Component::Key_:
		      selectedObj_->Add(new Behavior(static_cast<Key*>(data)));
		      break;
          /*case Component::Collider_:
          ImGuiColl(static_cast<Collider*>(elem));
          break;*/
        case Component::Health_:
          selectedObj_->Add(new Health(static_cast<Health*>(data)));
          break;

        case Component::Sprite_:
          selectedObj_->Add(new Sprite(static_cast<Sprite*>(data)));
          break;

        case Component::Audio_:
          selectedObj_->Add(new Audio(static_cast<Audio*>(data)));
          break;
        }
      }
    }
    ImGui::End();
  }
}

void Editor::TilesMenu(float offsetY)
{
  if (objectMenuActive_)
  {
    int width;
    int height;
    glfwGetFramebufferSize(Graphics::Window(), &width, &height);
    ImGui::SetNextWindowPos(ImVec2(0, offsetY));
    ImGui::SetNextWindowSize(ImVec2(width / 8.f, height - offsetY));
    if (ImGui::Begin("Archetypes"))
    {
      int i = 0;
      for (auto elem : *archList)
      {
		  Sprite* spritz = elem.second->Has(Sprite);
		  SpriteSource* source = spritz->GetSpriteSource();
        ImTextureID id = (void*)source->GetId();
		glm::vec4 tColor = spritz->GetRGBA();
		glm::vec2 uv = source->GetUV(0);
		ImVec4 color(tColor.r, tColor.g, tColor.b, tColor.a);
        if (i % 2 != 0)
        {
          ImGui::SameLine();
        }
        if (ImGui::ImageButton(id, ImVec2(100, 100), ImVec2(0,0), ImVec2(uv.y, uv.y),-1, ImVec4(0,0,0,0), color))
        {
          reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentScreen()->AddNewObject(elem.second->Name());
        }
        i++;
      }
    }
    ImGui::End();
  }
}

void Editor::ObjectEditor(float offsetY)
{
  if (objectMenuActive_)
  {
    ComponentsMenu(offsetY);
    SelectedObjectMenu(offsetY);
  }
}

void Editor::ScreenEditor(float offsetY)
{
	if (screenEditorActive_)
	{
		// Flags for this menu 
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoResize;

		// Keep the window open
		bool* p_open = NULL;

		// Specify the Window Position and Size 
		ImGui::SetNextWindowPos(ImVec2(800, 280), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(420, 420), ImGuiCond_FirstUseEver);

		// Get Screen Manager 
		ScreenManager* SM = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

		// Current Screen 
		Screen* current = SM->CurrentScreen();

		// Current area, converted from Screen ptr 
		// (We check for the type before using area methods 
		Area* current_area = (Area*)(current);

		// Main Body of the Menu starts here 
		if (!ImGui::Begin("Screen Menu", p_open, flags))
		{
			// Early out if the window is collapsed, as an optimization. 
			ImGui::End();
			return;
		}

		if (ImGui::CollapsingHeader("Editor"))
		{
			// Get Current Screen name and type 
			static char name_buffer[64] = "";

			// Background Name Buffer 
			static char background_buffer[64] = "";

			// If the screen has been changed, get it and copy into the name buffer 
			if (has_changed)
			{
				// Get name 
				std::string screen_name = current->GetName();
				const char* char_name = screen_name.c_str();
				strcpy_s(name_buffer, char_name);
				// Get background
				std::string screen_background = current->GetBackground();
				const char* background = screen_background.c_str();
				strcpy_s(background_buffer, background);
				has_changed = false;
			}
			ImGui::InputText("Screen Name", name_buffer, 64);

			// Reset Options
			if (current_area != NULL)
			{
				ImGui::Indent(15.0f);
				int areas_connected = 0;

				const char* items[] = { "All", "Enemy", "Puzzle" };
				ImGui::Combo("", &type_index, items, IM_ARRAYSIZE(items));

				// Reset Button 
				ImGui::SameLine();
				if (ImGui::Button("RESET"))
				{
					// Reset the current screen 
					switch (type_index)
					{
						// Reset All 
						case 0:
						{
							current_area->EnemyReset();
							current_area->PuzzleReset();
							break;
						}
						// Reset Enemies 
						case 1:
						{
							current_area->EnemyReset();
							break;
						}
						// Reset Puzzles 
						case 2:
						{
							current_area->PuzzleReset();
							break;
						}
						default:
						{
							break;
						}
					}
				}
				// Change Background 
				ImGui::Indent(-15.0f);
				ImGui::InputText("Background", background_buffer, 64);
				ImGui::Text("Current Start Screen: %s", (SM->StartScreen())->GetName().c_str());
				// Change the current screen to start screen
				if (ImGui::Button("Set Current to Start"))
				{
					SM->StartScreen(current);
				}
				ImGui::Indent(15.0f);

				// Add New Screen Buttons 
				ImGui::Separator();
				// Check each of the areas, increment areas connected if there's an area 
				if (current_area->GetNorthArea() != "") { ++areas_connected; }
				if (current_area->GetSouthArea() != "") { ++areas_connected; }
				if (current_area->GetEastArea() != "") { ++areas_connected; }
				if (current_area->GetWestArea() != "") { ++areas_connected; }

				// Tell how many rooms area connnected
				ImGui::Text("Rooms Connected: %i", areas_connected);
				ImGui::Indent(20.0f);
				// Show name of the room attached, if there is one
				ImGui::Text("North Room: %s", current_area->GetNorthArea().c_str());
				ImGui::Text("South Room: %s", current_area->GetSouthArea().c_str());
				ImGui::Text("East Room: %s", current_area->GetEastArea().c_str());
				ImGui::Text("West Room: %s", current_area->GetWestArea().c_str());

				ImGui::Indent(-30.0f);
				ImGui::Separator();
				ImGui::Indent(10.0f);
				// Add A room to the north of the current one
				if (ImGui::Button("Add North"))
				{
					// Generic Name for the room
					std::string name = "NORTH_AREA_";
					// Add the Screen's ID to the end, making the name unique
					name.push_back(current->GetID());
					// Create a new area, and attach the current area to it 
					(SM->CreateNewArea(name, ScreenManager::North_));
				}
				// The process is repeated for other directions
				if (ImGui::Button("Add South"))
				{
					std::string name = "SOUTH_AREA_";
					name.push_back(current->GetID());
					// Create a new area, and attach the current area to it 
					(SM->CreateNewArea(name, ScreenManager::South_));
				}

				if (ImGui::Button("Add East"))
				{
					std::string name = "EAST_AREA_";
					name.push_back(current->GetID());
					// Create a new area, and attach the current area to it 
					(SM->CreateNewArea(name, ScreenManager::East_));
				}

				if (ImGui::Button("Add West"))
				{
					std::string name = "WEST_AREA_";
					name.push_back(current->GetID());
					// Create a new area, and attach the current area to it 
					(SM->CreateNewArea(name, ScreenManager::West_));
				}
				ImGui::Indent(-10.0f);
			}

			// Save 
			if (ImGui::Button("SAVE"))
			{
				// Mark that the screen has changed 
				has_changed = true;
				// save new name
				current->SetName(name_buffer);
				// Save new background
				current->SetBackground(background_buffer);
				// Finally, Call Data System to save the screen
				reinterpret_cast<Data*>(EngineGetSystem("Data"))->SaveScreen(current);
			}
			ImGui::SameLine();
			ImGui::Text("Changes will not save otherwise!");
		}

		// Screen Navigator: move to other areas. 
		if (ImGui::CollapsingHeader("Navigator"))
		{
			if (current_area != NULL)
			{
				ImGui::Text("ROOMS");
				ImGui::Separator();
				std::string name;
				// Get Other areas
				// Get North Room
				ImGui::Text("North Room: ");
				// If there's a north room, print it
				if (current_area->GetNorthArea() != "")
				{
					// Make sure the name is printed on the same line
					ImGui::SameLine();
					// Get the North area's nanme
					name = current_area->GetNorthArea();
					ImGui::Text("%s", name.c_str());
					// On the same line, give option to move in that direction
					ImGui::SameLine();
					if (ImGui::Button("Move North"))
					{
						SM->ChangeCurrentScreen(name);
						has_changed = true;
					}
				}
				// Process repeated for other cardinal direction rooms
				// Get South Room
				ImGui::Text("South Room: ");
				if (current_area->GetSouthArea() != "")
				{
					ImGui::SameLine();
					name = current_area->GetSouthArea();
					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();
					if (ImGui::Button("Move South"))
					{
						SM->ChangeCurrentScreen(name);
						has_changed = true;
					}
				}
				// Get East Room
				ImGui::Text("East Room: ");
				if (current_area->GetEastArea() != "")
				{
					ImGui::SameLine();
					name = current_area->GetEastArea();
					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();
					if (ImGui::Button("Move East"))
					{
						SM->ChangeCurrentScreen(name);
						has_changed = true;
					}
				}
				// Get West Room
				ImGui::Text("West Room: ");
				if (current_area->GetWestArea() != "")
				{
					ImGui::SameLine();
					name = current_area->GetWestArea();
					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();
					if (ImGui::Button("Move West"))
					{
						SM->ChangeCurrentScreen(name);
						has_changed = true;
					}
				}
			}
			else
			{
				// Area Pointer is NULL; Screen is not considered an area
				ImGui::Text("This Screen is Not considered an Area. You can't navigate from here.");
			}
		}
		ImGui::End();
	}
}
void Editor::NewFrame()
{
  //if (editorActive_ || MenuIsOpen())
  //{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  //}
}

void Editor::Render()
{
  //if (editorActive_ || MenuIsOpen())
  //{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  //}
}

void Editor::ObjectMenu(std::vector<Obj*> objects)
{
  if (editorActive_)
  {
    if (&objects)
    {
      if (ImGui::CollapsingHeader(std::string("Object List").c_str()))
      {
        for (unsigned int i = 0; i < objects.size(); ++i)
        {
          ImGuiObjectMenu(objects[i]);
        }
      }
    }
  }
}

void Editor::ArchetypeMenu(std::vector<Obj*> objects)
{
  if (editorActive_)
  {
    if (&objects)
    {
      if (ImGui::CollapsingHeader(std::string("ArcheyType List").c_str()))
      {
        for (unsigned int i = 0; i < objects.size(); ++i)
        {
          ImGuiObjectMenu(objects[i]);
        }
      }
    }
  }
}

void Editor::CreateComps()
{
  Audio* aud = new Audio();
  components_.push_back(aud);
  Health* health = new Health(0);
  components_.push_back(health);
  Physics* phys = new Physics();
  components_.push_back(phys);
  Sprite* sprite = new Sprite("Deafult/Path", "Deafult/Path");
  components_.push_back(sprite);
  Transform* trans = new Transform(Vec2D(0, 0));
  components_.push_back(trans);
  Animation* anim = new Animation("Deafult Name");
  components_.push_back(anim);
  Behavior* behav = new Behavior();
  components_.push_back(behav);
  Collider* coll = new Collider();
  components_.push_back(coll);
  ParticleEmitter* emit = new ParticleEmitter();
  components_.push_back(emit);
}

void Editor::DeleteComps()
{
  for (auto elem : components_)
  {
    delete elem;
  }
}

void Editor::SetActive(bool active)
{
  editorActive_ = active;
}
bool Editor::GetActive()
{
  return editorActive_;
}

bool Editor::Paused()
{
  if (editorActive_)
    return true;
  else
    return false;
}

void Editor::Save()
{
	if (save_)
	{
		ImGui::OpenPopup("Save?");
	}
	if (ImGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button("Yes"))
		{
			Data* data = reinterpret_cast<Data*>(EngineGetSystem("Data"));
			data->SaveScreen(reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentScreen());
			/*ArchetypeList alist = Space::GetArchetypeList();
			for (std::pair<std::string, Obj*> obj : alist)
			{
				data->SaveArchetype(*obj.second);
			}*/
			save_ = false;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Cancel"))
		{

			save_ = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Editor::DeleteObj()
{
  if (selectedObj_)
  {
	  if (delete_)
	  {
		  ImGui::OpenPopup("Delete?");
	  }
	  if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	  {
		  if (ImGui::Button("Yes"))
		  {
			  delete_ = false;
			  reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentScreen()->RemoveObject(selectedObj_);
			  ObjectList objs = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentScreen()->GetObjectList();
			  int size = objs.size();
			  for (int i = 0; i < size; ++i)
			  {
				  if (objs[i]->Name() == selectedObj_->Name())
				  {
					  delete objs[i];
					  selectedObj_ = nullptr;
				  }
			  }
			  ImGui::CloseCurrentPopup();
		  }
		  if (ImGui::Button("Cancel"))
		  {

			  delete_ = false;
			  ImGui::CloseCurrentPopup();
		  }
		  ImGui::EndPopup();
	  }
  }
}

void Editor::AddPopup(PopupParam& popup)
{
	Popups.push_back(popup);
}

///////////////PRIVATE FUNCTIONS///////////////

void Editor::DrawPopups(void)
{
	std::vector<PopupParam>::iterator it;
	for (it = Popups.begin(); it != Popups.end(); ++it)
	{
		// Get a Popup from the vector
		PopupParam& params = (*it);

		// Draw the Popup
		PopupMenu(params.text, params.obj, params.length, params.height);

		// Reduce time for popup on screen
		params.time -= dt_;

		// once time has hit 0, 
		if (params.time <= 0.0f)
		{
			Popups.erase(it);
			it = Popups.begin();
			if (Popups.size() == 0)
				break;
		}
	}
}