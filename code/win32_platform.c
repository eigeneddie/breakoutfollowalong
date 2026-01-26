/* PERSONAL NOTE:
 1. Compiling the code
  to build program, run:
  call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 
  on the cmd prompt so you can build the code from using cl command
  
 2. OG Handmadehero notes
  here's external resource indexint handmadehero episodes.
  Use to pin down philosophy + core engine architecture patterns

  https://yakvi.github.io/handmade-hero-notes/index.md.html?

 3. Quick OG episode rundown
  Day 001–004 (loop + backbuffer + animation)
  Day 010–015 (timing + platform API + memory + debug I/O)
  Day 021–023 (DLL hot reload + looped live edit)
  Day 088 (push buffer rendering)
  Day 112–113 (CPU model + perf counters)
  Day 132 (asset streaming starts)
 
 4. For the windows platform layer code (win32_platform.c)
  it will heavily comment the code to indicate which MSDN Doc is used since it's 
  highly linked with interacting with windows, which needs specific functions/classes, etc.


 5. The main goal of this c file is 3 fold
  a. Show the WINDOW
  b. handle user INPUT
  c. RENDERING on the window
*/
  /*
// Test code
void main(){
    printf("hello sailor!\n");
}
*/

#include "utils.c"
#include "math.c"

#include "platform_common.c"
#include <windows.h>

typedef struct {
    // Platform non-specific part
    int width, height;
    u32 *pixels;

    // Platform specific part
    BITMAPINFO bitmap_info;

} Render_Buffer;

global_variable Render_Buffer render_buffer;

#include "software_rendering.c"
#include "game.c"

WNDPROC Wndproc;

// MSDN WNDPROC https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-callwindowproca
// Windows procedure: Callback function Windows call to deliver messages to the window
// This is a function pointer I give to Windows for IT to use it when something happen to a window in Win32.
// This is callback that is given to window_class.
internal LRESULT  
window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;
    switch(message){
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        } break;

        case WM_SIZE:{
            // 1. Get width and height
            // MSDN GetWindowRect https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowrect
            RECT rect;
            GetWindowRect(window, &rect);
            render_buffer.width = rect.right - rect.left;
            render_buffer.height = rect.bottom - rect.top;

            // 2. Allocate the buffer
            if(render_buffer.pixels){
                //free
                // MSDN virtualfree https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualfree
                VirtualFree(render_buffer.pixels, 0, MEM_RELEASE);
  
            }

            // MSDN VirtualAlloc https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
            render_buffer.pixels = VirtualAlloc(0, sizeof(u32)*render_buffer.width*render_buffer.height, 
                                    MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

            // 3. Fill bitmap info

            // MSDN bitmapinfoheader https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
            render_buffer.bitmap_info.bmiHeader.biSize = sizeof(render_buffer.bitmap_info.bmiHeader);
            render_buffer.bitmap_info.bmiHeader.biWidth = render_buffer.width;
            render_buffer.bitmap_info.bmiHeader.biHeight = render_buffer.height;
            render_buffer.bitmap_info.bmiHeader.biPlanes = 1;
            render_buffer.bitmap_info.bmiHeader.biBitCount = 32;
            render_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
        } break;

        default:{
            result = DefWindowProcA(window, message, w_param, l_param);
        }
    }
    return result;

}

// WINDOWS ENTRY POINT (the void main() for windows)
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    // STEP 1: Define the window_class
    WNDCLASSA window_class = {0}; 
    window_class.style = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = window_callback;
    window_class.lpszClassName = "Game_Window_Class";

    RegisterClassA(&window_class);

    // STEP 2: Create window and initialize values
    HWND window = CreateWindowExA(0, window_class.lpszClassName, "Random game!!",
        WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);
    HDC hdc = GetDC(window);

    Input input = {0};

    // STEP 3: Super loop while the program runs
    while (running){

        // PART 1: INPUT
        for (int i = 0; i < BUTTON_COUNT; i++) input.buttons[i].changed = false;

        MSG message;
        // MSDN PeekMessageA https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
        while(PeekMessageA(&message, window, 0, 0, PM_REMOVE)){ 
            // This is the Windows message pump.
            // this is how the app reads input events (keyboard, mouse, etc) from OS and hands it to Windows to handle normally [??]
            // PeekMessage is a queue of messages (key presses, mouse, resize, close, etc.). it pulls one message at a time.
            // PM_REMOVE --> remove it from queue when it's read
            // The while keeps going until the queue is empty
            //

            switch(message.message){
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                case WM_KEYDOWN:
                case WM_KEYUP:{
                    u32 vk_code = (u32)message.wParam;
                    b32 was_down = ((message.lParam & (1<<30)) != 0); // bit 30 in lParam means previous key state
                    b32 is_down = ((message.lParam & (1<<31)) == 0); // bit 31 in lParam means transition state

                    process_button(VK_LEFT, BUTTON_LEFT);
                    process_button(VK_RIGHT, BUTTON_RIGHT);
                    process_button(VK_UP, BUTTON_UP);
                    process_button(VK_DOWN, BUTTON_DOWN);

                } break;

                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
        }

        // PART 2: SIMULATION
        simulate_game(&input);

        // PART 3: RENDER

        // NOTES:
        // Buffer (memory) -> draw to (up to the game)
        // use the buffer (draw) -> StretchDIBits   

        // MSDN StretchDIBits https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
        StretchDIBits(hdc, 0, 0, render_buffer.width, render_buffer.height, 
            0, 0, render_buffer.width, render_buffer.height, render_buffer.pixels, &render_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
        /*
            HDC              hdc, // handle to device context
            int              xDest,
            int              yDest,
            int              DestWidth,
            int              DestHeight,
            int              xSrc,
            int              ySrc,
            int              SrcWidth,
            int              SrcHeight,
            const VOID       *lpBits,
            const BITMAPINFO *lpbmi,
            UINT             iUsage,
            DWORD            rop
        */

    }
}
