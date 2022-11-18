param (
    [switch]$DEBUG = $false,
    [switch]$fast = $false,
    [switch]$DLL = $false,
    [switch]$AGshaders = $false,
    [switch]$PiCshaders = $false,
    [switch]$bmshaders = $false,
    [switch]$qdrshaders = $false,
    [switch]$math = $false,
    [switch]$windowsmath = $false
)

$windows = $true

if ($DEBUG) {
    $DEBUGS = "-DMKT_DEBUG"
    $DEBUGG = "-g"
} else {
    $DEBUGS = ""
    $DEBUGG = ""
}
if($DLL)
{
    gcc ./fileLibraries/PiC/RAW2PiC.c -I ./src/ $DEBUGG -shared -o ./build/files/MKTRAWPiC.dll
    gcc ./fileLibraries/PiC/PiC2PiC.c -I ./src/ $DEBUGG -shared -o ./build/files/MKTP.dll
    gcc ./fileLibraries/AG/AG2AG.c -I ./src/ $DEBUGG -shared -o ./build/files/MKTAG.dll
    gcc ./fileLibraries/BM/BM2BM.c -I ./src/ $DEBUGG -shared -o ./build/files/MKTbm.dll
}
if ($AGshaders) {
./shadersCompilation/AGshaders.bat
}
if ($PiCshaders) {
./shadersCompilation/PiCshaders.bat
}
if ($bmshaders) {
./shadersCompilation/bmshaders.bat
}
if ($qdrshaders) {
    ./shadersCompilation/qdrshaders.bat
}

# elf64 x86_64
# win64 x64

if ($math) {
    nasm -f elf64 ./src/math/0.x86_64.stat.s -o ./linking/0.x84_64.o
} elseif ($windowsmath) {
    nasm -f win64 ./src/math/0.x64.stat.s -o ./linking/0.x64.o
}

gcc $DEBUGS $(If ($windows) {"./linking/0.x64.o"} Else {"./linking/0.x84_64.o"}) -I ./src/vulkan/ ./src/math/math.c ./src/MKTDLL/MKTDLL.c ./src/vulkan/images.c ./src/vulkan/fileLoading/BM.c ./src/vulkan/fileLoading/TD.c ./src/vulkan/fileLoading/ViD.c ./src/vulkan/fileLoading/PiC.c ./src/vulkan/fileLoading/Info.c ./src/vulkan/fileLoading/Audio.c ./src/vulkan/fileLoading/AG.c ./src/vulkan/fileLoading/fileLoading.c ./src/vulkan/utils/MKTbuffer.c ./src/vulkan/goodLuckMate/glm.c ./src/vulkan/MKTAppEngineUtils.c ./src/vulkan/MKTAppEngineRun.c ./src/vulkan/pipeline/pipeline.c ./src/vulkan/MKTAppEngineInit.c ./src/vulkan/delQue/delQue.c ./src/errorHandling.c ./src/utils.c ./src/main.c ./src/vulkan/MKTAppEngine.c -L C:\sdk\lib -l SDL2 -l vulkan-1 -l SDL2main -l gdi32 -l user32 -l kernel32 $(If ($fast) {"-Ofast"} Else {""}) $DEBUGG  -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -l glfw3 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -I C:\sdk\include -o ./build/main.exe