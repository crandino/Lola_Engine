#include "ModuleEditor.h"
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
	
	return true;
}

UPDATE_STATUS ModuleEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::Update(float dt)
{
	ImVec4 clear_color = ImColor(114, 144, 154);
	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	/*if (ImGui::Button("Test Window")) show_test_window ^= 1;
	if (ImGui::Button("Another Window")) show_another_window ^= 1;*/
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	return true;
}