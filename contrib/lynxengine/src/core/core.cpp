#include "core.h"

#include <../src/core/time.h>
#include <../src/core/input.h>

#include <../src/graphics/display.h>

// Gameloop Callbacks

void (*Core::InitCallback) (void) = nullptr;
void (*Core::UpdateCallback) (void) = nullptr;
void (*Core::LateUpdateCallback) (void) = nullptr;
void (*Core::DisplayCallback) (void) = nullptr;
void (*Core::OnGUICallback) (void) = nullptr;
void (*Core::TermCallback) (void) = nullptr;

// Gameloop Callback Setup

void Core::SetInitCallback(void (*Init) ()) {
	InitCallback = Init;
	
	Debug::Info("CALLBACK", Init ? "Registered Init Callback" : "Reset Init Callback");
}

void Core::SetUpdateCallback(void (*Update) ()) {
	UpdateCallback = Update;
	
	Debug::Info("CALLBACK", Update ? "Registered Update Callback" : "Reset Update Callback");
}

void Core::SetLateUpdateCallback(void (*LateUpdate) ()) {
	LateUpdateCallback = LateUpdate;
	
	Debug::Info("CALLBACK", LateUpdate ? "Registered LateUpdate Callback" : "Reset LateUpdate Callback");
}

void Core::SetDisplayCallback(void (*Display) ()) {
	DisplayCallback = Display;
	
	Debug::Info("CALLBACK", Display ? "Registered Display Callback" : "Reset Display Callback");
}

void Core::SetOnGUICallback(void (*OnGUI) ()) {
	OnGUICallback = OnGUI;
	
	Debug::Info("CALLBACK", OnGUI ? "Registered OnGUI Callback" : "Reset OnGUI Callback");
}

void Core::SetTermCallback(void (*Term) ()) {
	TermCallback = Term;
	
	Debug::Info("CALLBACK", Term ? "Registered Term Callback" : "Reset Term Callback");
}

void Core::SetCallbacks(void (*Init) (), void (*Update) (), void (*LateUpdate) (), void (*Display) (), void (*OnGUI) (), void (*Term) ()) {
	SetInitCallback(Init);
	SetUpdateCallback(Update);
	SetLateUpdateCallback(LateUpdate);
	SetDisplayCallback(Display);
	SetOnGUICallback(OnGUI);
	SetTermCallback(Term);
}

// Member Functions

void Core::Init() {
	if (!glfwInit()) {
		Debug::Error("CORE", "Failed to initialise GLFW");

		return;
	}

	Debug::Info("CORE", "Initialised GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Do Initialise
	
	if (InitCallback) InitCallback();
	//Scene::Init();
	
	//GUI::Init();
	
	// Do Gameloop
	
	while (Display::Valid()) {
		Time::Update();

		// Update Resources, Inputs & Display

		//Resource::Update();
		Input::Update();

		if (!Config::Get<bool>("headless")) {
			Display::Update();

			if (!Display::Valid()) break;
		}
		
		// Do Update Step
		
		if (UpdateCallback) UpdateCallback();
		//Scene::Update();

		//if (!Config::Get<bool>("headless")) {
		//	Compositor::LateUpdate();
		//}

		if (LateUpdateCallback) LateUpdateCallback();
		//Scene::LateUpdate();
		
		// Do Display Step
		
		if (!Config::Get<bool>("headless")) {
			if (DisplayCallback) DisplayCallback();
			//Scene::Display();

			//Compositor::Display();
		
			if (OnGUICallback) OnGUICallback();
			//Scene::OnGUI();
		
			Display::Finish();
		}

		// Do FPS Limit

		Time::Wait();
	}
	
	// Do Terminate
	
	if (TermCallback) TermCallback();
	//Scene::Term();
	
	glfwTerminate();
}
