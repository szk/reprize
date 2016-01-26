#include "Common.hpp"

#include "GLWin32Screen.hpp"

using namespace reprize;
using namespace vid;

// callback
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch(iMessage)
    {
    case WM_ACTIVATE:
	g_focus = true;
	break;
    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_SYSDEADCHAR:
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        break;
    case WM_COMMAND:
        break;
    //destroy
    case WM_DESTROY:
        g_finish = true;
        PostQuitMessage(0);
        break;
	case WM_QUERYDRAGICON:
            // if you need any icons, add some code here.
            break;
    default:
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
    }

    return 0L;
}

GLWin32Screen::GLWin32Screen(Win32DepInfo* depinfo_)
    : Screen(depinfo_), win32info(depinfo_)
{
    hinst = (win32info->get_hinst());
    hwnd = (win32info->get_hwnd());

//     width = SCREEN_WIDTH;
//     height = SCREEN_HEIGHT;
}

GLWin32Screen::~GLWin32Screen(void)
{
    HGLRC hRC = wglGetCurrentContext();
    hdc = wglGetCurrentDC();

    wglMakeCurrent(NULL, NULL);

    if(!hRC)
    { MessageBox(hwnd, TEXT("Release HGLRC Failed...."), TEXT("ERROR"),MB_OK); }
    else { wglDeleteContext(hRC); }
    if(!hdc)
    { MessageBox(hwnd, TEXT("Release HDC Failed...."), TEXT("ERROR"), MB_OK); }
    else { ReleaseDC(hwnd, hdc); }
}

void GLWin32Screen::init(void)
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hIcon = create_icon();
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hinst;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = TEXT("Window");
    RegisterClass(&wc);
    int width, height;
    // check the window size
    if (FULLSCREEN)
    {
	// if now state is fullscreen, go ahead
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
    }
    else
    {
        // if now state is window mode, add frame size
        width = SCREEN_WIDTH + GetSystemMetrics(SM_CXEDGE);
        height = SCREEN_HEIGHT + GetSystemMetrics(SM_CYEDGE)
            + GetSystemMetrics(SM_CYCAPTION);
    }

    // create window (don't appear it yet
    hwnd = CreateWindow(TEXT("WINDOW"),
                        TEXT("reprize"),
                        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        width,
                        height,
                        NULL,
                        NULL,
                        hinst,
                        NULL);

    win32info->set_hwnd(hwnd);

    // create rendering context
    HGLRC hRC;
    int pixelformat;

    static PIXELFORMATDESCRIPTOR pfd = 
	{
	    sizeof(PIXELFORMATDESCRIPTOR),	// stract's size
	    1,					//version of opengl
	    PFD_DRAW_TO_WINDOW |		//Window style
	    PFD_SUPPORT_OPENGL |		//OpenGL
	    PFD_DOUBLEBUFFER,			//enable doublebuffer
	    PFD_TYPE_RGBA,              // RGBA
	    16,		                //color counts
	    0, 0,			//R
	    0, 0,			//G
	    0, 0,			//B
	    0, 0,			//A
	    0,				//accumulation buffer
	    0, 0, 0, 0, 		//RGBA accumulation buffer
	    16,				//Z buffer
	    0,				//stencil buffer
	    0,				//not use
	    PFD_MAIN_PLANE,		//layer type
	    0,				//reserved
	    0, 0, 0			//layer masks
	};

    //device context
    if (!(hdc = GetDC(hwnd)))
    {
        MessageBox(hwnd, TEXT("Getting HDC Failed"), TEXT("Error"), MB_OK);
        return;
    }
    //choose pixelformat
    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0)
    {
        MessageBox(hwnd, TEXT("ChoosePixelFormat Failed"),
                   TEXT("Error"), MB_OK);
        return;
    }
    //set pixel format
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
    {
        MessageBox(hwnd, TEXT("SetPixelFormat Failed"), TEXT("Error"), MB_OK);
        return;
    }
    //check rendering context
    if (!(hRC = wglCreateContext(hdc)))
    {
        MessageBox(hwnd, TEXT("Creating HGLRC Failed"), TEXT("Error"), MB_OK);
        return;
    }

    // create rendering context
    wglMakeCurrent(hdc, hRC);
}

void GLWin32Screen::appear(void)
{
    //show the window
    ShowWindow(hwnd, SW_SHOW );
    UpdateWindow(hwnd);
}

void GLWin32Screen::begin_paint(void)
{
    hdc = BeginPaint(hwnd, &ps);
}

void GLWin32Screen::finish_paint(void)
{
    hdc = wglGetCurrentDC();
}

const bool GLWin32Screen::flip(void)
{
    SwapBuffers(hdc);
    EndPaint(hwnd, &ps);
    return true;
}

void GLWin32Screen::release(void)
{
}

HICON GLWin32Screen::create_icon(void)
{
    HDC hMemDC;
    DWORD dwWidth, dwHeight;
    BITMAPV5HEADER bi;
    HBITMAP hBitmap, hOldBitmap;
    void *lpBits;
    DWORD x,y;
    HICON hAlphaIcon = NULL;

    dwWidth = 32;  // width of cursor
    dwHeight = 32;  // height of cursor

    ZeroMemory(&bi,sizeof(BITMAPV5HEADER));
    bi.bV5Size            = sizeof(BITMAPV5HEADER);
    bi.bV5Width           = dwWidth;
    bi.bV5Height          = dwHeight;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    // The following mask specification specifies a supported 32 BPP
    // alpha format for Windows XP.
    bi.bV5RedMask   =  0x00FF0000;
    bi.bV5GreenMask =  0x0000FF00;
    bi.bV5BlueMask  =  0x000000FF;
    bi.bV5AlphaMask =  0xFF000000;

    HDC icon_hdc;
    icon_hdc = GetDC(NULL);

    // Create the DIB section with an alpha channel.
    hBitmap = CreateDIBSection(icon_hdc,
                               (BITMAPINFO *)&bi, DIB_RGB_COLORS, 
                               (void **)&lpBits, NULL, (DWORD)0);

    hMemDC = CreateCompatibleDC(icon_hdc);
    ReleaseDC(NULL, icon_hdc);

    // Draw something on the DIB section.
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    PatBlt(hMemDC,0,0,dwWidth,dwHeight,WHITENESS);
    SetTextColor(hMemDC,RGB(0,0,0));
    SetBkMode(hMemDC,TRANSPARENT);
    TextOut(hMemDC, 0, 9, "rgba", 4);
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

    // Create an empty mask bitmap.
    HBITMAP hMonoBitmap = CreateBitmap(dwWidth,dwHeight,1,1,NULL);

    // Set the alpha values for each pixel in the icon so that
    // the complete icon is semi-transparent.
    DWORD *lpdwPixel;
    lpdwPixel = (DWORD *)lpBits;
    for (x = 0; x < dwWidth; x++)
    {
       for (y = 0; y < dwHeight; y++)
       {
           // Clear the alpha bits
           *lpdwPixel &= 0x00FFFFFF;
           // Set the alpha bits to 0x9F (semi-transparent)
           *lpdwPixel |= 0x9F000000;
           lpdwPixel++;
       }
    }

    ICONINFO ii;
    ii.fIcon = FALSE;// Change fIcon to TRUE to create an alpha icon
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = hMonoBitmap;
    ii.hbmColor = hBitmap;

    // Create the alpha cursor with the alpha DIB section.
    hAlphaIcon = CreateIconIndirect(&ii);

    DeleteObject(hBitmap);
    DeleteObject(hMonoBitmap);

    return hAlphaIcon;
}
