#ifndef timer_h
#define timer_h

struct Time {
private:
	// Member Variables

	double minDelta = 0;
	double frameTime = 0;
	double deltaTime = 0;
public:
	// Member Variables
	
	float delta = 0.0f;
	
	unsigned int fps = 0;
	unsigned int tick = 0;
	
	// Member Functions

	void update();
	void wait();

	void sleep(double seconds);

	void set_limit(unsigned int fps);
};

extern Time timer;

#endif
