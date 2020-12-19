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
        ctx = canvas_myengine.getContext("webgl2", settings_myengine);
        myengine.handle = GL.registerContext(ctx, settings_myengine);
        GL.makeContextCurrent(myengine.handle);
        myengine.ctx = ctx;
    });

    EM_ASM({
        myengine.comp = canvas_composite.getContext("2d");
    });

    loop = [&] {    

        if (g_rm.pending) return;

        EM_ASM({
            GL.makeContextCurrent(myengine.handle_offscreen);
        });
        loop_filament();

        EM_ASM({
            GL.makeContextCurrent(myengine.handle);
        });

        glClear(GL_COLOR_BUFFER_BIT);
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

        EM_ASM({
            myengine.comp.drawImage(canvas_filament, 0, 0);
            myengine.comp.drawImage(canvas_myengine, 0, 0);
        });
    };

    g_imctx = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(800, 600);
    ImGui_ImplEmscripten_Init();
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsLight();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    EM_ASM({
        const ctx_offscreen = canvas_filament.getContext("webgl2", settings_filamenet);
        ctx_offscreen.getExtension('WEBGL_compressed_texture_s3tc');
        ctx_offscreen.getExtension('WEBGL_compressed_texture_astc');
        ctx_offscreen.getExtension('WEBGL_compressed_texture_etc');
        myengine.handle_offscreen = GL.registerContext(ctx_offscreen, settings_filamenet);
        GL.makeContextCurrent(myengine.handle_offscreen);
    });
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