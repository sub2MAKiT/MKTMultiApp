// #include <glm/glm.hpp>
// #include <glm/gtx/transform.hpp>
#include <types.h>


// Push constants
struct MeshPushConstants {
    glm::vec4 data;
    glm::mat4 render_matrix;
};

struct AGPushConstants {
    glm::mat4 colourModification;
    glm::mat4 transformation;
    glm::vec3 movement;
};

struct TEXTPushConstant {
	glm::vec3 colour;
	glm::mat4 transformation;
	glm::vec3 movement;
};

// MKT3D
struct GPUCameraData{
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewproj;
};

// MKTPiC

typedef struct vulPic {
VkImage textureImage;
VkDeviceMemory textureImageMemory;
VkImageView textureImageView;
AGPushConstants PC;
char isVisible;
} vPic;

typedef struct MKTRGBAPixel {
unsigned char R;
unsigned char G;
unsigned char B;
unsigned char A;   
} MKTPix;

typedef struct MKTRGBApicture {
MKTPix * pixels;
size_t width;
size_t height;
} MKTPic;