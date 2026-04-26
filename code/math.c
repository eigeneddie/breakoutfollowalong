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
        f32 e[2]; // element
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


inline v2   // v2 struct addition
add_v2(v2 a, v2 b){
    return (v2){a.x + b.x, a.y + b.y};
}

inline v2  // v2 struct element wise multiplicatin
mul_v2(v2 a, f32 s){
    return (v2){a.x * s, a.y * s};
}