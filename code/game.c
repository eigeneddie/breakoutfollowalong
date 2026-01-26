internal void
simulate_game(Input *input){
    clear_screen(0x551100);
    if (input->buttons[BUTTON_LEFT].is_down){
        draw_rect_in_pixels(20, 20, 50, 50, 0xffff00); // x0, y0, x1, y1, color
    }
}