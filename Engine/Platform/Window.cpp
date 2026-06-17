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
    HINSTANCE instance = nullptr;
    bool open = false;
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
        DestroyWindow(window);
        return 0;
    case WM_DESTROY:
        if (impl != nullptr) {
            impl->open = false;
            impl->handle = nullptr;
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

    const std::wstring className = ToWide(m_impl->className);

    WNDCLASSEXW windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = m_impl->instance;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
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

    core::Logger::Info("Window created.");
}

Window::~Window()
{
    if (m_impl && m_impl->handle != nullptr) {
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

    if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0) {
        input.SetEscapeDown(true);
        m_impl->open = false;
    }
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

#endif

} // namespace rw::platform
