#ifndef SHOOTING_H
#define SHOOTING_H

#include "splashkit.h"
#include <vector>

// Forward declaration of game_data
struct game_data;

/**
 * Structure representing the data associated with a projectile.
 */
struct projectile_data {
    circle shape;       // The shape of the projectile
    double angle;       // The angle of the projectile
    double speed;       // The speed of the projectile
    int owner;          // The owner of the projectile (0 for player, 1 for enemy)
};

// Function declarations

/**
 * Returns the position of a circle point on the ship sprite.
 * 
 * @param ship The sprite representing the ship.
 * @return The position of the circle point.
 */
point_2d circle_point_position(const sprite& ship);

/**
 * Creates a new projectile and adds it to the game's projectiles vector.
 * 
 * @param game The game data.
 * @param projectiles The vector of projectiles in the game.
 */
void create_projectile(game_data& game, std::vector<projectile_data>& projectiles);

/**
 * Updates the position of the projectile based on the time delta.
 * 
 * @param projectile The projectile to update.
 * @param time_delta The time delta between updates.
 */
void update_projectile(projectile_data& projectile, double time_delta);

/**
 * Draws the projectile on the screen.
 * 
 * @param projectile The projectile to draw.
 */
void draw_projectile(const projectile_data& projectile);


#endif  // SHOOTING_H
