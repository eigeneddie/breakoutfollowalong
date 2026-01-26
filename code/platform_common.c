
// Button keys
enum{
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_UP,
    BUTTON_DOWN,

    BUTTON_COUNT,
};


// Button state
typedef struct{
    b32 is_down;
    b32 changed;
} Button;


//Mouse input
typedef struct {
    int mouse_x;
    int mouse_y;

    Button buttons[BUTTON_COUNT];
} Input;
