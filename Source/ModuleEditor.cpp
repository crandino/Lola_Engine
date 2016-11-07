#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleGameObjectManager.h"

#include "GameObject.h"
#include "Component.h"

#include "DrawDebug.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"

// Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Editor");
}

// Destructor
ModuleEditor::~ModuleEditor()
{ }

bool ModuleEditor::Awake(JSONParser &config)
{	
	if (App->window->window != NULL)
	{
		// Setup ImGui binding
		DEBUG("Setup ImGUI binding");
		ImGui_ImplSdlGL3_Init(App->window->window);
	}		

	node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	leaf_flags = node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	item_selected_by_id = -1;

	return true;
}

UPDATE_STATUS ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
		console_menu = !console_menu;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		hierarchy_menu = !hierarchy_menu;

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		conf_menu = !conf_menu;

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN && go_selected)
		warning_alert = true;
	
	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::Update(float dt)
{
	ShowMenuBar();
	//ImGui::ShowTestWindow();

	if (about_menu) ShowAboutMenu();
	if (conf_menu) ShowConfMenu();
	if (console_menu) ShowConsole();
	if (hierarchy_menu) ShowHierarchy();
	if (warning_alert) ShowWarning();

	ShowComponentInfo();

	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	DEBUG("Closing ImGUI");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

// Showing Windows
void ModuleEditor::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		ShowMenuFile();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Create"))
	{
		ShowCreateMenu();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Debug"))
	{	
		ImGui::MenuItem("Show AABB", nullptr, &DrawDebug::show_aabb);
		ImGui::MenuItem("Show Frustum", nullptr, &DrawDebug::show_frustum);
		ImGui::MenuItem("Show OcTree", nullptr, &DrawDebug::show_octree);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		ImGui::MenuItem("About", NULL, &about_menu);
		if (ImGui::MenuItem("GitHub Wiki"))
			App->RequestBrowser("https://github.com/crandino/Lola_Engine/wiki");
		ImGui::EndMenu();
	}	

	ImGui::EndMainMenuBar();
}

void ModuleEditor::ShowMenuFile()
{
	if (ImGui::MenuItem("Quit", "ESC")) { App->CloseApp(); }	
}

void ModuleEditor::ShowCreateMenu()
{
	if (ImGui::MenuItem("Camera")) { App->gameobject_manager->CreateCamera(); }
}

void ModuleEditor::ShowAboutMenu()
{
	ImGui::Begin("About", &about_menu, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("LOLA Engine");
	ImGui::Separator();
	ImGui::Text("3D Engine created and developed by Carlos Randino (crandino on GitHub) in VideoGame");
	ImGui::Text("Engines Subject on CITM (Terrassa). This engine is hosted on a GitHub repository");
	ImGui::Text("(available on Help/GitHub Wiki menu). LOLA Engine is under a MIT License.");
	ImGui::End();
}

void ModuleEditor::ShowConfMenu()
{
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Configuration", &conf_menu);//ImGuiWindowFlags_AlwaysAutoResize
	if(ImGui::CollapsingHeader("Application"))
	{
		// App name and organization
		ImGui::InputText("App name", (char*)App->GetAppName(), 128);
		ImGui::InputText("Organization", (char*)App->GetOrganization(), 128);
		ImGui::Separator();

		// FPS and dt histograms
		char title[25];
		ImGui::SliderInt("Max FPS", &App->perf_info.getFrameRate(), 0, 75);
		sprintf_s(title, 25, "Framerate %.1f", App->perf_info.getFrameRateHist()[HISTOGRAM_VALUES - 1]);
		ImGui::PlotHistogram("##framerate", App->perf_info.getFrameRateHist(), HISTOGRAM_VALUES, 0, title, 0.0f, 75.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Miliseconds %.1f", App->perf_info.getMilisecondsHist()[HISTOGRAM_VALUES - 1]);
		ImGui::PlotHistogram("##miliseconds", App->perf_info.getMilisecondsHist(), HISTOGRAM_VALUES, 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}	
	if (ImGui::CollapsingHeader("Software & Hardware"))
	{
		ImGui::LabelText("", "%s", "Software");
		ImGui::Separator();

		// SDL
		static SDL_version ver; SDL_GetVersion(&ver);
		ImGui::LabelText("", "%s", "SDL Version:"); ImGui::SameLine();		
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%u.%u.%u", ver.major, ver.minor, ver.patch);

		// OpenGL
		ImGui::LabelText("", "%s", "OpenGL Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", App->renderer3D->gl_version);

		// Glew
		ImGui::LabelText("", "%s", "GLew Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", App->renderer3D->glew_version);

		// Bullet
		ImGui::LabelText("", "%s", "Bullet Version:");// ImGui::SameLine();
		//ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%u.%u", BT_BULLET_VERSION / 100, BT_BULLET_VERSION % 200);

		// ImGUI
		ImGui::LabelText("", "%s", "ImGUI Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", ImGui::GetVersion());

		//MathGeoLib
		ImGui::LabelText("", "%s", "MathGeoLib Version:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s", "1.5");
	}

	ImGui::End();
}

void ModuleEditor::ShowConsole()
{
	App->console->Draw("Console", &console_menu);
}

void ModuleEditor::ShowHierarchy()
{
	ImGui::Begin("Hierarchy", &hierarchy_menu);	
	ExpandTree(App->gameobject_manager->GetRoot());
	ImGui::End();
}

void ModuleEditor::ShowComponentInfo()
{
	if (go_selected != nullptr)
	{
		float width = 350.0f;
		ImGui::SetNextWindowSizeConstraints(ImVec2(width, 0), ImVec2(width, FLT_MAX));
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width, 20.0f));
		ImGui::Begin(go_selected->GetName(), nullptr, ImGuiWindowFlags_ShowBorders);

		if (ImGui::Checkbox("Active", &go_selected->active))
			go_selected->ChangeActiveState(&go_selected->active);
		ImGui::Separator();

		for (uint i = 0; i < go_selected->components.size(); ++i)
			go_selected->components[i]->ShowEditorInfo();			

		ImGui::End();
	}	
}

void ModuleEditor::ShowWarning()
{
	ImGuiWindowFlags window_flags = (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
									 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Warning", nullptr, window_flags);
	ImGui::Text("%s%s%s", "Are you sure you wanna delete ", go_selected->GetName(), " ?");

	ImVec2 button_size = ImVec2(80.0f, 20.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.75f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.75f, 0.0f, 0.9f));
	if (ImGui::Button("Yes", button_size))
	{
		App->gameobject_manager->MarkChildToDelete(go_selected);
		go_selected = nullptr;
		warning_alert = false;
	}
		
	ImGui::PopStyleColor(3);

	ImGui::SameLine(0.0f, 100.0f);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.0f, 0.0f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.75f, 0.0f, 0.0f, 0.9f));
	if (ImGui::Button("No", button_size))
		warning_alert = false;
	ImGui::PopStyleColor(3);

	ImGui::End();
}

// Expand Tree recursively shows all GameObjects on the Hierarchy Window
void ModuleEditor::ExpandTree(const GameObject* go_to_expand)
{
	int num_children = go_to_expand->children.size();
	GameObject *child = nullptr;

	for (int i = 0; i < num_children; ++i)
	{
		child = go_to_expand->children[i];
		node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((child->id == item_selected_by_id) ? ImGuiTreeNodeFlags_Selected : 0);

		if (child->children.size() > 0)
		{
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, child->GetName());

			if (ImGui::IsItemClicked())
				ChangeSelectedGameObject(child);

			if (node_open)
			{
				ExpandTree(child);
				ImGui::TreePop();
			}
		}
		else
		{
			leaf_flags = node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)(intptr_t)i, leaf_flags, child->GetName());
			if (ImGui::IsItemClicked())
				ChangeSelectedGameObject(child);
		}
	}
}

void ModuleEditor::ChangeSelectedGameObject(GameObject *new_go)
{
	if (go_selected)
		go_selected->selected = false;

	go_selected = new_go;
	go_selected->selected = true;
	item_selected_by_id = new_go->id;
}
