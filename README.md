# ConsoleApplication1

ConsoleApplication1 because I am just playing around with freeglut library.

## Exposing dynamic libraries to the program

Place the dynamic libraries to this location: [deps\DLLS](./deps/DLLS).

## Windows setup

### Generate freeglut project for Visual Studio using CMake

These instructions are located at [README.cmake](./deps/freeglut-3.4.0/README.cmake), I modified them for my own use case to be much more specific.

#### Download CMake GUI

1. Open [CMake download page](https://cmake.org/download/)
2. Scroll down to **BINARY DISTRIBUTIONS**
3. Find **Windows 64 ZIP**
4. Download it
5. Extract it to `C:\Users\<User>\Documents`
6. There should be directory `C:\Users\<User>\Documents\cmake-3.25.2-windows-x86_64`
7. Run `C:\Users\<User>\Documents\cmake-3.25.2-windows-x86_64\bin\cmake-gui`

I prefer zip because I don't have to deal with installer and I know installer won't mess with cmake I installed using MSYS2. The CMake I installed using MSYS2 is in system variables and globally available, so I don't want the two of them to conflict.

I downloaded the CMake GUI application because freeglut library recommended to use it to build it using CMake. I wanted to run the exact steps the library recommended to follow because I am not sure what I am doing so I want to follow the instructions as closely as possible.

#### Generate Visual Studio project

I used visual studio 2022, I don't know if it works with other versions.

1. In the "Where is the source code" box, set the value to `C:\Users\<User>\source\repos\ConsoleApplication1\deps\freeglut-3.4.0`
2. In the "Where to build the binaries" box, set the value to `C:/Users/<User>/source/repos/ConsoleApplication1/deps/freeglut-3.4.0/build`
3. Hit the Configure button near the bottom of the window.
4. Pick your target compiler, make sure that its installed on your system of course!
5. I have a compiler located at: `C:\msys64\mingw64\bin\clang++` and I used that one.
6. Answer Ok when asked if you want to create the build directory.
7. Wait for the configure process to finish.
8. The screen will now have some configuration options on it, for instance specifying whether you want to build static and/or shared libraries ([See here at the bottom of the file for full list](./deps/freeglut-3.4.0/README.cmake)). When you've selected your options, click the Configure button again.
9. The Generate button at the bottom will now be enabled. Click Generate.
10. The build files will now be generated in the location in: `C:/Users/<User>/source/repos/ConsoleApplication1/deps/freeglut-3.4.0/build`.

Now the freeglut should be ready to be built using Visual Studio.

### Build freeglut library using Visual Studio

#### Open freeglut project in Visual Studio and build it

1. Open the directory where freeglut is built, for me it is at `C:\Users\<User>\source\repos\ConsoleApplication1\deps\freeglut-3.4.0\build`.
2. Run `freeglut.sln` to open Visual Studio.
3. Select `Debug` and `x64` from the top bar.
4. Build the project by pressing `Ctrl + Shift + B`. Or Go to `Build` -> `Build Solution`.

#### Verify that the library was built successfully

**ABSOLUTELY IMPORTANT!!! BUILD LIBRARY AND THE PROGRAM USING THE SAME CONFIGURATION!!!**

**LIBRARY BUILT USING _DEBUG x64_ HAS TO BE USED BY APPLICATION BUILT USING _DEBUG x64_ CONFIGURATION!!!**

1. Find `freeglutd.dll` at `C:\Users\<User>\source\repos\ConsoleApplication1\deps\freeglut-3.4.0\build\bin\Debug`
2. Find `freeglutd.lib` and `freeglut_staticd.lib` at `C:\Users\<User>\source\repos\ConsoleApplication1\deps\freeglut-3.4.0\build\lib\Debug`.
3. Verify that these exist at these locations.

If all is good so far, we are ready to go to the next step and link the program against the library.

### Link program against freeglut library

1. Add the library's include path: Go to the "Project" menu and select "Properties." In the "Configuration Properties" section, navigate to "C/C++" -> "General" and add the library's include path to the "Additional Include Directories" field. `C:\Users\<User>\source\repos\ConsoleApplication1\deps\freeglut-3.4.0\include`
2. Link the library to the project: Go to the "Project" menu and select "Properties." In the "Configuration Properties" section, navigate to "Linker" -> "General" and add the library's path to the "Additional Library Directories" field. `C:\Users\<User>\source\repos\ConsoleApplication1\deps\freeglut-3.4.0\build\lib\Debug`
3. Then navigate to "Linker" -> "Input" and add the library's name to the "Additional Dependencies" field. `freeglutd.lib;freeglut_staticd.lib;`
4. Press start without debugging to run the program. Or use `Ctrl + F5`.

The program show show an error that it can't find `freeglutd.dll`.

1. Open `C:\Users\<User>\source\repos\ConsoleApplication1\deps\freeglut-3.4.0\build\bin\Debug` and find `freeglutd.dll`.
2. Copy `freeglutd.dll`.
3. Open `C:\Windows\System32` and paste `freeglutd.dll` there.
4. The path to `freeglutd.dll` should now be: `C:\Windows\System32\freeglutd.dll`
5. Press start without debugging to run the program. Or use `Ctrl + F5`.

The program should now run successfully.
