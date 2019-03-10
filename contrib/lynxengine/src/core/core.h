#ifndef core_h
#define core_h

#include <../src/shared.h>

struct Core {
private:
	// Gameloop Callbacks
	
	static void (*InitCallback) (void);
	static void (*UpdateCallback) (void);
	static void (*LateUpdateCallback) (void);
	static void (*DisplayCallback) (void);
	static void (*OnGUICallback) (void);
	static void (*TermCallback) (void);
	
public:
	// Gameloop Callback Setup
	
	static void SetInitCallback(void (*Init) ());
	static void SetUpdateCallback(void (*Update) ());
	static void SetLateUpdateCallback(void (*LateUpdate) ());
	static void SetDisplayCallback(void (*Display) ());
	static void SetOnGUICallback(void (*OnGUI) ());
	static void SetTermCallback(void (*Term) ());

	static void SetCallbacks(void (*Init) (), void (*Update) (), void (*LateUpdate) (), void (*Display) (), void (*OnGUI) (), void (*Term) ());
	
	// Member Functions
	
	static void Init();
};

#endif
