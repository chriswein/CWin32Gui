#include <windows.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// compile with "gcc win.c -o win.exe -mwindows -municode"

HWND window;
HWND okbutton, cancelbutton, text;

RECT windowSize()
{
    LPRECT lpr = malloc(sizeof(LPRECT));
    GetWindowRect(window, lpr);
    return *lpr;
}
HWND createButton(wchar_t *text, long x, long y, long width, long height)
{
    return CreateWindow(
        L"BUTTON",                                             // class identifier
        text,                                                  // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        x,                                                     // x position
        y,                                                     // y position
        width,                                                 // Button width
        height,                                                // Button height
        window,                                                // Parent window
        NULL,                                                  // No menu.
        (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE),
        NULL);
}

void init_components(HWND m_hwnd)
{
    RECT bounds = windowSize();
    long width = bounds.right - bounds.left;
    long height = bounds.bottom - bounds.top;
    long offset = 290;
    long offset_y = 150;

    okbutton = createButton(L"Ok", width - offset, height - offset_y, 100l, 100l);
    cancelbutton = createButton(L"Close Application", width - offset + 10l + 100l, height - offset_y, 130l, 100l);

    text = CreateWindowW(
        L"EDIT",
        L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
        0l, 0l, width - 17l, height - offset_y - 10l, m_hwnd, NULL, NULL, NULL);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR szCmdLine, int CmdShow)
{
    const unsigned short CLASS_NAME[] = L"Sample Window Class";
    int width, height;
    width = 800;
    height = 640;
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    window = CreateWindowEx(
        0,                                    // Optional window styles.
        CLASS_NAME,                           // Window class
        L"Boilerplate Win32 Api Application", // Window text
        // WS_POPUP,
        WS_OVERLAPPEDWINDOW,                                        // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // Size and position
        NULL,                                                       // Parent window
        NULL,                                                       // Menu
        hInstance,                                                  // Instance handle
        NULL                                                        // Additional application data
    );

    if (window == NULL)
    {
        return 0;
    }
    ShowWindow(window, SW_SHOWDEFAULT);
    init_components(window);
    UpdateWindow(window);
    
    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ((HWND)lParam == okbutton)
    {

        long outLength = GetWindowTextLength(text) + 1;
        wchar_t *tex = GlobalAlloc(GPTR, outLength * sizeof(wchar_t));
        GetWindowText(text, tex, outLength);

        for (size_t i = 0; i < outLength; i++)
        {
            tex[i] = (wchar_t)(tex[i] + 1);
        }

        SetWindowText(text, tex);
    }
    if ((HWND)lParam == cancelbutton)
    {
        PostQuitMessage(0);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    wchar_t msg[32];
    switch (uMsg)
    {

    case WM_COMMAND:
    {
        if (HIWORD(wParam) == BN_CLICKED)
        {
            ButtonProc(hwnd, uMsg, wParam, lParam);
        }
    }

    case WM_CREATE:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);

        return 0;
    }

    case WM_SIZING:
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
