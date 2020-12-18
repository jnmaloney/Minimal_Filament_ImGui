#include "filament_setup.h"
#include <stdio.h>
#include <filament/FilamentAPI.h>
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/View.h>
#include <utils/EntityManager.h>
#include <filament/RenderableManager.h>
#include <filament/Scene.h>
#include <filament/Material.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/Skybox.h>
#include <filament/Viewport.h>
#include <filament/SwapChain.h>
#include <filament/Camera.h>
#include <filament/TransformManager.h>
#include <filameshio/MeshReader.h>
#include <math/mat3.h>
#include <math/mat4.h>
#include <utils/Path.h>
#include <filament/LightManager.h>
#include <filament/TextureSampler.h>
#include <filament/IndirectLight.h>
#include <image/KtxBundle.h>
#include <image/KtxUtility.h>
#include "ResourceManager.h"


// Filement core objects
filament::Engine* fila_engine;
filament::SwapChain* fila_swap_chain;
filament::View* fila_view;
filament::Renderer* fila_renderer;
filament::Camera* fila_cam;
filament::Scene* fila_scene;

unsigned int get_w() { return 800; }
unsigned int get_h() { return 600; }

void set_camera_angle(float a, float b)
{
  float r = 8.f;
  float x = r * sinf(a) * cosf(b);
  float y = r * sinf(b);
  float z = r * cosf(a) * cosf(b);
  fila_cam->lookAt({x, y, z}, {0, 0, 0}, {0, 1, 0});
}


void init_filament()
{
  // Create engine (Set current GL context beforehand)
  fila_engine = filament::Engine::create();
  if (fila_engine == nullptr) 
  {
    printf("Failled to create the filament engine.\n");
    return;
  }

  // Swap chain from context?
  fila_swap_chain = fila_engine->createSwapChain(nullptr);

  if (fila_swap_chain == nullptr) 
  {
    printf("Failed to create the SwapChain.\n");
    return;
  }

  // Other objects...

  fila_renderer = fila_engine->createRenderer();
  fila_scene = fila_engine->createScene();
  fila_view = fila_engine->createView();
  auto camera_entity = utils::EntityManager::get().create();
  fila_cam = fila_engine->createCamera(camera_entity);

  fila_cam->setExposure(16.0f, 1 / 125.0f, 100.0f);
  //fila_cam->setExposure(100.0f);
  fila_cam->setProjection(60.0f, float(get_w())/get_h(), 0.1f, 100.0f);
  set_camera_angle(0.f, 0.f);
  
  printf("Creating viewport %ix%i\n", get_w(), get_h());

  fila_view->setViewport({0, 0, get_w(), get_h()});
  fila_view->setScene(fila_scene);
  fila_view->setCamera(fila_cam); 
  fila_view->setName("main-view");

  fila_renderer->setClearOptions({
    .clearColor = { 0.0f, 0.13f, 0.0f, 1.0f },
    .clear = true
  });
}


void stop_filament()
{
}


void loop_filament()
{
  if (fila_renderer->beginFrame(fila_swap_chain)) 
  {
    fila_renderer->render(fila_view);
    fila_renderer->endFrame();
  }

  fila_engine->execute();
}


void* on_ktx_load(void* buffer, unsigned int buffer_size)
{
  image::KtxBundle* bundle = 
    new image::KtxBundle(
      (const uint8_t*)buffer, 
      buffer_size
    );

  filament::Texture* texture = image::ktx::createTexture(fila_engine, bundle, !bundle->isCubemap());

  filament::Texture* skytex = texture; 
  filament::Skybox* sky = filament::Skybox::Builder().environment(skytex).build(*fila_engine);
  fila_scene->setSkybox(sky);

  return (void*)texture;
}

