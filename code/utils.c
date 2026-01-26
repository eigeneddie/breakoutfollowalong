#include <stdint.h>

typedef int32_t s8;
typedef uint32_t u8;

typedef int32_t s16;
typedef uint32_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int32_t s64;
typedef uint32_t u64;

typedef float f32;
typedef double f64;

typedef int b32;

#define true 1
#define false 0

#define global_variable static
#define internal static

global_variable b32 running = true;
