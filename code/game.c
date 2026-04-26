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

// Ball attributes
v2 ball_p; // position
v2 ball_dp; // speed (or delta position)
v2 ball_half_size; 

// Player attributes
v2 player_p;   
v2 player_dp;
v2 player_half_size; 

b32 initialized = false; // at startup, make it false


internal void
simulate_game(Input *input, f32 dt){

    // STEP 1: initialize values if not already
    if (!initialized){
        initialized = true;
        ball_dp.y = -10;
        ball_p.y = 10;
        ball_half_size = (v2){.75, .75};

        player_p.y = -28;
        player_half_size = (v2){10, 2};
    }

    // STEP 2: Locate player position (i.e. mouse position on screen)
    f32 player_new_x = pixels_to_world(input->mouse).x;
    player_dp.x = (player_new_x - player_p.x)/dt;
    player_p.x = player_new_x;

    // STEP 3: Locate ball position (based on basic kinematics)
    ball_p = add_v2(ball_p, mul_v2(ball_dp, dt));

    // STEP 4: collision detection between ball and player
    if (aabb_vs_aabb(player_p, player_half_size, ball_p, ball_half_size)){
        // ball reflection or something
        ball_dp.y *= -1; // basically change direction
        ball_dp.x += player_dp.x;
    }

    // STEP 5: clear screen and draw
    clear_screen(0x551100);
    draw_rect(ball_p, (v2){0.5, 0.5}, 0x00ffff);
    draw_rect((v2){player_p.x, -28}, (v2){10, 2}, 0x00ff00);
    
}