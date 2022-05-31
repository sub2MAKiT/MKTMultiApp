#pragma once
#include "types.h"
#include "vkb/VkBootstrap.h"

#ifdef MKATAPPENGINECFILE

bool _isInitialized{false};
int _frameNumber{0};

VkExtent2D _windowExtent{1700 , 900};

struct SDL_Window* _window{nullptr};

void _MKTAE_init();

void _MKTAE_cleanup();

void _MKTAE_draw();

void _MKTAE_run();
    
VkInstance _instance; // Vulkan library handle
VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
VkDevice _device; // Vulkan device for commands
VkSurfaceKHR _surface; // Vulkan window surface
void init_vulkan();

#else

extern bool _isInitialized;
extern int _frameNumber;

extern VkExtent2D _windowExtent;

extern struct SDL_Window* _window;

extern void _MKTAE_init();

extern void _MKTAE_cleanup();

extern void _MKTAE_draw();

extern void _MKTAE_run();
    
extern VkInstance _instance; // Vulkan library handle
extern VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
extern VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
extern VkDevice _device; // Vulkan device for commands
extern VkSurfaceKHR _surface; // Vulkan window surface
extern void init_vulkan();

#endif