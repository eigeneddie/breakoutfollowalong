internal int
clamp(int min, int val, int max){
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

// Vector 2
typedef struct{
    union{
        struct{
            f32 x;
            f32 y;   
        };
        f32 e[2];
    };
} v2;


// Vector 2 (int)
typedef struct{
    union{
        struct{
            int x;
            int y;   
        };
        int e[2];
    };
} v2i;