#include <MKTAppEngine.h>

void copyBufferToImage(VkBuffer buffer, VkImage * image, uint32_t width, uint32_t height);
void transitionImageLayout(VkImage * image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);