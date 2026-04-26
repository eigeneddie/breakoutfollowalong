inline b32 
aabb_vs_aabb(v2 p1, v2 half_size1, v2 p2, v2 half_size2){ // handles rect vs rect
    // NOTE:
    // Generally - 
    // p1 is player, 
    // p2 is ball
    return 
    p1.y + half_size1.y > p2.y - half_size2.y && // ball is lower than paddle
    p1.x + half_size1.x > p2.x - half_size2.x && // ball is on right side of paddle
    p1.x - half_size1.x < p2.x - half_size2.x ; // ball is on left side of paddle
}