param (
    [switch]$DEBUG = $false,
    [switch]$fast = $false,
    [switch]$DLL = $false,
    [switch]$AGshaders = $false,
    [switch]$PiCshaders = $false
)
if($DLL)
{
    gcc ./MKTModuleHandler/MKTMH.c -shared -o ./MKTModuleHandler/MKTMH.dll
    
    gcc ./MKTfileConverters/PiC/RAW2PiC.c -I ./src/ -shared -o ./build/files/MKTRAWPiC.dll
}
if ($DEBUG) {
    $DEBUGS = "-DMKT_DEBUG"
} else {
    $DEBUGS = ""
}
if ($AGshaders) {
./shadersCompilation/AGshaders.bat
}
if ($PiCshaders) {
./shadersCompilation/PiCshaders.bat
}

gcc $DEBUGS -I ./src/vulkan/ ./src/MKTDLL/MKTDLL.c ./src/vulkan/fileLoading/ViD.c ./src/vulkan/fileLoading/PiC.c ./src/vulkan/fileLoading/Info.c ./src/vulkan/fileLoading/Audio.c ./src/vulkan/fileLoading/AG.c ./src/vulkan/fileLoading/fileLoading.c ./src/vulkan/utils/MKTbuffer.c ./src/vulkan/goodLuckMate/glm.c ./src/vulkan/MKTAppEngineUtils.c ./src/vulkan/MKTAppEngineRun.c ./src/vulkan/pipeline/pipeline.c ./src/vulkan/MKTAppEngineInit.c ./src/vulkan/delQue/delQue.c ./src/errorHandling.c ./src/utils.c ./src/main.c ./src/vulkan/MKTAppEngine.c -L C:\sdk\lib -l SDL2 -l vulkan-1 -l SDL2main -l gdi32 -l user32 -l kernel32 $(If ($fast) {"-Ofast"} Else {""}) $(If ($DEBUG) {"-g"} Else {""})  -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -l glfw3 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -I C:\sdk\include -o ./build/main.exe