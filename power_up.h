#ifndef POWER_UP_H
#define POWER_UP_H

#include "splashkit.h"
#include "lost_in_space.h"

// Add a forward declaration for game_data
struct game_data;

/**
 * Enum representing the different types of power-ups.
 */
enum power_up_kind {
    SHIELD, // provides temporary shield protection
    FUEL,   // replenishes fuel for the game's ship
    ROCKET, // gives the game a rocket boost
    TIME    // adds extra time to the game
};

/**
 * Structure representing a point with x and y coordinates.
 */
struct point {
    int x; // x-coordinate of a point
    int y; // y-coordinate of a point
};

/**
 * Structure representing power-up data.
 */
struct power_up_data {
    power_up_kind kind;         // The type of power-up
    sprite power_up_sprite;     // The sprite for the power-up
    bool is_visible;            // Visibility status of the power-up
    point position;             // Position of the power-up
    point velocity;             // Velocity of the power-up
};


/**
 * Returns the appropriate bitmap for the given power-up kind.
 *
 * @param kind The power-up kind.
 * @return The bitmap associated with the power-up kind.
 */
bitmap power_up_bitmap(power_up_kind kind);

/**
 * Creates a new power-up at the specified x and y coordinates.
 *
 * @param x The x-coordinate of the power-up.
 * @param y The y-coordinate of the power-up.
 * @return The newly created power-up data.
 */
power_up_data new_power_up(double x, double y);

/**
 * Draws the power-up sprite on the screen.
 *
 * @param power_up The power-up data to be drawn.
 */
void draw_power_up(const power_up_data &power_up);

/**
 * Removes the power-up at the given index from the power_ups vector.
 *
 * @param power_ups The vector of power-ups.
 * @param index The index of the power-up to be removed.
 */
void remove_power_up(std::vector<power_up_data> &power_ups, int index);

/**
 * Applies the effect of the power-up on the game.
 *
 * @param game The game data.
 * @param power_up The power-up data.
 */
void apply_power_up(game_data &game, power_up_data &power_up);

/**
 * Updates the position and velocity of the power-up.
 *
 * @param power_up The power-up data to be updated.
 */
void update_power_up(power_up_data &power_up);

/**
 * Adds a new power-up to the game.
 *
 * @param game The game data.
 */
void add_power_up(game_data &game);


#endif