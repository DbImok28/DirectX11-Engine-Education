#include "WindowContainer.hpp"



//RenderWindow::RenderWindow(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
//: hInst(hInstance), window_title(window_title), window_title_wide(StringHelper::StringToWide(window_title)),
//window_class(window_class), window_class_wide(StringHelper::StringToWide(window_class)), width(width), height(height)
//{
//}
bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    hInst = hInstance;
    this->window_title = window_title;
    this->window_title_wide = StringHelper::StringToWide(window_title);
    this->window_class = window_class;
    this->window_class_wide = StringHelper::StringToWide(window_class);
    this->width = width;
    this->height = height;

    RegisterWindowClass();

    int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

    RECT wr;
    wr.left = centerScreenX;
    wr.top = centerScreenY;
    wr.right = wr.left + width;
    wr.bottom = wr.top + height;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    hWnd = CreateWindowEx(
        0,
        this->window_class_wide.c_str(),
        this->window_title_wide.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        wr.left, 
        wr.top,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL, NULL, hInst,
        pWindowContainer
    );
    if (hWnd == NULL)
    {
        ErrorLogger::Log(GetLastError(), "Error while creating window. CreateWindowEX. " + this->window_title);
        return false;
    }

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    return true;
}

bool RenderWindow::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (msg.message == WM_NULL)
    {
        if (!IsWindow(hWnd))
        {
            hWnd = NULL;
            UnregisterClass(window_class_wide.c_str(), hInst);
            return false;
        }
    }
    return true;
}

HWND RenderWindow::GetHWND() const noexcept
{
    return hWnd;
}

RenderWindow::~RenderWindow()
{
    if (hWnd != NULL)
    {
        UnregisterClass(window_class_wide.c_str(), hInst);
        DestroyWindow(hWnd);
    }
}

LRESULT CALLBACK HandleMsgRedirect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        return 0;
    }
    default:
    {
        WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);
    }
    }

}

LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
        if (pWindow == nullptr)
        {
            ErrorLogger::Log("Critical Error: pointer to WindowContainer is null. WM_NCCREATE.");
            exit(-1);
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void RenderWindow::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = HandleMessageSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, (LPTSTR)IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = this->window_class_wide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);
}
