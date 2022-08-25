param (
    [switch]$DEBUG = $false,
    [switch]$fast = $false,
    [switch]$TESTDLL = $false,
    [switch]$FULLDLL = $false,
    [switch]$AGshaders = $false,
    [switch]$commitFiles = $false,
    [switch]$buildFiles = $false,
    [switch]$buildModuleHandler = $false,
    [switch]$all = $false,
    [switch]$RDEBUG = $false
)

if($all)
{
    $DEBUG = $false
    $fast = $true
    $TESTDLL = $true
    $FULLDLL = $true
    $AGshaders = $true
    $commitFiles = $true
    $buildFiles = $true
    $buildModuleHandler = $true
}
if($modules)
{
    $commitFiles = $true
    $buildFiles = $true
    $buildModuleHandler = $true
}
if($buildModuleHandler)
{
    gcc ./MKTModuleHandler/MKTMH.c -shared -o ./MKTModuleHandler/MKTMH.dll
    cp ./MKTModuleHandler/MKTMH.dll ./build/lib/windows/
    gcc ./MKTModuleHandler/testModule.c -shared -o ./simpleTestModulePackage/testModule.dll
}
if($buildFiles)
{
    gcc ./MKTfileConverters/MKTP/src/MKTP.c -shared -o ./MKTfileConverters/MKTP/build/windows/MKTP.dll
}
if($commitFiles)
{
    cp ./MKTfileConverters/MKTP/build/windows/* ./build/files/
}
if ($DEBUG) {
    $DEBUGS = "-DMKT_DEBUG -g"
} else {
    $DEBUGS = ""
}
if ($AGshaders) {
./shadersCompilation/AGshaders.bat
}
if($TESTDLL)
{
    gcc ./src/testLibraries/testLibrary.c -I C:/sdk/ -shared -o ./build/lib/windows/testLibrary.dll
    gcc ./src/testLibraries/testLibrary2.c -I C:/sdk/ -shared -o ./build/lib/windows/testLibrary2.dll
}
if($FULLDLL)
{
    gcc ./MAD/ArrayGraphicsLibrary/AG.c ./MAD/ArrayGraphicsLibrary/basicMKT.o -I C:/sdk/ -shared -o ./build/lib/windows/AG.dll
}

if($RDEBUG)
{
    $RDEBUGS = "-DMKT_R_DEBUG"
} else {
    $RDEBUGS = ""
}

gcc $RDEBUGS $DEBUGS -I ./src/vulkan/ ./src/vulkan/fileLoading/AG.c ./src/vulkan/fileLoading/fileLoading.c ./src/vulkan/utils/MKTbuffer.c ./src/vulkan/goodLuckMate/glm.c ./src/vulkan/MKTAppEngineUtils.c ./src/vulkan/MKTAppEngineRun.c ./src/vulkan/pipeline/pipeline.c ./src/vulkan/MKTAppEngineInit.c ./src/vulkan/delQue/delQue.c ./src/errorHandling.c ./src/utils.c ./src/main.c ./src/vulkan/MKTAppEngine.c -L C:\sdk\lib -l SDL2 -l vulkan-1 -l SDL2main -l gdi32 -l user32 -l kernel32 $(If ($fast) {"-Ofast"} Else {""}) $(If ($DEBUG) {"-g"} Else {""})  -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -l glfw3 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -I C:\sdk\include -o ./build/main.exe >> error.log