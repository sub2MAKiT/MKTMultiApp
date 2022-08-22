#define _MKT_APP_ENGINE_THIRD_
#include <MKTAppEngine.h>
// MAIN FUNCIONS

void init()
{

    _VE_INIT_window(); // #0000ff

    _VE_INIT_Instance(); // #0000ff

    _VE_INIT_PhysicalDevice(); // #0000ff

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

    vkDestroyInstance(_instance, NULL);

    glfwDestroyWindow(_window);

    glfwTerminate();

    free(_requiredExtensions.pString);
    free(validationLayers.pString);
    free(_extensions);

    DEBUG("III> cleanup <III");
    return;
}