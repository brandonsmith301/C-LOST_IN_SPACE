#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "splashkit.h"
#include "game_data.h"
#include "explosion.h"


/**
 * Structure representing explosion data.
 * It holds information about the explosion's sprite, bitmap, start time, removal flag,
 * current frame index, and frame duration.
 */
struct explosion_data {
    sprite explosion_sprite;     // Sprite object representing the explosion
    bitmap explosion_bitmap;     // Bitmap associated with the explosion
    double start_time;           // Time when the explosion started
    bool should_remove;          // Flag indicating if the explosion should be removed
    int frame_index;             // Index of the current frame of the explosion animation
    double frame_duration;       // Duration of each frame in the explosion animation
};

/**
 * Add an explosion at the specified position.
 * It takes the game data, x-coordinate, and y-coordinate as input.
 * The function creates a new explosion by initializing the explosion data
 * with the provided position and adds it to the game data's explosions vector.
 *
 * @param game The game data.
 * @param x The x-coordinate of the explosion.
 * @param y The y-coordinate of the explosion.
 */
void add_explosion(game_data &game, float x, float y);

/**
 * Update an explosion's animation.
 * It takes the explosion data as input.
 * The function updates the explosion's frame index and checks if all frames have been shown,
 * marking the explosion for removal if necessary.
 *
 * @param explosion The explosion data.
 */
void update_explosion(explosion_data &explosion);

/**
 * Update all explosions in the game.
 * It takes the game data as input.
 * The function iterates through all explosions in the game's explosions vector,
 * updating each explosion's animation and removing any explosions marked for removal.
 *
 * @param game The game data.
 */
void update_explosions(game_data &game);

/**
 * Draw an explosion's current frame.
 * It takes the explosion data as input.
 * The function draws the current frame of the explosion's bitmap at the sprite's position.
 *
 * @param explosion The explosion data.
 */
void draw_explosion(const explosion_data &explosion);


#endif
