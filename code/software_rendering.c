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

global_variable f32 scale = 0.01f;

inline f32
calculate_aspect_multiplier(){
    // use same multiplier for both axes so position isn't stretched
    // = width normally, but clamped if window is taller than aspect ratio
    f32 aspect_multiplier = (f32) render_buffer.height;
    if ((f32)render_buffer.width / (f32)render_buffer.height < WINDOW_ASPECT_RATIO){
        aspect_multiplier = (f32)render_buffer.width / WINDOW_ASPECT_RATIO;
    }
    return aspect_multiplier;
}

internal v2
pixels_to_world(v2i pixels_coord){
    v2 result;
    f32 aspect_multiplier = calculate_aspect_multiplier();
    // the offset by screen width/height is implying the origin is at the center of the screen
    result.x = (f32)pixels_coord.x - (f32)render_buffer.width*.5f;
    result.y = (f32)pixels_coord.y - (f32)render_buffer.height*.5f;
    
    result.x /= aspect_multiplier;
    result.x /= scale;

    result.y /= aspect_multiplier;
    result.y /= scale;
    
    return result;
}

internal void
draw_rect(v2 p, v2 half_size, u32 color){
    // p: center in game units, (0,0) = screen center
    // half_size: half width/height in game units. 1 unit = 1% of screen (scale=0.01)

    // pixel_size = game_units * screen_dimension * scale
    
    half_size.x *= (f32)render_buffer.width * scale;
    half_size.y *= (f32)render_buffer.height * scale;

    f32 aspect_multiplier = calculate_aspect_multiplier();

    // pixel_pos = game_pos * aspect_multiplier * scale
    p.x *= aspect_multiplier * scale;
    p.y *= aspect_multiplier * scale;

    // shift origin from screen center to top-left: pixel_pos += screen_dimension * 0.5
    p.x += (f32)render_buffer.width * .5f;
    p.y += (f32)render_buffer.height * .5f;

    //f32 scale = 0.0001f;
    //half_size.x /= (f32)render_buffer.width * scale;
    //half_size.y /= (f32)render_buffer.height * scale;

    // corners: (x0,y0) = p - half_size,  (x1,y1) = p + half_size
    int x0 = (int)(p.x - half_size.x);
    int y0 = (int)(p.y - half_size.y);
    int x1 = (int)(p.x + half_size.x);
    int y1 = (int)(p.y + half_size.y);

    draw_rect_in_pixels(x0, y0, x1, y1, color);
}