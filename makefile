DEV_DIR=../..
WEBCORE_DIR=$(DEV_DIR)/WebCore
FILAMENT_DIR=$(DEV_DIR)/filament-1.9.9_wasm/filament

FILAMENT_INCLUDE = -I$(FILAMENT_DIR)/include -I$(FILAMENT_DIR)/backend/include -I$(FILAMENT_DIR)/../libs/utils/include -I$(FILAMENT_DIR)/../libs/math/include -I$(FILAMENT_DIR)/../libs/filameshio/include -I$(FILAMENT_DIR)/../libs/filabridge/include -I$(FILAMENT_DIR)/../libs/image/include 
INCLUDE = -Isrc -I/usr/local/include/ $(FILAMENT_INCLUDE) -Iextern/glad/include/ 

OUTPUT=out/myengine.js

SOURCES = main.cpp $(wildcard src/*.cpp) $(wildcard extern/glad/src/*.cpp)

FLAGS = -std=c++1z 
CC = emcc
CFLAGS = $(FLAGS) $(INCLUDE) 

LIB_DIRS = -L/lib/ -L/usr/local/lib/ -L$(FILAMENT_DIR)/../linktome

FILAMENT_LIBS=-lfilament -lbackend -lfilabridge -lfilaflat -lutils -lgeometry -lsmol-v -libl -lfilameshio -lmeshoptimizer -limage
LIBS = $(FILAMENT_LIBS) 

USE_IMGUI=-I$(WEBCORE_DIR)/imgui/ $(wildcard $(WEBCORE_DIR)/imgui/*.cpp) $(WEBCORE_DIR)/imgui/backends/imgui_impl_emscripten.cpp $(WEBCORE_DIR)/imgui/backends/imgui_impl_opengl3.cpp

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(LIBS) $(LIB_DIRS) -o $(OUTPUT) -s FULL_ES3=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 $(USE_IMGUI)
    
clean:
	rm $(OUTPUT)
