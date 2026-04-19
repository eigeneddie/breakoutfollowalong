v2 player_p;

internal void
simulate_game(Input *input, f32 dt){
    clear_screen(0x551100);
    if (is_down(BUTTON_LEFT)) player_p.x -= 20 * dt;
    if (is_down(BUTTON_RIGHT)) player_p.x += 20 * dt;
    if (is_down(BUTTON_DOWN)) player_p.y -= 20 * dt;
    if (is_down(BUTTON_UP)) player_p.y += 20 * dt;   

    draw_rect(player_p, (v2){10, 10}, 0x00ff00);
}