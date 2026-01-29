internal void
clear_screen (u32 color){
    u32 *pixel = render_buffer.pixels;
    
    for (int y = 0; y < render_buffer.height; y++){
        for (int x = 0; x < render_buffer.width; x++){
            *pixel = color; pixel++;
        }
    }
}

internal void 
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color){
    
    x0 = clamp(0, x0, render_buffer.width);
    y0 = clamp(0, y0, render_buffer.height);
    x1 = clamp(0, x1, render_buffer.width);
    y1 = clamp(0, y1, render_buffer.height);
    
    for (int y = y0; y < y1; y++){
        u32 *pixel = render_buffer.pixels + x0 + render_buffer.width*y;
        // Note that the pixels on screen is represented as 1D array
        for (int x = x0; x < x1; x++){
            *pixel = color; pixel++;
        }
    }
}


internal void
draw_rect(v2 p, v2 half_size, u32 color){
    // Convert the units to pixels and call draw_rect_in_pixels

    p.x += (f32)render_buffer.width * .5f;
    p.y += (f32)render_buffer.height * .5f;

    //f32 scale = 0.0001f;
    //half_size.x /= (f32)render_buffer.width * scale;
    //half_size.y /= (f32)render_buffer.height * scale;

    int x0 = (int)(p.x - half_size.x);
    int y0 = (int)(p.y - half_size.y);
    int x1 = (int)(p.x + half_size.x);
    int y1 = (int)(p.y + half_size.y);

    draw_rect_in_pixels(x0, y0, x1, y1, color); 
}