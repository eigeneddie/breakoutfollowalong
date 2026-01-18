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
//
//  https://yakvi.github.io/handmade-hero-notes/index.md.html?
//
// 3. Quick OG episode rundown
//  Day 001–004 (loop + backbuffer + animation)
//  Day 010–015 (timing + platform API + memory + debug I/O)
//  Day 021–023 (DLL hot reload + looped live edit)
//  Day 088 (push buffer rendering)
//  Day 112–113 (CPU model + perf counters)
//  Day 132 (asset streaming starts)

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

    RegisterClassA(&window_class);

    HWND window = CreateWindowExA(0, window_class.lpszClassName, "Breakout",
        WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);
}
