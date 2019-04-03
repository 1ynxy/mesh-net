#include "time.h"

#include <GLFW/glfw3.h>

#ifdef __linux__
	#include <unistd.h>
#elif WIN32
	#include <windows.h>
#endif

// Member Functions

void Time::update() {
	// Calculate Delta Time
	
	double newTime = glfwGetTime();

	deltaTime = newTime - frameTime;

	frameTime = newTime;

	// Set Public Variables

	delta = (float) deltaTime;

	fps = 1 / deltaTime;

	tick++;
}

void Time::wait() {
	if (minDelta != 0) {
		double wait = minDelta - (glfwGetTime() - frameTime);

		if (wait > 0) sleep(wait);
	}
}

void Time::sleep(double seconds) {
	int milli = (unsigned int) (seconds * 1000);

	#ifdef __linux__
    	usleep(milli * 1000);
	#elif WIN32
    	Sleep(milli);
	#endif
}

void Time::set_limit(unsigned int fps) {
	minDelta = 1.0f / fps;
}
