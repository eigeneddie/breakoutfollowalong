# WORK IN PROGRESS GAME FROM SCRATCH

Mastering C by making a fully shippable computer game from scratch!
Will make my own assets and art


The code is compiled using a batch file `build.bat` that runs the `cl` command from Microsoft Visual Studios that is set up using `vcvarsall.bat`.

The batch file `vcvarsall.bat` is obtained from installing the community version. Choose the module "Desktop Development with C++."
Every time you want to build the code using `build.bat` on the command prompt, you must run `vcvarsall.bat` just once to enable the `cl` command. 
The location of `vcvarsall.bat` may be different if you installed Microsoft Visual Studios differently, but generally the following prompt should work off the bat.

```
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```
Unlike mainstream software development, we are compiling everything from scratch every time. No build tools like CMake or Ninja. 

# Important Artifacts to implement in Random Game development
Obviously, watching +600 episodes of handmadehero is inhumane. Instead, I asked chatGPT what would be the 80/20 principles of computer systems that handmadehero teaches so we can skim his insights. Below is the list of "artifacts" critical for getting down first-principle of computer systems 

## Tier 0 artifacts (core 80/20)

1. Build + run pipeline
    - one-command debug build, one-command release build
    - learn: compiler flags, linking, debug symbols, warnings-as-errors

2. Platform ↔ Game boundary
    - `platform` calls `GameUpdateAndRender()` (and later `GameGetSoundSamples()`)
    - game code is “pure” and only touches memory/input buffers passed in

3. Offscreen buffer (software framebuffer)
    - backbuffer struct: pixels, width, height, pitch
    - render by writing pixels + blitting to window

4. Game loop timing
    - frame timing + stable dt
    - optionally fixed timestep + accumulator

5. Input system with transitions
    - is_down + “half transitions”/changed
    - input is a snapshot per frame, not raw events sprinkled everywhere

6. Memory model
    - one big allocation
    - “permanent” + “transient” regions
    - simple arena allocator (alignment matters)

7. Debug layer
    - `ASSERT`, logging, debug counters/overlay
    - “make the machine visible”

8. Audio output path
    - platform audio callback / buffer fill
    - game generates samples into a buffer
    - basics: latency, sample rate, no allocations in callback

9. File I/O abstraction
    - platform provides read/write file functions
    - game can load assets without depending on OS headers everywhere

## Tier 1 artifacts (high leverage after the above)

10. Hot reload / live code reloading
    - DLL reload or equivalent
    - massively improves iteration speed + teaches dynamic linking boundaries

11. Math + coordinate conventions
    - 2D transforms, units, camera/view, fixed vs float decisions
    - avoids later “everything is inconsistent” pain

12. Basic game state architecture
    - GameState struct, initialization, reset, mode transitions
    - clean separation of “state” vs “render scratch”

13. Asset pipeline (minimum viable)
    - load bitmaps, pack assets, versioned formats
    - practical binary parsing practice

## Tier 2 artifacts (valuable but optional early)

14. Multithreading / job system
    - helps later but can be deferred

15. Profiling + performance passes
    - cycle counters, frame breakdowns
    - do once you have something “slow enough to matter”

16. Debug UI / dev console
    - great systems practice, not required initially

17. SIMD/intrinsics deep dives
    - only when you care about peak perf

## Keywords to search for on the episode list
- platform layer, win32 layer, SDL layer
- offscreen buffer, backbuffer, bitmap buffer
- game loop, timing, dt, fixed timestep
- input, controller, keyboard, half transitions
- memory, arena, allocator, permanent/transient
- debug, assert, instrumentation, overlay
- audio, sound, samples, wave, latency
- file I/O, asset loading, bitmap loading
- DLL reload, hot reload, live code