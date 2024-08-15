# BattleBeyz

BattleBeyz is a 3D game that uses OpenGL for rendering graphics. My goal is to create the first realistic Beyblade game that handles high-speed collisions with custom physics, and I wanted to learn from the process of creating a game from scratch.

Any collaboration or feedback is welcome!

## Features (So far!)

- 3D rendering with OpenGL
- Font rendering with FreeType
- Basic Camera control
- Lighting and Shading
- Basic Textures
- Dynamic Meshes
- UI Elements/Navigation
- Scrolling Backgrounds

## Building with Visual Studio 2022
- Open a command prompt, and change to the project folder, for example, CD \BattleBeyz.
- Remove any existing build folder: rm build/s/q
- Run cmake:  cmake -S . -B build
  Read the output carefully, and make sure cmake found the correct version of Visual C++.
- Change to the build folder:  cd build
- Open the Solution file:  devenv BattleBeyz.sln
- Recommended:  Right click on the BattleBeyz subproject in the solution explorer, and select "Set as Startup Project".
- Select the build variant (Debug, Release, etc), and then build the solution from the Build menu.
- Debug or run the application.

On your first run attempt you might get a "freetype.dll not found".
A quick fix is to copy the dll lib\freetype-2.13.2\dll\win64\freetype.dll to build\Debug.
Eventually we will make this happen automatically.


## Installation
TODO

### Libraries

#### GLFW, GLEW, GLM, FreeType, Assimp, Imgui, stb_image
These libraries are already built and included in the `lib` folder. If you want to build them yourself, you can follow
the links below for the corresponding versions.
```sh
cd lib
curl -LO https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
unzip glfw-3.4.zip
curl -LO https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.zip
unzip glew-2.2.0.zip
curl -LO https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
unzip glm-0.9.9.8.zip
curl -LO https://download.savannah.gnu.org/releases/freetype/freetype-2.13.2.tar.gz
tar -xzf freetype-2.13.2.tar.gz
curl -LO https://github.com/assimp/assimp/archive/refs/tags/v5.2.5.zip
unzip v5.2.5.zip
curl -LO https://github.com/ocornut/imgui/archive/refs/tags/v1.90.8.zip
unzip v1.90.8.zip
curl -LO https://github.com/nothings/stb/archive/refs/tags/v0.8.zip
unzip v0.8.zip
```
