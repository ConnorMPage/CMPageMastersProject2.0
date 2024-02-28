//Include files 

#include <windows.h>
#include <sstream>
#include "UserInputDevice.h"
#include "EngineTimer.h"
#include "EngineDevice.h"
#include "CommonGlobals.h"
#include <memory>
#include "Scene.h"


bool spin = false;
//Variable Declaration
bool gMouseOn = false;
// create a const for the class name
const auto sClassName = L"MastersProjClass";
//create a const for the window name
auto cWindowName = "CMPage_MastersProject - Frame Time: ";
const auto sWindowName = L"CMPage_MastersProject";

//forward declarations of functions 
HWND InitWindow(HINSTANCE hInstance, int nCmdShow, int wWidth, int wHeight);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void UpdateTitleFPS(HWND hwnd, float frameTime);

// Windows function entry point
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine, int nCmdShow)
{

    //defines the window size
    const int windowWidth = 1760;
    const int windowHeight = 990;

    // creates a HWND variable then executes the init window function
    HWND hWnd = InitWindow(hInstance, nCmdShow, windowWidth, windowHeight);
    //checks if hWnd is null if it is then exits the program
    if (hWnd == NULL)
    {
        throw std::runtime_error("Failure to create window");
        return 0;
    }
    std::unique_ptr<Scene> scene;
    //initialise DX3D 
    try
    {
        DX = std::make_unique<EngineDevice>(hWnd);
        scene = std::make_unique<Scene>();
    }
    catch (std::runtime_error a)
    {
        MessageBoxA(hWnd, a.what(), NULL, MB_OK);
        return 0;
    }

    EngineTimer fps;
    fps.StartTimer();

    //setting up mouse control
    InputInit();
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = 0;
    Rid[0].hwndTarget = 0;
    if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)  return 0;
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    gMouseOn = true;

   
    //Message Loop

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // Deal with messages
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (KeyHit(Key_Escape))  DestroyWindow(hWnd);
            if (KeyHit(Key_F9))  gMouseOn = !gMouseOn;
            if (KeyHit(Key_Q)) spin = !spin;
            float currentFPS = fps.GetTimeLap();
           
            scene->RenderScene();
            UpdateTitleFPS(hWnd, currentFPS);


        
        }
    }

    return msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance, int nCmdShow, int wWidth, int wHeight)
{

    SHSTOCKICONINFO standardIcon;
    standardIcon.cbSize = sizeof(standardIcon);
    if (SHGetStockIconInfo(SIID_APPLICATION, SHGSI_ICON, &standardIcon) != S_OK)
    {
        return NULL;
    }




    // Register window class
    WNDCLASSEX windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0; SIID_APPLICATION;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = standardIcon.hIcon;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = sClassName;
    windowClass.hIconSm = standardIcon.hIcon;

    // checks if the class is registered / if not registered than return null
    if (!RegisterClassEx(&windowClass))
    {
        return NULL;
    }


    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT rc = { 0, 0, wWidth, wHeight };
    AdjustWindowRect(&rc, windowStyle, FALSE);

    // Create window, the second parameter is the text that appears in the title bar at first
    HWND hwnd = CreateWindow(sClassName, sWindowName, windowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

    //checks if hwind is null
    if (hwnd != NULL)
    {
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
    }

    return hwnd;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ // the switch case is used to handle all of the important windows messages 
    switch (message)
    {
    case WM_DESTROY: //if a window is destroyed 
        PostQuitMessage(1);// quits the application
        break;
    case WM_LBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);


    }
    break;
    case WM_INPUT:
    {

        UINT dwSize;
        RAWINPUTHEADER header;
        GetRawInputData((HRAWINPUT)lParam, RID_HEADER, &header, &dwSize, sizeof(RAWINPUTHEADER));
        if (header.dwType == RIM_TYPEMOUSE)
        {
            RAWINPUT rawInput;
            dwSize = sizeof(RAWINPUT);
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &rawInput, &dwSize, sizeof(RAWINPUTHEADER));
            if (gMouseOn)  MouseMove(rawInput.data.mouse.lLastX, rawInput.data.mouse.lLastY);
        }
        break;
    }
    case WM_KEYDOWN:
        KeyDownEvent(static_cast<KeyCode>(wParam));
        break;

    case WM_KEYUP:
        KeyUpEvent(static_cast<KeyCode>(wParam));
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);//defualt case is to allow the defwindowproc to handle any other message
    }
    return 0;
}
void UpdateTitleFPS(HWND hwnd, float frameTime)
{

    const float fpsUpdate = 0.5f;
    static float totalFrameTime = 0;
    static int frameCount = 0;
    totalFrameTime += frameTime;
    ++frameCount;
    if (totalFrameTime > fpsUpdate)
    {

        float avgFrameTime = totalFrameTime / frameCount;
        std::ostringstream frameTimeMs;
        frameTimeMs.precision(2);//rounds to 2 decimal places
        frameTimeMs << std::fixed << avgFrameTime * 1000;
        std::string windowTitle = cWindowName + frameTimeMs.str() +
            "ms, FPS: " + std::to_string(static_cast<int>(1 / avgFrameTime + 0.5f));
        SetWindowTextA(hwnd, windowTitle.c_str());
        totalFrameTime = 0;
        frameCount = 0;
    }
}