gcc ./src/testLibraries/testLibrary.c -shared -o ./build/lib/linux/testLibrary.so
gcc ./src/testLibraries/testLibrary2.c -shared -o ./build/lib/linux/testLibrary2.so
gcc ./MAD/ArrayGraphicsLibrary/AG.c ./MAD/stdLib/object.c -shared -o ./build/lib/linux/AG.so