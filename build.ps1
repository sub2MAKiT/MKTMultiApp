gcc ./src/main.c -l glfw3 -l gdi32 -l user32 -lopengl32 -I c:/sdk/include/ -I c:/sdk/ -L c:/sdk/lib/ -L c:/sdk/ -Ofast -o main.exe
# -l vulkan-1 -l SDL2main -l kernel32  -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid