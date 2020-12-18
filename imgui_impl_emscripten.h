// dear imgui: Platform Backend for Emscripten
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API

IMGUI_IMPL_API bool     ImGui_ImplEmscripten_Init();
IMGUI_IMPL_API void     ImGui_ImplEmscripten_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplEmscripten_NewFrame();
