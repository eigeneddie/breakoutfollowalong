v2 player_p;


/*
hello world initial platform layer ok

TODO:

1. initial platform layer (window, rendering, input, time) [OK]
2. basic gameplay exploration
3. engine improvements (audio, threading, bitmap)
4. more gameplay exploration
5. polish pass (engine and game)

*/
internal void
simulate_game(Input *input, f32 dt){
    
    f32 speed = 100;

    if (is_down(BUTTON_LEFT)) player_p.x -=  speed * dt;
    if (is_down(BUTTON_RIGHT)) player_p.x += speed * dt;
    if (is_down(BUTTON_DOWN)) player_p.y -=  speed * dt;
    if (is_down(BUTTON_UP)) player_p.y +=    speed * dt;   
    
    clear_screen(0x551100);
    draw_rect(player_p, (v2){1, 1}, 0x00ff00);
}