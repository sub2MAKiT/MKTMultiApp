param (
    [switch]$DEBUG = $false,
    [switch]$fast = $false,
    [switch]$Dshaders = $false,
    [switch]$Cshaders = $false,
    [switch]$Pshaders = $false
)
if ($DEBUG) {
    $DEBUGS = "-DMKT_DEBUG"
} else {
    $DEBUGS = ""
}
if ($Dshaders) {
./shadersCompilation/shaders.bat
}
if ($Cshaders) {
    ./shadersCompilation/Cshaders.bat
}
if ($Pshaders) {
    ./shadersCompilation/PMshaders.bat
}
g++ $DEBUGS ./src/vulkan/vkB.o ./src/main.c -I C:/sdk/ ./src/vulkan/MKTAppEngine.cpp ./src/vulkan/init.cpp ./src/vulkan/MKTMesh.cpp -o ./build/main -I C:\sdk\include -L C:\sdk\lib -l SDL2 -l vulkan-1 -l SDL2main -l gdi32 -l user32 -l kernel32 $(If ($fast) {"-Ofast"} Else {""})  -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid && .\build\main.exe