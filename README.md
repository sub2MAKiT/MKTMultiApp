# VERSIONS

### 00.00.01.00 - Basic error handling
- Basic DEBUG
- `main` function

### 00.00.02.00 - Basic error handling
- Basic error handling

### 00.00.03.00 - Basic error handling
- Functional delQue

### 00.00.04.00 - Basic GLFW implementation
- GLFW

### 00.00.05.00 - VkInstance creation
- VkInstance

### 00.00.06.00 - Validation
- Validation layers
- Added the `0` error code - No errors
- Added the `1` error code - System error
- Added the `100` error code - Validation layers requested but not supported

### 00.00.07.00 - Device picking
- Ability for VentumEngine to pick a GPU
- Ability for VentumEngine to pick a GPU based on rating
- Added the `101` error code - No physical devices found

### 00.00.08.00 - Queue families
- Device picking function, now takes into account the Queue families support

### 00.00.09.00 - Swapchain
- Swapchain

### 00.00.10.00 - Swapchain Images
- Swapchain Images
- Swapchain Image Views

### 00.00.11.00 - Pipelines
- Pipeline creation

### 00.00.12.00 - Runtime
- Runtime

### 00.00.13.00 - Dynamic
- Dynamic init
- Dynamic runtime

### 00.00.14.00 - Triangle
- Triangle

### 00.01.00.00 - First working engine
- Vertex buffers
- Index buffers
- Staging buffers

### 00.01.01.00 - Cleanup
- Cleanup

### 00.01.02.00 - ArrayGraphics
- ArrayGraphics

### 00.01.02.01 - MKTag patch
- MKTag patch

### 00.01.02.02 - C naming convencion patch
- C naming convencion patch

### 00.01.02.03 - fixing patch
- SAFEMALLOC
- Padding buffers
- Fixed the main buffer error

### 00.01.03.00 - fileloading 1
- AG fileloading
- Fixed the heap corruption AG error

### 00.01.04.00 - dll loading
- Dll loading

### 00.01.05.00 - dynamic file dll loading
- Dynamic file dll loading
- Added the `200` error code - Wrong file type loaded

### 00.01.06.00 - AG descriptors
- Created the AG descriptors
- Started using the `<stdalign.h>` header
- Modified the AGShader to use the AG descriptors

### 00.01.07.00 - PiC
- Added the `103` error code - Unsupported layout transition
- cleanup

# ERRORS
0. No errors
1. System error
2. Could not open file
3. File empty

100. Validation layers requested but not supported
101. No physical devices found
102. Failed to find suitable memory type
103. Unsupported layout transition

104. Wrong file type loaded

# Includes used
1. <vulkan/vulkan.h> - **From Vulkan SDK**
2. <GLFW/glfw3.h> - **GLFW**
3. <GLFW/glfw3native.h> - **GLFW**
4. <stdio.h> - **From C standard library**
5. <stdlib.h> - **From C standard library**
6. <stdarg.h> - **From C standard library**
7. <stdalign.h> - **From C standard library**
8. <windows.h> / <dlfcn.h> - **OS specific include**