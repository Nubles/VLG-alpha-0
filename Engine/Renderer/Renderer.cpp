#include "Engine/Renderer/Renderer.h"

#include "Engine/Core/Logger.h"

#if defined(RW_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#endif

#include <stdexcept>

namespace rw::renderer {

namespace {

void DrawGrid(const rw::scene::SceneObject& object)
{
#if defined(RW_PLATFORM_WINDOWS)
    glColor3f(object.color.x, object.color.y, object.color.z);
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; ++i) {
        glVertex3f(static_cast<float>(i), 0.0F, -10.0F);
        glVertex3f(static_cast<float>(i), 0.0F, 10.0F);
        glVertex3f(-10.0F, 0.0F, static_cast<float>(i));
        glVertex3f(10.0F, 0.0F, static_cast<float>(i));
    }
    glEnd();
#endif
}

void DrawCube(const rw::scene::SceneObject& object)
{
#if defined(RW_PLATFORM_WINDOWS)
    const auto& p = object.transform.position;
    const auto& s = object.transform.scale;
    glPushMatrix();
    glTranslatef(p.x, p.y, p.z);
    glRotatef(object.transform.rotationEuler.y * 57.2957795F, 0.0F, 1.0F, 0.0F);
    glScalef(s.x, s.y, s.z);
    glColor3f(object.color.x, object.color.y, object.color.z);

    glBegin(GL_QUADS);
    glVertex3f(-0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);

    glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);

    glVertex3f(-0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);

    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F);

    glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, 0.5F);

    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);
    glEnd();

    glColor3f(0.02F, 0.02F, 0.025F);
    glBegin(GL_LINES);
    glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F); glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F); glVertex3f(-0.5F, -0.5F, 0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F); glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F); glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F); glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
    glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F); glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F); glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);
    glEnd();
    glPopMatrix();
#else
#endif
}

} // namespace

Renderer::~Renderer()
{
    Shutdown();
}

void Renderer::Initialize(rw::platform::Window& window)
{
#if defined(RW_PLATFORM_WINDOWS)
    auto* deviceContext = static_cast<HDC>(window.DeviceContext());
    m_deviceContext = deviceContext;

    PIXELFORMATDESCRIPTOR format = {};
    format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    format.nVersion = 1;
    format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    format.iPixelType = PFD_TYPE_RGBA;
    format.cColorBits = 32;
    format.cDepthBits = 24;
    format.iLayerType = PFD_MAIN_PLANE;

    const int pixelFormat = ChoosePixelFormat(deviceContext, &format);
    if (pixelFormat == 0 || SetPixelFormat(deviceContext, pixelFormat, &format) == FALSE) {
        throw std::runtime_error("Failed to set OpenGL pixel format.");
    }

    HGLRC context = wglCreateContext(deviceContext);
    if (context == nullptr || wglMakeCurrent(deviceContext, context) == FALSE) {
        throw std::runtime_error("Failed to create OpenGL context.");
    }

    m_renderContext = context;
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.06F, 0.075F, 0.10F, 1.0F);
    rw::core::Logger::Info("OpenGL renderer initialized.");
#else
    (void)window;
#endif
}

void Renderer::Shutdown()
{
#if defined(RW_PLATFORM_WINDOWS)
    if (m_renderContext != nullptr) {
        wglMakeCurrent(static_cast<HDC>(m_deviceContext), nullptr);
        wglDeleteContext(static_cast<HGLRC>(m_renderContext));
        m_renderContext = nullptr;
        m_deviceContext = nullptr;
    }
#endif
}

void Renderer::Render(rw::platform::Window& window, const rw::scene::Scene& scene, const rw::scene::Camera& camera)
{
#if defined(RW_PLATFORM_WINDOWS)
    BeginFrame(window);

    const float aspect = window.Height() > 0
        ? static_cast<float>(window.Width()) / static_cast<float>(window.Height())
        : 1.0F;

    glViewport(0, 0, window.Width(), window.Height());
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera.ProjectionMatrix(aspect).Data());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera.ViewMatrix().Data());

    for (const auto& object : scene.Objects()) {
        if (object.primitive == rw::scene::DebugPrimitive::Grid) {
            DrawGrid(object);
        } else if (object.primitive == rw::scene::DebugPrimitive::Cube) {
            DrawCube(object);
        }
    }

    EndFrame();
    window.SwapBuffers();
#else
    (void)window;
    (void)scene;
    (void)camera;
#endif
}

void Renderer::BeginFrame(rw::platform::Window& window)
{
#if defined(RW_PLATFORM_WINDOWS)
    (void)window;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
    window.ClearBlankScreen();
#endif
}

void Renderer::EndFrame()
{
}

} // namespace rw::renderer
