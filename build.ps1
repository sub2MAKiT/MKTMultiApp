param (
    [switch]$DEBUG = $false,
    [switch]$fast = $false,
    [switch]$TESTDLL = $false,
    [switch]$FULLDLL = $false,
    [switch]$Dshaders = $false,
    [switch]$Cshaders = $false,
    [switch]$Pshaders = $false,
    [switch]$DSshaders = $false,
    [switch]$DiSshaders = $false,
    [switch]$commitFiles = $false,
    [switch]$buildFiles = $false,
    [switch]$buildModuleHandler = $false,
    [switch]$all = $false,
    [switch]$modules = $false
)
if($all)
{
    $DEBUG = $true
    $fast = $true
    $TESTDLL = $true
    $FULLDLL = $true
    $Dshaders = $true
    $Cshaders = $true
    $Pshaders = $true
    $DSshaders = $true
    $DiSshaders = $true
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
if ($Dshaders) {
./shadersCompilation/shaders.bat
}
if ($Cshaders) {
    ./shadersCompilation/Cshaders.bat
}
if ($Pshaders) {
    ./shadersCompilation/PMshaders.bat
}
if ($DSshaders) {
    ./shadersCompilation/DSshaders.bat
}
if ($DiSshaders) {
    ./shadersCompilation/DiSshaders.bat
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
g++ $DEBUGS ./src/vulkan/_render.c ./src/cmdMode.c ./src/libraryLoader.c ./src/fileManagment/MKTarrayGraphics.c ./src/vulkan/vkB.o ./src/main.c -I C:/sdk/ -static-libgcc -static-libstdc++ -static ./src/vulkan/MKTAppEngine.cpp ./src/vulkan/init.cpp ./src/vulkan/MKTMesh.cpp -o ./build/main -I C:\sdk\include -L C:\sdk\lib -l SDL2 -l vulkan-1 -l SDL2main -l gdi32 -l user32 -l kernel32 $(If ($fast) {"-Ofast"} Else {""}) $(If ($DEBUG) {"-g"} Else {""})  -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid && cd ./build/ && .\main.exe