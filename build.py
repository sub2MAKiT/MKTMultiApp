# windows wsl is a bit special so...

import os

os.system('nasm -f elf64 ./src/math/header.x64.s -o ./linking/header.x86_64.o')
os.system('gcc ./fileLibraries/PiC/RAW2PiC.c -I ./src/ -shared -o ./build/files/MKTRAWPiC.so')
os.system('gcc ./fileLibraries/PiC/PiC2PiC.c -I ./src/ -shared -o ./build/files/MKTP.so')
os.system('gcc ./fileLibraries/AG/AG2AG.c -I ./src/ -shared -o ./build/files/MKTAG.so')
os.system('gcc ./fileLibraries/BM/BM2BM.c -I ./src/ -shared -o ./build/files/MKTbm.so')
os.system('gcc -DMKT_DEBUG -g ./src/vulkan/openCLSetup.c ./src/math/math.c ./src/MKTDLL/MKTDLL.c ./src/vulkan/images.c ./src/vulkan/fileLoading/BM.c ./src/vulkan/fileLoading/TD.c ./src/vulkan/fileLoading/ViD.c ./src/vulkan/fileLoading/PiC.c ./src/vulkan/fileLoading/Info.c ./src/errorHandling.c ./src/vulkan/fileLoading/Audio.c ./src/vulkan/fileLoading/AG.c ./src/vulkan/fileLoading/fileLoading.c ./src/vulkan/utils/MKTbuffer.c ./src/vulkan/goodLuckMate/glm.c ./src/vulkan/MKTAppEngineUtils.c ./src/vulkan/MKTAppEngineRun.c ./src/vulkan/pipeline/pipeline.c ./src/vulkan/MKTAppEngineInit.c ./src/vulkan/delQue/delQue.c ./src/utils.c ./src/main.c ./src/vulkan/MKTAppEngine.c ./linking/header.x86_64.o -l vulkan -l glfw -l OpenCL -lm -I ../1.3.231.2/x86_64/include/ -L ../Bglfw/src/ -L ../1.3.231.2/x86_64/lib/ -I ./src/vulkan/ -I ../glfw/include/ -I /usr/include/nvidia -o ./build/main')