g++ ./src/cmdMode.c ./src/libraryLoader.c ./src/fileManagment/MKTarrayGraphics.c ./src/vulkan/vkbL.o ./src/main.c -I ../../sdk/libraries/inc/ -L ../../sdk/libraries/lib/ ./src/vulkan/MKTAppEngine.cpp ./src/vulkan/init.cpp ./src/vulkan/MKTMesh.cpp -o ./build/main.bin -lSDL2 -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi