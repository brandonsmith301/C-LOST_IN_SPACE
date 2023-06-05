#include "splashkit.h"
#include "lost_in_space.h"
#include "player.h"
#include "planets.h"
#include "shooting.h"
#include "enemy.h"
#include <cmath>
#include <ctime>
#include <algorithm>
#include <unordered_map>


/**
* An unordered_map is used to map a ship_kind enum to a string representing the corresponding bitmap name.
* This is more efficient than using switch-case statements, especially for large numbers of ship kinds.
*/
const std::unordered_map<ship_kind, std::string> ship_bitmaps {
    { AQUARII, "aquarii" },
    { GLIESE, "gliese" },
    { PEGASI, "pegasi" }
};

/**
* Returns a bitmap corresponding to the given ship kind.
* This function retrieves the bitmap name from the ship_bitmaps unordered_map based on the given kind,
* and then uses bitmap_named() to retrieve the corresponding bitmap.
* @param kind The ship kind for which to retrieve the bitmap.
* @return The bitmap corresponding to the given ship kind.
*/
bitmap ship_bitmap(ship_kind kind) {
    return bitmap_named(ship_bitmaps.at(kind));
}

/**
 * Set the position of a sprite to the specified coordinates.
 * 
 * @param player_sprite The sprite to set the position of.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 */
void set_sprite_position(sprite &player_sprite, double x, double y) {
    sprite_set_x(player_sprite, x);
    sprite_set_y(player_sprite, y);
}

/**
 * Initialise the attributes of a player.
 * 
 * @param player The player data to initialise.
 */
void player_attributes(player_data &player) {
    player.damage = 100;
    player.fuel_pct = 1.0;
    player.shield_pct = 0.0;
    player.rocket_count = 25;
    player.has_moved = false;
    player.time_remaining = 120;
    player.kill_count = 0;
    player.player_boost = 6.0;
    player.player_speed = 2.0;
}


/**
 * Create a new player and initialise  its properties.
 * 
 * @return The newly created player data.
 */
player_data new_player() {
    player_data result;
    bitmap default_bitmap = ship_bitmap(AQUARII);

    // Create the sprite with 3 layers - we can turn on and off based on the ship kind selected
    result.player_sprite = create_sprite(default_bitmap);

    // Add each ship kind as a separate layer to the player sprite
    sprite_add_layer(result.player_sprite, ship_bitmap(GLIESE), "GLIESE");
    sprite_add_layer(result.player_sprite, ship_bitmap(PEGASI), "PEGASI");

    // Default to layer 0 = Aquarii so hide others
    sprite_hide_layer(result.player_sprite, 1);
    sprite_hide_layer(result.player_sprite, 2);

    result.kind = AQUARII;

    // Get the width and height of the sprite
    int width = sprite_width(result.player_sprite);
    int height = sprite_height(result.player_sprite);

    // Position the sprite in the center of the initial screen
    set_sprite_position(result.player_sprite, (800 - width) / 2, (800 - height) / 2);

    // initialise  attributes
    player_attributes(result);

    return result;
}

 
/**
 * Update the player's position based on rotation and velocity.
 * Test if the player's sprite is within the screen boundaries and adjust it if necessary.
 *
 * @param player The player data to update.
 */
void update_player_position(player_data& player) {
    // Apply movement based on rotation and velocity in the sprite
    update_sprite(player.player_sprite);

    point_2d sprite_position = { sprite_x(player.player_sprite), sprite_y(player.player_sprite) };

    // Test if the sprite is within the screen boundaries and adjust it if necessary
    if (sprite_position.x < MIN) {
        sprite_set_x(player.player_sprite, MIN);
    } else if (sprite_position.x > MAX) {
        sprite_set_x(player.player_sprite, MAX);
    }

    if (sprite_position.y < MIN) {
        sprite_set_y(player.player_sprite, MIN);
    } else if (sprite_position.y > MAX) {
        sprite_set_y(player.player_sprite, MAX);
    }
}


/**
 * Adjust the camera position based on the player's position.
 * Tests the edge of the screen boundaries to adjust the camera, ensuring the player stays in view.
 *
 * @param player The player data.
 */
void adjust_camera_position(const player_data& player) {
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER;
    double top_edge = camera_y() + SCREEN_BORDER;
    double bottom_edge = top_edge + screen_height() - 2 * SCREEN_BORDER;

    point_2d sprite_center = center_point(player.player_sprite);

    // Test if the player is outside the area and move the camera accordingly
    if (sprite_center.y < top_edge) {
        move_camera_by(0, sprite_center.y - top_edge);
    } else if (sprite_center.y > bottom_edge) {
        move_camera_by(0, sprite_center.y - bottom_edge);
    }

    if (sprite_center.x < left_edge) {
        move_camera_by(sprite_center.x - left_edge, 0);
    } else if (sprite_center.x > right_edge) {
        move_camera_by(sprite_center.x - right_edge, 0);
    }
}

/**
 * Update the player's position and adjust the camera position.
 *
 * @param player_to_update The player data to update.
 * @param time_delta The time difference between updates.
 */
void update_player(player_data& player_to_update, const double time_delta) {
    update_player_position(player_to_update);
    adjust_camera_position(player_to_update);
}

/**
 * Draws the player sprite on the screen.
 * @param player_to_draw The player to draw.
 */
void draw_player(const player_data &player_to_draw) {
    draw_sprite(player_to_draw.player_sprite);
}

/**
 * Switch the ship kind - will adjust which layer is hidden/shown
 * @param player The player_data object to update.
 * @param target The target ship_kind to switch to.
 */
void player_switch_to_ship(player_data& player, ship_kind target) {
    // Only do this if there is a change in ship kind
    if (player.kind != target) {
        // Show then hide layers
        sprite_show_layer(player.player_sprite, static_cast<int>(target));
        sprite_hide_layer(player.player_sprite, static_cast<int>(player.kind));

        // Remember the current ship kind
        player.kind = target;
    }
}

/**
 * Handle player ship switching based on keyboard input.
 * Allows the player to switch ships by pressing the corresponding number keys.
 *
 * @param game The game data.
 */
void handle_ship_switch(game_data &game) {
    if (key_typed(NUM_1_KEY))
        player_switch_to_ship(game.player, AQUARII);
    if (key_typed(NUM_2_KEY))
        player_switch_to_ship(game.player, GLIESE);
    if (key_typed(NUM_3_KEY))
        player_switch_to_ship(game.player, PEGASI);
}

/**
 * Handle player rotation based on keyboard input.
 * Allows rotation with the left and right arrow keys.
 *
 * @param game The game data.
 */
void handle_rotation(game_data &game) {
    float rotation = sprite_rotation(game.player.player_sprite);
    if (key_down(LEFT_KEY)) {
        sprite_set_rotation(game.player.player_sprite, rotation - PLAYER_ROTATE_SPEED);
        game.player.current_rotation = rotation - PLAYER_ROTATE_SPEED;
    }
    if (key_down(RIGHT_KEY)) {
        sprite_set_rotation(game.player.player_sprite, rotation + PLAYER_ROTATE_SPEED);
        game.player.current_rotation = rotation + PLAYER_ROTATE_SPEED;
    }
}


/**
 * Handle player movement based on keyboard input.
 * Increases speed when the up key is pressed and decreases speed when it's not pressed.
 *
 * @param game The game data.
 */
void handle_movement(game_data &game) {
    if (key_down(UP_KEY)) {
        sprite_set_dx(game.player.player_sprite, game.player.player_boost);
        game.player.has_moved = true;
    } else {
        sprite_set_dx(game.player.player_sprite, game.player.player_speed);
        game.player.has_moved = true;
    }
}

/**
 * Handle firing projectiles based on keyboard input.
 * Fires a projectile when the space bar is pressed, if the player has enough rockets.
 *
 * @param game The game data.
 */
void handle_fire(game_data &game) {
    if (key_typed(SPACE_KEY)) {
        if (game.player.rocket_count > 0) {
            play_sound_effect("laser");
            create_projectile(game, game.projectiles);
            game.player.rocket_count--;
        } else {
            play_sound_effect("noammo");
        }
    }
}

/**
 * Handle player input by calling individual input handling functions.
 *
 * @param game The game data.
 */
void handle_input(game_data &game) {
    handle_ship_switch(game);
    handle_rotation(game);
    handle_movement(game);
    handle_fire(game);
}
