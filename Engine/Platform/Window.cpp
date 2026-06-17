#include "Engine/Platform/Window.h"

#include "Engine/Core/Logger.h"

#if defined(RW_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <stdexcept>
#include <string>

namespace rw::platform {

#if defined(RW_PLATFORM_WINDOWS)

struct Window::Impl {
    HWND handle = nullptr;
    HDC deviceContext = nullptr;
    HINSTANCE instance = nullptr;
    bool open = false;
    int width = 0;
    int height = 0;
    std::string className;
};

namespace {

constexpr COLORREF kBlankColor = RGB(8, 10, 14);

std::wstring ToWide(const std::string& value)
{
    if (value.empty()) {
        return {};
    }

    const int required = MultiByteToWideChar(
        CP_UTF8, 0, value.c_str(), static_cast<int>(value.size()), nullptr, 0);
    if (required <= 0) {
        return L"Realmbound Wilds";
    }

    std::wstring result(static_cast<size_t>(required), L'\0');
    MultiByteToWideChar(
        CP_UTF8, 0, value.c_str(), static_cast<int>(value.size()),
        result.data(), required);
    return result;
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto* impl = reinterpret_cast<Window::Impl*>(
        GetWindowLongPtrW(window, GWLP_USERDATA));

    switch (message) {
    case WM_NCCREATE: {
        auto* createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        impl = reinterpret_cast<Window::Impl*>(createStruct->lpCreateParams);
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(impl));
        return TRUE;
    }
    case WM_CLOSE:
        if (impl != nullptr) {
            impl->open = false;
        }
        return 0;
    case WM_SIZE:
        if (impl != nullptr) {
            impl->width = LOWORD(lParam);
            impl->height = HIWORD(lParam);
        }
        return 0;
    case WM_DESTROY:
        if (impl != nullptr) {
            impl->open = false;
        }
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(window, message, wParam, lParam);
    }
}

} // namespace

Window::Window(const WindowConfig& config)
    : m_impl(std::make_unique<Impl>())
{
    m_impl->instance = GetModuleHandleW(nullptr);
    m_impl->className = "RealmboundWildsWindowClass";
    m_impl->width = config.width;
    m_impl->height = config.height;

    const std::wstring className = ToWide(m_impl->className);

    WNDCLASSEXW windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = m_impl->instance;
    windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    windowClass.hbrBackground = CreateSolidBrush(kBlankColor);
    windowClass.lpszClassName = className.c_str();

    if (RegisterClassExW(&windowClass) == 0) {
        const DWORD error = GetLastError();
        if (error != ERROR_CLASS_ALREADY_EXISTS) {
            throw std::runtime_error("Failed to register Win32 window class.");
        }
    }

    RECT windowRect = { 0, 0, config.width, config.height };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    const std::wstring title = ToWide(config.title);
    m_impl->handle = CreateWindowExW(
        0,
        className.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        m_impl->instance,
        m_impl.get());

    if (m_impl->handle == nullptr) {
        throw std::runtime_error("Failed to create Win32 window.");
    }

    m_impl->open = true;
    ShowWindow(m_impl->handle, SW_SHOW);
    UpdateWindow(m_impl->handle);
    m_impl->deviceContext = GetDC(m_impl->handle);

    core::Logger::Info("Window created.");
}

Window::~Window()
{
    if (m_impl && m_impl->handle != nullptr) {
        if (m_impl->deviceContext != nullptr) {
            ReleaseDC(m_impl->handle, m_impl->deviceContext);
            m_impl->deviceContext = nullptr;
        }
        DestroyWindow(m_impl->handle);
        m_impl->handle = nullptr;
    }
}

Window::Window(Window&&) noexcept = default;

Window& Window::operator=(Window&&) noexcept = default;

bool Window::IsOpen() const
{
    return m_impl && m_impl->open;
}

void Window::PollEvents(rw::input::InputState& input)
{
    MSG message = {};
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            m_impl->open = false;
            input.RequestQuit();
        }

        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    const bool escapeDown = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
    input.SetEscapeDown(escapeDown);
    if (escapeDown) {
        m_impl->open = false;
    }

    input.SetKeyDown(rw::input::Key::W, (GetAsyncKeyState('W') & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::A, (GetAsyncKeyState('A') & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::S, (GetAsyncKeyState('S') & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::D, (GetAsyncKeyState('D') & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::E, (GetAsyncKeyState('E') & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F1, (GetAsyncKeyState(VK_F1) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F2, (GetAsyncKeyState(VK_F2) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F3, (GetAsyncKeyState(VK_F3) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F4, (GetAsyncKeyState(VK_F4) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F5, (GetAsyncKeyState(VK_F5) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F6, (GetAsyncKeyState(VK_F6) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F7, (GetAsyncKeyState(VK_F7) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F8, (GetAsyncKeyState(VK_F8) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::F9, (GetAsyncKeyState(VK_F9) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::Up, (GetAsyncKeyState(VK_UP) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::Down, (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::Left, (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::Right, (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0);
    input.SetKeyDown(rw::input::Key::Shift,
        (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);
}

void Window::ClearBlankScreen()
{
    RECT rect = {};
    GetClientRect(m_impl->handle, &rect);

    HDC deviceContext = GetDC(m_impl->handle);
    HBRUSH brush = CreateSolidBrush(kBlankColor);
    FillRect(deviceContext, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(m_impl->handle, deviceContext);
}

void Window::SetTitle(const std::string& title)
{
    SetWindowTextW(m_impl->handle, ToWide(title).c_str());
}

void Window::SwapBuffers()
{
    ::SwapBuffers(m_impl->deviceContext);
}

int Window::Width() const
{
    return m_impl->width;
}

int Window::Height() const
{
    return m_impl->height;
}

void* Window::NativeHandle() const
{
    return m_impl->handle;
}

void* Window::DeviceContext() const
{
    return m_impl->deviceContext;
}

#else

struct Window::Impl {
};

Window::Window(const WindowConfig&)
{
    throw std::runtime_error("No platform window backend is available.");
}

Window::~Window() = default;
Window::Window(Window&&) noexcept = default;
Window& Window::operator=(Window&&) noexcept = default;
bool Window::IsOpen() const { return false; }
void Window::PollEvents(rw::input::InputState&) {}
void Window::ClearBlankScreen() {}
void Window::SetTitle(const std::string&) {}
void Window::SwapBuffers() {}
int Window::Width() const { return 0; }
int Window::Height() const { return 0; }
void* Window::NativeHandle() const { return nullptr; }
void* Window::DeviceContext() const { return nullptr; }

#endif

} // namespace rw::platform
