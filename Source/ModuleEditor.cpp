#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"

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
		App->console.AddLOG("Setup ImGUI binding");
		ImGui_ImplSdlGL3_Init(App->window->window);
	}		

	return true;
}

UPDATE_STATUS ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		conf_menu = !conf_menu;

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
		console_menu = !console_menu;
	
	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::Update(float dt)
{
	ShowMenuBar();
	//ImGui::ShowTestWindow();

	if (about_menu) ShowAboutMenu();
	if (conf_menu) ShowConfMenu();
	if (console_menu) ShowConsole();

	return UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEditor::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	App->console.AddLOG("Closing ImGUI");
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

void ModuleEditor::ShowAboutMenu()
{
	ImGui::Begin("About", &about_menu, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("dear imgui, %s", ImGui::GetVersion());
	ImGui::Separator();
	ImGui::Text("By Omar Cornut and all github contributors.");
	ImGui::Text("ImGui is licensed under the MIT License, see LICENSE for more information.");
	ImGui::End();
}

void ModuleEditor::ShowConfMenu()
{
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Configuration", &conf_menu);//ImGuiWindowFlags_AlwaysAutoResize
	if(ImGui::CollapsingHeader("Application"))
	{
		// App name and organization
		//const char *name = App->GetAppName();
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
	App->console.Draw("Console", &console_menu);
}
