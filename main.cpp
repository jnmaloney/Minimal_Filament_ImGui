#include <emscripten.h>
#include <emscripten/html5.h>
#include <functional>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_emscripten.h"

#include "filament_setup.h"
#include "ResourceManager.h"


ImGuiContext* g_imctx;


ResourceManager g_rm;


std::function<void()> loop;
void main_loop() { loop(); }


int main() 
{
    EM_ASM({
        const ctx = canvas.getContext("webgl2", defaults);
        ctx.getExtension('WEBGL_compressed_texture_s3tc');
        ctx.getExtension('WEBGL_compressed_texture_astc');
        ctx.getExtension('WEBGL_compressed_texture_etc');
        const handle = GL.registerContext(ctx, defaults);
        GL.makeContextCurrent(handle);
    });

    loop = [&] {    

        glClearColor(0.2f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        loop_filament();

        ImGui::SetCurrentContext(g_imctx);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplEmscripten_NewFrame();
        ImGui::NewFrame();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("x: %.1f", io.MousePos.x);
        ImGui::Text("y: %.1f", io.MousePos.y);
        ImGui::Text("button %i", io.MouseDown[0]);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    g_imctx = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(800, 600);
    ImGui_ImplEmscripten_Init();
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsLight();
    
    init_filament();

    enum 
    {
      FILAMESH = 0,
      FILAMAT,
      TEXTURE_KTX,
    };
    std::function z = &on_ktx_load;
    g_rm.addResourceType(TEXTURE_KTX, &z);

    g_rm.addResource("sky",               "default_env/default_env_skybox.ktx",   TEXTURE_KTX);
    g_rm.start_async_load();

    emscripten_set_main_loop(main_loop, 0, true);

    return 0;
}