#define _MKT_APP_ENGINE_THIRD_
#include <MKTAppEngine.h>
// MAIN FUNCIONS

void init()
{

    _VE_INIT_window(); // #0000ff

    DEBUG("III init III");
    DEBUG("III> init <III");
    return;
}

void run()
{
    DEBUG("III run III");

    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
    }

    DEBUG("III> run <III");
    return;
}

void cleanup()
{
    DEBUG("III cleanup III");

    MKTreturnDelQueue();
    MKTreturnError("./errors.log");

    glfwDestroyWindow(_window);

    glfwTerminate();

    DEBUG("III> cleanup <III");
    return;
}