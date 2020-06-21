#include <chrono>
#include <thread>

#include <box2d/box2d.h>

#include "draw.h"
#include "settings.h"

// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs,
// OpenGL/Vulkan/Metal graphics context creation, etc.)

#include <stdio.h>
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function
//  pointers. Helper libraries are often used for this purpose! Here we are supporting a few common
//  ones (gl3w, glew, glad). You may use another loader/header of your choice (glext, glLoadGen,
//  etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>  // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>  // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/gl.h>  // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or multiple definition
                           // errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or multiple definition
                           // errors.
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>  // Initialize with glbinding::initialize()
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of
// testing and compatibility with old VS compilers. To link with VS2010-era libraries, VS2015+
// requires linking with legacy_stdio_definitions.lib, which we do using this pragma. Your own
// project should not be affected, as you are likely to link with a newer binary of GLFW that is
// adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

extern DebugDraw g_debugDraw;
Settings settings;

GLFWwindow* g_mainWindow = nullptr;

int main(int, char**) {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

    // Decide GL+GLSL versions
#if __APPLE__
  // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  // Create window with graphics context
  g_mainWindow = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
  if (g_mainWindow == NULL)
    return 1;
  glfwMakeContextCurrent(g_mainWindow);
  glfwSwapInterval(1);  // Enable vsync

  // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL(glfwGetProcAddress) == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
  bool err = false;
  glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
  bool err = false;
  glbinding::initialize(
      [](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
  bool err = false;  // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to
                     // requires some form of initialization.
#endif
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(g_mainWindow, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  g_debugDraw.Create();

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  b2World world(b2Vec2(0.0f, -10.0f));
  world.SetDebugDraw(&g_debugDraw);

  // Ground body
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -10.0f);
  b2Body* groundBody = world.CreateBody(&groundBodyDef);

  // Ground fixture
  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);

  // Dynamic body
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 40.0f);
  b2Body* body = world.CreateBody(&bodyDef);

  // Dynamic body fixture
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;

  body->CreateFixture(&fixtureDef);

  // Simulation
  float timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;

  std::chrono::duration<double> sleepAdjust(0.0);
  std::chrono::duration<double> frameTime(0.0);

  // Main loop
  while (!glfwWindowShouldClose(g_mainWindow)) {
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    glfwGetWindowSize(g_mainWindow, &g_camera.m_width, &g_camera.m_height);
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(g_mainWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (g_debugDraw.m_showUI) {
      ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
      ImGui::SetNextWindowSize(ImVec2(float(g_camera.m_width), float(g_camera.m_height)));
      ImGui::SetNextWindowBgAlpha(0.0f);
      ImGui::Begin("Overlay", nullptr,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs |
                       ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
      ImGui::End();
    }

    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

    uint32 flags = 0;
    flags += settings.m_drawShapes * b2Draw::e_shapeBit;
    flags += settings.m_drawJoints * b2Draw::e_jointBit;
    flags += settings.m_drawAABBs * b2Draw::e_aabbBit;
    flags += settings.m_drawCOMs * b2Draw::e_centerOfMassBit;
    g_debugDraw.SetFlags(flags);
    world.DebugDraw();
    g_debugDraw.Flush();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(g_mainWindow);
    glfwPollEvents();

    // Throttle to cap at 60Hz. This adaptive using a sleep adjustment. This could be improved by
    // using mm_pause or equivalent for the last millisecond.
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> target(1.0 / 60.0);
    std::chrono::duration<double> timeUsed = t2 - t1;
    std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;
    if (sleepTime > std::chrono::duration<double>(0)) {
      std::this_thread::sleep_for(sleepTime);
    }

    std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
    frameTime = t3 - t1;
    // Compute the sleep adjustment using a low pass filter
    sleepAdjust = 0.9 * sleepAdjust + 0.1 * (target - frameTime);
  }

  // Cleanup
  g_debugDraw.Destroy();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(g_mainWindow);
  glfwTerminate();

  return 0;
}
