#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "game_data.h"
#include "kill_streaks.h"
#include "shooting.h"
#include "enemy.h"

/**
 * Handle the collision between an enemy and a player projectile.
 *
 * @param game The game_data object containing the player, projectiles, enemies, and sound effects.
 * @param enemy The enemy_data object involved in the collision.
 * @param enemy_index The index of the enemy in the enemies vector.
 * @param projectile_index The index of the player projectile in the projectiles vector.
 */
void handle_enemy_collision(game_data &game, enemy_data &enemy, int enemy_index, int projectile_index);

/**
 * Check for collisions between the player's projectiles and enemies.
 * Calls the handle_enemy_collision function for each collision.
 *
 * @param game The game_data object containing the player, projectiles, and enemies.
 */
void check_player_projectile_collisions(game_data &game);

/**
 * Handle the collision between a projectile and the player.
 * Updates player's shield or fuel percentage based on the collision result,
 * checks if the player's fuel percentage has reached zero, and removes the projectile.
 *
 * @param game The game_data object containing the player, projectiles, and enemies.
 * @param i The index of the projectile involved in the collision.
 */
void handle_projectile_collision(game_data &game, int i);

/**
 * Check for collisions between enemy projectiles and the player.
 * Calls the handle_projectile_collision function for each collision.
 *
 * @param game The game_data object containing the player, projectiles, and enemies.
 */
void check_enemy_projectile_collisions(game_data &game);

/**
 * Handle the collision between the player and a power-up.
 * Applies the power-up effect to the player and removes the power-up from the game.
 *
 * @param game The game_data object containing the player and power-ups.
 * @param i The index of the power-up involved in the collision.
 */
void handle_power_up_collision(game_data &game, int i);

/**
 * Check for collisions between the player and power-ups.
 * Calls the handle_power_up_collision function for each collision.
 *
 * @param game The game_data object containing the player and power-ups.
 */
void check_player_power_up_collisions(game_data &game);

/**
 * Check for collisions between game entities.
 * Calls functions to handle player-power-up collisions, player-projectile collisions,
 * and enemy-projectile collisions.
 *
 * @param game The game_data object containing the player, projectiles, and power-ups.
 */
void check_collisions(game_data &game);


/**

* Check for collision between two circles.
* @param c1 The first circle to check.
* @param c2 The second circle to check.
* @return True if the circles intersect, False otherwise.
*/
bool circle_collision(const circle &c1, const circle &c2);


#endif // COLLISIONS_H
