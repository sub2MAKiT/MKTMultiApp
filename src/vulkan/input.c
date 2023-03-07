#include <MKTAppEngine.h>

char shiftIsPressed = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_LEFT_SHIFT) shiftIsPressed = action == GLFW_PRESS;

    char pressed = key;

    switch(key)
    {
        case GLFW_KEY_BACKSPACE:
        {
            pressed = 10;
            break;
        }
    }

    if(!shiftIsPressed)
    {
        if(pressed > 64 && pressed < 91)
            pressed += 32;
    }

    if (action == GLFW_PRESS)
        printf("%c",pressed);
}