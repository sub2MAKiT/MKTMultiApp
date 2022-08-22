#include <../utils.h>
#include <delQue/delQue.h>
#include <../errorHandling.h>
#include <types.h>

// FUNCTIONS

// MAIN FUNCTIONS
void init();
void run();
void cleanup();

// INIT FUNCTIONS
void _VE_INIT_window(); // An init function for GLFW window creation

// GLOBAL VARIABLES

#ifdef _MKT_APP_ENGINE_THIRD_
// Without the guard

// GLFW
GLFWwindow * _window;

#else
// With the guard

// GLFW
extern GLFWwindow * _window;

#endif

/* some helpfull errors to know
* 1.
* 2.
* 3. -1073741510 - The physical resources of this disk have been exhausted.
*/