#include "splashkit.h"
#include "lost_in_space.h"
#include "power_up.h"
#include "planets.h"
#include "shooting.h"

// The total number of power-up types
const int POWER_UP_TYPES = 6;

// The bonus amount applied to the player's attribute when a power-up is applied
const double POWER_UP_BONUS = 0.25;

// The maximum percentage value
const double MAX_PERCENT = 1.0;

// The extra time amount added to the player's remaining time when the extra time power-up is applied
const int EXTRA_TIME = 5;

/**
 * Generate a random power-up kind.
 * 
 * @return A random power-up kind selected from the available power-up types.
 */
power_up_kind random_power_up_kind() {
    // Convert the random number to a power-up kind using static_cast
    return static_cast<power_up_kind>(rnd(POWER_UP_TYPES));
}

/**
 * Returns the bitmap associated with a power-up kind.
 * 
 * @param kind The kind of power-up.
 * @return The bitmap of the power-up.
 */
bitmap power_up_bitmap(power_up_kind kind) {
    switch (kind) {
        case SHIELD: 
            return bitmap_named("shield");
        case ROCKET: 
            return bitmap_named("bullet");
        case FUEL: 
            return bitmap_named("fuel");
        default: 
            return bitmap_named("time");
    }
}

/**
 * Creates a new power-up object with the given coordinates.
 * 
 * @param x The x-coordinate of the power-up.
 * @param y The y-coordinate of the power-up.
 * @return The new power-up object.
 */
power_up_data new_power_up(double x, double y) {
    power_up_data result;

    // Choose a random power-up kind
    result.kind = random_power_up_kind();

    // Create a new sprite for the power-up
    result.power_up_sprite = create_sprite(power_up_bitmap(result.kind));

    // Set the initial position and velocity of the power-up
    sprite_set_position(result.power_up_sprite, {x, y});
    sprite_set_velocity(result.power_up_sprite, {rnd() * 4 - 2, rnd() * 4 - 2});

    result.is_visible = true; 

    return result;
}

/**
 * Draws a power-up object if it is visible.
 * 
 * @param power_up The power-up to draw.
 */
void draw_power_up(const power_up_data &power_up) {
    if (power_up.is_visible) {
        draw_sprite(power_up.power_up_sprite);
    }
}

/**
 * Apply the shield power-up effect to the player in the game.
 * 
 * @param game The game data to apply the shield power-up effect to.
 */
void apply_shield(game_data &game) {
    play_sound_effect("shield");
    game.player.shield_pct = std::min(MAX_PERCENT, game.player.shield_pct + POWER_UP_BONUS);
}

/**
 * Apply the rocket power-up effect to the player in the game.
 * 
 * @param game The game data to apply the rocket power-up effect to.
 */
void apply_rocket(game_data &game) {
    play_sound_effect("gunreload");  
    game.player.rocket_count++;
}

/**
 * Apply the fuel power-up effect to the player in the game.
 * 
 * @param game The game data to apply the fuel power-up effect to.
 */
void apply_fuel(game_data &game) {
    play_sound_effect("fuel");
    game.player.fuel_pct = std::min(MAX_PERCENT, game.player.fuel_pct + POWER_UP_BONUS);
}

/**
 * Apply the extra time power-up effect to the player in the game.
 * 
 * @param game The game data to apply the extra time power-up effect to.
 */
void apply_time(game_data &game) {
    play_sound_effect("yougotit");
    game.player.time_remaining += EXTRA_TIME;
}

/**
 * Apply the effect of a power-up to the game based on its kind.
 * 
 * @param game The game data to apply the power-up effect to.
 * @param power_up The power-up data specifying the kind of power-up.
 */
void apply_power_up(game_data &game, power_up_data &power_up) {
    switch (power_up.kind) {
        case SHIELD:
            apply_shield(game);
            break;
        case ROCKET:
            apply_rocket(game);
            break;
        case FUEL:
            apply_fuel(game);
            break;
        default:
            apply_time(game);
            break;
    }  
}

/**
 * Make a power-up bounce off the screen boundaries.
 * 
 * @param power_up The power-up data to bounce.
 */
void bounce_power_up(power_up_data &power_up) {
    const int MIN = -1300;
    const int MAX = 1300;

    if (power_up.position.x > MAX || power_up.position.x < MIN) {
        power_up.velocity.x = -power_up.velocity.x;
    }
    if (power_up.position.y > MAX || power_up.position.y < MIN) {
        power_up.velocity.y = -power_up.velocity.y;
    }
}

/**
 * Removes a power-up from the power-ups vector.
 * 
 * @param power_ups The vector of all active power-ups.
 * @param index The index of the power-up to remove.
 */
void remove_power_up(std::vector<power_up_data> &power_ups, int index) {
    power_ups.erase(power_ups.begin() + index);
}

/**
 * Update the position and behaviour of a power-up.
 * 
 * @param power_up The power-up data to update.
 */
void update_power_up(power_up_data &power_up) {
    update_sprite(power_up.power_up_sprite);
    bounce_power_up(power_up);
}

/**
 * Add a new power-up to the game at a random position.
 * 
 * @param game The game data to add the power-up to.
 */
void add_power_up(game_data &game) {
    // Generate random x and y positions within the range
    int x = rnd(MAX_X - MIN_X + 1) + MIN_X;
    int y = rnd(MAX_Y - MIN_Y + 1) + MIN_Y;

    // Create new power-up at the random position
    power_up_data power_up = new_power_up(x, y);

    // Add power-up to game's power_ups vector
    game.power_ups.push_back(power_up);
}