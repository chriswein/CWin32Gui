#include <windows.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// compile with "gcc win.c -o win.exe -mwindows -municode"

HWND window;
HWND shiftRightButton,shiftLeftButton, cancelbutton, text;

void ShiftTextInTextfield(HWND textfield, int direction){
        long outLength = GetWindowTextLengthW(textfield) + 1;
        wchar_t *buffer = malloc(sizeof(wchar_t) * outLength);

        GetWindowTextW(text, (LPWSTR)buffer, outLength);

        for (size_t i = 0; i < outLength - 1; i++)
        {
            buffer[i] = (wchar_t)(buffer[i] + direction);
        }

        SetWindowTextW(textfield, buffer);
}

RECT getWindowSize()
{
    LPRECT lpr = malloc(sizeof(LPRECT));
    GetWindowRect(window, lpr);
    return *lpr;
}

/*
    Function for creating new buttons in the window
*/
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

/*
    Initialize the contents of the window
*/
void initializeComponents(HWND m_hwnd)
{
    RECT bounds = getWindowSize();
    long width = bounds.right - bounds.left;
    long height = bounds.bottom - bounds.top;
    long offset = 300;
    long offset_y = 151;

    shiftRightButton = createButton(L"Shift Right", width - offset, height - offset_y, 100l, 100l);
    shiftLeftButton = createButton(L"Shift Left", width-offset-110l, height- offset_y, 100l,100l);
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
    initializeComponents(window);
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

/*
    Handling button events
*/
void ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ((HWND)lParam == shiftRightButton)
    {
        ShiftTextInTextfield(text,1);
    }
    if ((HWND)lParam == shiftLeftButton){
        ShiftTextInTextfield(text,-1);
    }
    if ((HWND)lParam == cancelbutton)
    {
        PostQuitMessage(0);
    }
}

/*
    Main application message loop
*/
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
    break;
    case WM_CREATE:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);

        return 0;
    }
    break;
    case WM_SIZING:
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
