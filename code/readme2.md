# Breakout Game — Code Breakdown

This is a from-scratch game written in C targeting Windows directly (no engine, no framework).
The whole program is built around **three goals**:

1. Show a Window
2. Handle User Input
3. Render pixels to that window

---

## How the Files Fit Together

```
win32_platform.c         <- The entry point. Talks to Windows OS.
  |-- utils.c            <- Custom type aliases + global flags
  |-- math.c             <- Math types (vectors, clamp)
  |-- platform_common.c  <- Input structs shared between platform + game
  |-- software_rendering.c  <- Pixel drawing functions
  |-- game.c             <- Game logic (player movement, drawing)
```

All files are `#include`d directly into `win32_platform.c` — there is no linker step between them.
This is called a **unity build** (one translation unit).

---

## File-by-File Breakdown

The order below follows the flow of execution — starting at the Windows entry point, working through game logic, then down into the supporting utilities.

---

### `win32_platform.c` — The Entry Point (Windows Platform Layer)

This is where the program starts. All Windows-specific code lives here so the game logic can stay platform-independent. Everything else is `#include`d from this file.

#### The `Render_Buffer` Struct

```c
typedef struct {
    int width, height;
    u32 *pixels;             // The actual pixel data in memory
    BITMAPINFO bitmap_info;  // Windows needs this to display the pixels
} Render_Buffer;
```

This is the **back buffer** — a block of memory you draw into every frame. When the frame is done, you copy it to the screen all at once. This prevents flickering.

---

#### `window_callback()` — The Windows Message Handler

```c
internal LRESULT window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
```

Windows uses a **callback** pattern — you give Windows a function pointer, and Windows calls it whenever something happens to your window (resize, close, etc.).

| Message | What it means | What the code does |
|---|---|---|
| `WM_CLOSE` / `WM_DESTROY` | User closed the window | Sets `running = false` to exit the game loop |
| `WM_SIZE` | Window was resized | Re-allocates the pixel buffer to match the new size |
| Everything else | Any other Windows event | Passed to `DefWindowProcA` (Windows default handler) |

**On resize**, the code:
1. Gets the new width/height with `GetWindowRect()`
2. Frees the old pixel buffer with `VirtualFree()`
3. Allocates a new one with `VirtualAlloc()` — direct OS memory allocation, no `malloc()`
4. Fills in `BITMAPINFO` so Windows knows how to interpret the pixel format (32-bit RGB)

---

#### `WinMain()` — Program Start

`WinMain` is Windows' version of `main()`. It runs in three steps before entering the game loop.

**Step 1 — Register the Window Class**
```c
WNDCLASSA window_class = {0};
window_class.style = CS_HREDRAW | CS_VREDRAW;   // Redraw on resize
window_class.lpfnWndProc = window_callback;      // Our message handler
window_class.lpszClassName = "Game_Window_Class";
RegisterClassA(&window_class);
```
A "window class" is a template. You register it once, then use it to create actual windows.

**Step 2 — Create the Window**
```c
HWND window = CreateWindowExA(0, "Game_Window_Class", "Random game!!",
    WS_VISIBLE | WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, ...);
HDC hdc = GetDC(window);
```
- Creates a 1280×720 window titled "Random game!!"
- `HDC` (Handle to Device Context) is Windows' way of representing a drawable surface — you need it to blit pixels to the screen.

**Step 3 — The Game Loop**
```c
while (running) {
    // PART 1: INPUT — drain the Windows message queue
    // PART 2: SIMULATION — update game state
    // PART 3: RENDER — copy pixel buffer to screen
}
```
This is the heartbeat of the game. Every iteration is one frame.

**Input (inside the loop):**
```c
while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
    switch (message.message) {
        case WM_KEYDOWN: case WM_KEYUP: ...
            process_button(VK_LEFT, BUTTON_LEFT);
            ...
    }
}
```
`PeekMessageA` pulls one event off the OS queue at a time. Key events are decoded using `lParam` bit flags:
- **Bit 30** = was the key already held before this event? (held vs. fresh press)
- **Bit 31** = is the key transitioning up? (0 = pressed, 1 = released)

**Simulation:**
```c
f32 dt = 0.01666f;   // ~60fps fixed timestep
simulate_game(&input, dt);
```
Calls the game logic with the current input and a fixed time step. `0.01666` seconds = 1/60th of a second.

**Render:**
```c
StretchDIBits(hdc, 0, 0, render_buffer.width, render_buffer.height,
    0, 0, render_buffer.width, render_buffer.height,
    render_buffer.pixels, &render_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
```
`StretchDIBits` copies the pixel buffer from memory onto the actual screen. It's the very last step each frame — everything before it was just writing numbers into an array.

---

### `game.c` — Game Logic

This is where the actual game lives. It has no Windows code — it only reads `Input` and writes to `render_buffer` via the rendering functions.

```c
v2 player_p;  // Player position in game-space

internal void simulate_game(Input *input, f32 dt) {
    clear_screen(0x551100);                         // Dark red background
    if (is_down(BUTTON_LEFT))  player_p.x -= 20 * dt;
    if (is_down(BUTTON_RIGHT)) player_p.x += 20 * dt;
    if (is_down(BUTTON_DOWN))  player_p.y -= 20 * dt;
    if (is_down(BUTTON_UP))    player_p.y += 20 * dt;
    draw_rect(player_p, (v2){10, 10}, 0x00ff00);   // Green rectangle
}
```

- Called once per frame with the current input state and `dt` (delta time).
- Multiplying speed by `dt` makes movement **frame-rate independent** — the player travels the same distance per second regardless of how fast the loop runs.
- Colors are hex `0xRRGGBB`: `0x551100` = dark red, `0x00ff00` = bright green.

---

### `software_rendering.c` — Drawing Pixels

Draws directly to the pixel buffer in memory — no GPU, no DirectX, no OpenGL.

#### `clear_screen(color)`
```c
u32 *pixel = render_buffer.pixels;
for (int y ...) for (int x ...) { *pixel = color; pixel++; }
```
Walks every pixel and sets it to one color. Called at the start of each frame to wipe the previous frame.

#### `draw_rect_in_pixels(x0, y0, x1, y1, color)`
Draws a filled rectangle using raw pixel coordinates. Clamps all coordinates so you can't accidentally draw outside the buffer. The buffer is a flat 1D array — to reach row `y`, column `x`:
```c
pixel = render_buffer.pixels + x0 + render_buffer.width * y;
```

#### `draw_rect(position, half_size, color)`
A higher-level version that accepts **game-space coordinates** (centered, resolution-independent units) and converts them to pixel coordinates before calling `draw_rect_in_pixels`. Game logic uses this so it doesn't need to think in pixels.

---

### `platform_common.c` — Shared Input Definitions

Defines the input types used by both the platform layer and the game. Sitting in the middle, it's the contract between them.

```c
enum { BUTTON_LEFT, BUTTON_RIGHT, BUTTON_UP, BUTTON_DOWN, BUTTON_COUNT };

typedef struct { b32 is_down; b32 changed; } Button;

typedef struct { int mouse_x; int mouse_y; Button buttons[BUTTON_COUNT]; } Input;
```

- `Button` tracks two things per key: **is it held?** (`is_down`) and **did it change state this frame?** (`changed`).
- `Input` bundles all buttons + mouse into one struct that gets passed to `simulate_game` each frame.
- `process_button` is a macro that checks a virtual key code and updates the matching button's state.
- Three helper macros for reading input cleanly in game logic:
  - `pressed(b)` — key was just pressed this frame
  - `released(b)` — key was just released this frame
  - `is_down(b)` — key is currently held

---

### `math.c` — Math Utilities

```c
internal int clamp(int min, int val, int max){ ... }

typedef struct {
    union {
        struct { f32 x; f32 y; };
        f32 e[2];
    };
} v2;
```

- `clamp()` keeps a value between a min and max. Used by the renderer to prevent out-of-bounds pixel writes.
- `v2` is a 2D vector. The `union` trick lets you use `.x/.y` for readability or `e[0]/e[1]` for looping — same memory, two access styles.

---

### `utils.c` — Type Aliases and Global Flags

The foundation everything else builds on. Included first.

```c
typedef uint32_t u32;
typedef float f32;
typedef int b32;

#define global_variable static
#define internal static

global_variable b32 running = true;
```

- Renames C's built-in types to shorter, clearer names (`u32`, `f32`, `b32` for boolean).
- `internal` = file-private function (like `private` in other languages).
- `global_variable` = module-level global. Same as `static`, just more readable.
- `running` is the flag that drives the main loop — set to `false` to exit.

---

## The Big Picture — Frame Lifecycle

```
Every frame:
  1. Read Windows events → update Input struct       [win32_platform.c]
  2. Call simulate_game(input, dt)                   [game.c]
       - clear_screen() wipes last frame             [software_rendering.c]
       - Move player based on keys held
       - draw_rect() writes pixels to render_buffer  [software_rendering.c]
  3. StretchDIBits() pushes render_buffer to screen  [win32_platform.c]
  4. Repeat until running = false
```

---

## Key Concepts to Remember

| Concept | Where it appears | Why it matters |
|---|---|---|
| Back buffer | `Render_Buffer.pixels` | Draw offscreen, then flip — avoids tearing |
| Unity build | All `#include` in one `.c` | Simpler build, one `cl` command compiles everything |
| Delta time (`dt`) | `game.c` movement | Speed stays consistent regardless of frame rate |
| Virtual memory | `VirtualAlloc/VirtualFree` | Direct OS allocation — no heap overhead |
| Message pump | `PeekMessageA` loop | Required by Windows to process all OS events |
| Callback pattern | `window_callback` | Windows calls your function when events happen |
