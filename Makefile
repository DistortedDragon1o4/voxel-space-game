all: clean move voxel

# add -X11 and -Xrandr to make nvidia work
CFLAGS = -std=c++20 -g -w
LDFLAGS = -lglfw -lGLU -lGL -ldl -lpthread -msse2 -l FastNoise
WDFLAGS = -lglfw3dll -lopengl32 -lwinpthread -static-libstdc++ -static-libgcc -LC:/mingw/lib -lcomdlg32 -lole32 -msse2
SRC = src/
BUILD = build/
ASSET = assets/

OPENGL_CPP = $(SRC)shaderCompiler.cpp $(SRC)VAO.cpp $(SRC)buffers.cpp $(SRC)texture.cpp $(SRC)camera.cpp $(SRC)HUD.cpp
VOXEL_CHUNKS_CPP = $(SRC)chunkBuilder.cpp $(SRC)chunkList.cpp $(SRC)chunkGenerator.cpp $(SRC)blockInit.cpp $(SRC)lookingAtBlock.cpp $(SRC)lighting.cpp
IMGUI = $(SRC)imgui/*

voxel: $(SRC)main.cpp
	g++ $(CFLAGS) -o $(BUILD)voxel $(SRC)main.cpp $(OPENGL_CPP) $(VOXEL_CHUNKS_CPP) $(IMGUI) $(SRC)stb.cpp $(SRC)glad.c -I include -I shaders -I include/imgui $(LDFLAGS)

gui-test:
	g++ $(CFLAGS) -o $(BUILD)test test/main.cpp $(IMGUI) $(SRC)stb.cpp $(SRC)glad.c -I include -I shaders -I include/imgui $(LDFLAGS)
	./build/test

voxel-windows: $(SRC)main.cpp
	x86_64-w64-mingw32-g++ $(CFLAGS) -o $(BUILD)voxel $(SRC)main.cpp $(OPENGL_CPP) $(VOXEL_CHUNKS_CPP) $(SRC)stb.cpp $(SRC)glad.c -I include -I shaders $(WDFLAGS)

move:
	mkdir build

install:
	mkdir build

run:
	./build/voxel

clean:
	rm -r build
