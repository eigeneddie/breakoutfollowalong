#include <windows.h>
// PERSONAL NOTE:
// 1. Compiling the code
//  to build program, run:
//  call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 
//  on the cmd prompt so you can build the code from using cl command
//  
// 2. OG Handmadehero notes
//  here's external resource indexint handmadehero episodes.
//  Use to pin down philosophy + core engine architecture patterns

//  https://yakvi.github.io/handmade-hero-notes/

/*
void main(){
    printf("hello sailor!\n");
}
*/


WNDPROC Wndproc;

LRESULT window_callback(HWND window, UINT message, WPARAM w_param, 
    LPARAM l_param)
{
    return DefWindowProcA(window, message, w_param, l_param);
}


int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    //printf("hello sailor!\n");
    

    WNDCLASSA window_class = {0};
    window_class.style = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = window_callback;

    window_class.lpszClassName = "Game_window_class";

    RegisterClassA(&window_callback);

    HWND window = CreateWindowExA(0, window_class.lpszClassName, "Breakout",
        WS_VISIBLE|

        DWORD     dwExStyle,
        LPCSTR    lpClassName,
        LPCSTR    lpWindowName,
        DWORD     dwStyle,
        int       X,
        int       Y,
        int       nWidth,
        int       nHeight,
        HWND      hWndParent,
        HMENU     hMenu,
        HINSTANCE hInstance,
        LPVOID    lpParam
        );
    
    */
}