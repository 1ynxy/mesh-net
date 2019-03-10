#ifndef timer_h
#define timer_h

#include <../src/shared.h>

struct Time {
private:
	// Member Variables

	static double minDelta;
	static double frameTime;
	static double deltaTime;
public:
	// Member Variables
	
	static float delta;
	
	static unsigned int fps;
	static unsigned int tick;
	
	// Member Functions

	static void Update();
	static void Wait();

	static void Sleep(double seconds);

	static void SetFPSLimit(unsigned int fps);
};

#endif
