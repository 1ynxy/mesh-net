#include "time.h"

#ifdef __linux__
	#include <unistd.h>
#elif WIN32
	#include <windows.h>
#endif

// Member Variables

double Time::minDelta = 0;
double Time::frameTime = 0;
double Time::deltaTime = 0;

float Time::delta = 0;
unsigned int Time::fps = 0;
unsigned int Time::tick = 0;

// Member Functions

void Time::Update() {
	// Calculate Delta Time
	
	double newTime = glfwGetTime();

	deltaTime = newTime - frameTime;

	frameTime = newTime;

	// Set Public Variables

	delta = (float) deltaTime;

	fps = 1 / deltaTime;

	tick++;
}

void Time::Wait() {
	if (minDelta != 0) {
		double wait = minDelta - (glfwGetTime() - frameTime);

		if (wait > 0) Time::Sleep(wait);
	}
}

void Time::Sleep(double seconds) {
	int milli = (unsigned int) (seconds * 1000);

	#ifdef __linux__
    	usleep(milli * 1000);
	#elif WIN32
    	Sleep(milli);
	#endif
}

void Time::SetFPSLimit(unsigned int fps) {
	minDelta = 1.0f / fps;
}