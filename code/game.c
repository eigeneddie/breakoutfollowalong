v2 player_p;

internal void
simulate_game(Input *input){
    clear_screen(0x551100);
    if (pressed(BUTTON_LEFT)) player_p.x -= 20;
    if (pressed(BUTTON_RIGHT)) player_p.x += 20;
    if (pressed(BUTTON_DOWN)) player_p.y -= 20;
    if (pressed(BUTTON_UP)) player_p.y += 20;   

    draw_rect(player_p, (v2){10, 10}, 0x00ff00);
}