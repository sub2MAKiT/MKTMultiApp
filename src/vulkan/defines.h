#define VK_CHECK(x)                                             \
do                                                              \
{                                                               \
    VkResult err = x;                                           \
    if (err)                                                    \
    {                                                           \
        printf("Detected Vulkan error: %d\n",err);                                      \
        abort();                                                \
    }                                                           \
} while (0)

#define GETINITDATA    \
#ifdef _WIN32\
char _USEROS_ = 0;\
#elif __gnu_linux__\
char _USEROS_ = 1;\
#elif __APPLE__\
char _USEROS_ = 2;\
#endif\
\
initi initData = {};\
initData.userOS = _USEROS_;\
initData.windowX = _windowExtent.width;\
initData.windowY = _windowExtent.height
