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

# ERRORS
0. No errors
1. System error
2. Could not open file
3. File empty

100. Validation layers requested but not supported
101. No physical devices found
102. Failed to find suitable memory type

# Includes used
1. <vulkan/vulkan.h> - **From Vulkan SDK**
2. <stdio.h> - **From C standard library**
3. <stdlib.h> - **From C standard library**
4. <stdarg.h> - **From C standard library**

# NonGLM indexes
0. Vertex - vec2 pos; vec3 color

# Problems
1. delQue
2. delQue in pipelines
3. delQue in buffers