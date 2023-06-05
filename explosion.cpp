#include "splashkit.h"
#include "game_data.h"
#include "explosion.h"

/**
 * Create a new explosion at a given position.
 * It takes the game data, x-coordinate, and y-coordinate as input.
 * The function initialises the explosion data by creating an explosion sprite and bitmap,
 * setting the position, timing, frame data, and adds the explosion to the game data.
 *
 * @param game The game data.
 * @param x The x-coordinate of the explosion.
 * @param y The y-coordinate of the explosion.
 */
void add_explosion(game_data &game, float x, float y) {
    // Initialise explosion data
    explosion_data explosion;

    // Create explosion sprite and bitmap
    explosion.explosion_sprite = create_sprite("explode");
    explosion.explosion_bitmap = bitmap_named("explode");

    // Set the position of the explosion
    sprite_set_position(explosion.explosion_sprite, point_2d { x, y });

    // Initialise explosion timing and frame data
    explosion.start_time = current_ticks();
    explosion.should_remove = false;
    explosion.frame_index = 0;
    explosion.frame_duration = 300; // Set the frame duration (in milliseconds)

    // Add the explosion to the game data
    game.explosions.push_back(explosion);
}

/**
 * Update the frame index and removal status of an explosion.
 * It takes the explosion data as input.
 * The function calculates the current time and updates the frame index based on the elapsed time.
 * If all frames have been shown, the explosion is marked for removal.
 *
 * @param explosion The explosion data.
 */
void update_explosion(explosion_data &explosion) {
    // Get the current time
    double current_time = current_ticks();

    // Define the number of frames in the explosion animation
    const int NUMBER_OF_FRAMES = 300;

    // If enough time has passed, advance to the next frame
    if (current_time - explosion.start_time >= explosion.frame_duration) {
        explosion.start_time = current_time;
        explosion.frame_index++;

        // If all frames have been shown, mark the explosion for removal
        if (explosion.frame_index >= NUMBER_OF_FRAMES) { 
            explosion.should_remove = true;
        }
    }
}

/**
 * Update all explosions in the game.
 * It takes the game data as input.
 * The function iterates through all explosions in reverse order, updating each explosion.
 * If an explosion is marked for removal, it frees the sprite memory and removes it from the game data.
 *
 * @param game The game data.
 */
void update_explosions(game_data &game) {
    // Iterate through all explosions in reverse order
    for (int i = game.explosions.size() - 1; i >= 0; i--) {
        // Update the current explosion
        update_explosion(game.explosions[i]);

        // If the explosion is marked for removal, remove it
        if (game.explosions[i].should_remove) {
            // Free the sprite memory and erase it from the game data
            free_sprite(game.explosions[i].explosion_sprite);
            game.explosions.erase(game.explosions.begin() + i);
        }
    }
}

/**
 * Draw a single frame of the explosion.
 * It takes the explosion data as input.
 * The function gets the current position of the explosion,
 * defines the frame dimensions, selects the part of the bitmap to draw, and draws the bitmap at the explosion position.
 *
 * @param explosion The explosion data.
 */
void draw_explosion(const explosion_data &explosion) {
    // Get the current position of the explosion
    point_2d explosion_position = sprite_position(explosion.explosion_sprite);
    
    // Define the frame dimensions
    double frame_width = bitmap_width(explosion.explosion_bitmap);
    double frame_height = bitmap_height(explosion.explosion_bitmap);

    // Define the part of the bitmap to draw
    double x = frame_width * explosion.frame_index;
    double y = 0;

    // Create drawing options for the part of the bitmap
    drawing_options part_bmp_opts = option_part_bmp(x, y, frame_width, frame_height);

    // Draw the bitmap at the explosion position
    draw_bitmap(explosion.explosion_bitmap, explosion_position.x, explosion_position.y, part_bmp_opts);
}
