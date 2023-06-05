#ifndef LOST_IN_SPACE_H
#define LOST_IN_SPACE_H

#include "splashkit.h"
#include "player.h"
#include "planets.h"
#include "shooting.h"
#include "enemy.h"
#include "explosion.h" 
#include "game_data.h"
#include <cmath>
#include <ctime>
#include <algorithm>

// Forward declaration
struct enemy_data;
struct player_data;

// Constants for min coordinates of game world
const int MIN_X = -1500;
const int MAX_X = 1500;
const int MIN_Y = -1500;
const int MAX_Y = 1500;

/**
 * Updates the remaining time of the game based on the elapsed time since the last update.
 *
 * @param game The game to update.
 */
void update_game_time(game_data &game);

/**
 * Checks for collisions between the player and power-ups in the game.
 *
 * @param game The game to check for collisions in.
 */
void check_collisions(game_data &game);

/**
 * Adds a new power-up to the game's power-ups vector.
 *
 * @param game The game to add the power-up to.
 */
void add_power_up(game_data &game);

/**
 * Draws the game by drawing the player and power-up sprites and the HUD.
 *
 * @param game The game to draw.
 */
void draw_game(const game_data &game);

/**
 * Updates the game by updating the player and power-up data and checking for collisions.
 *
 * @param game The game to update.
 */
void update_game(game_data &game);

/**
 * Adds a new planet to the screen
 */
void add_planet(game_data &game);

/**
 * Check if a power-up should be spawned.
 *
 * @param game The game_data object containing the game state.
 * @return True if a power-up should be spawned, false otherwise.
 */
bool should_spawn_power_up(game_data &game);

/**
 * Add an explosion to the game at the specified position.
 *
 * @param game The game_data object containing the game state.
 * @param x The x-coordinate of the explosion.
 * @param y The y-coordinate of the explosion.
 */
void add_explosion(game_data &game, float x, float y);

/**
 * Update the state of an explosion.
 *
 * @param explosion The explosion_data object representing the explosion.
 */
void update_explosion(explosion_data &explosion);

/**
 * Update all explosions in the game and remove any expired explosions.
 *
 * @param game The game_data object containing the game state.
 */
void update_explosions(game_data &game);

/**
 * Add a new enemy to the game.
 *
 * @param game The game_data object containing the game state.
 */
void add_enemy(game_data &game);

/**
 * Spawn power-ups in the game.
 *
 * @param game The game_data object containing the game state.
 */
void spawn_power_ups(game_data &game);

/**
 * Spawn enemies in the game.
 *
 * @param game The game_data object containing the game state.
 */
void spawn_enemy(game_data &game);

/**
 * Update the start menu of the game.
 *
 * @param game The game_data object containing the game state.
 */
void update_start_menu(game_data &game);

/**
 * Update the game while it is in progress.
 *
 * @param game The game_data object containing the game state.
 * @param time_delta The time elapsed since the last update.
 */
void update_in_progress(game_data &game, double time_delta);

/**
 * Update the game state when the game is over.
 *
 * @param game The game_data object containing the game state.
 */
void update_game_over(game_data &game);

/**
 * Draw the start menu of the game.
 *
 * @param game The game_data object containing the game state.
 */
void draw_start_menu(const game_data &game);

/**
 * Draw the game in progress.
 *
 * @param game The game_data object containing the game state.
 */
void draw_in_progress(const game_data &game);

/**
 * Draw an explosion on the screen.
 *
 * @param explosion The explosion_data object representing the explosion.
 */
void draw_explosion(const explosion_data &explosion);


#endif
