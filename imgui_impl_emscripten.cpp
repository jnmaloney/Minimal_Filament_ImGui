// dear imgui: Platform Backend for Emscripten
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)

#include "imgui.h"
#include "imgui_impl_emscripten.h"

#include <emscripten.h>
#include <emscripten/html5.h>


EM_BOOL em_mouse_callback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{

  if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
  {
      ImGuiIO& io = ImGui::GetIO();
      io.MouseDown[0] = 1;
  }

  if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
  {
      ImGuiIO& io = ImGui::GetIO();
      io.MouseDown[0] = 0;
  }

  if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
  {
      float x = mouseEvent->targetX;
      float y = mouseEvent->targetY;
      ImGuiIO& io = ImGui::GetIO();
      io.MousePos = ImVec2(x, y);
  }

  return 0;
}


bool ImGui_ImplEmscripten_Init()
{
    // Setup backend capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = "imgui_impl_emscripten";

    emscripten_set_mousedown_callback("canvas", nullptr, true, em_mouse_callback);
    emscripten_set_mouseup_callback("canvas", nullptr, true, em_mouse_callback);
    emscripten_set_mousemove_callback("canvas", nullptr, true, em_mouse_callback);

    return true;
}


void ImGui_ImplEmscripten_Shutdown()
{

}


static void ImGui_ImplEmscripten_UpdateMousePosAndButtons()
{
}


void ImGui_ImplEmscripten_NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer backend. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    ImGui_ImplEmscripten_UpdateMousePosAndButtons();
}
