#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"

// Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Editor");
}

// Destructor
ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{	
	if (App->window->window != NULL)
	{
		// Setup ImGui binding
		DEBUG("Setup ImGUI binding");
		ImGui_ImplSdlGL3_Init(App->window->window);
	}		

	return true;
}

UPDATE_STATUS ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::Update(float dt)
{

	ShowMenuBar();

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

void ModuleEditor::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		ShowMenuFile();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		ImGui::MenuItem("About", NULL, &about_menu);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if(about_menu)
		ShowAboutMenu();
}

void ModuleEditor::ShowMenuFile()
{
	if (ImGui::MenuItem("Quit", "ESC")) { App->CloseApp(); }	
}

void ModuleEditor::ShowAboutMenu()
{
	ImGui::Begin("About", &about_menu, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("dear imgui, %s", ImGui::GetVersion());
	ImGui::Separator();
	ImGui::Text("By Omar Cornut and all github contributors.");
	ImGui::Text("ImGui is licensed under the MIT License, see LICENSE for more information.");
	ImGui::End();
}
