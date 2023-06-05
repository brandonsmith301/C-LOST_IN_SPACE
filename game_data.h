#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "splashkit.h"
#include "planets.h"
#include "shooting.h"
#include "enemy.h" 
#include "power_up.h"
#include <map>

// Forward declaration of different game data structures
struct enemy_data;      // Data related to enemies in the game
struct player_data;     // Data related to the player in the game
struct explosion_data;  // Data related to explosions in the game
struct power_up_data;   // Data related to power-ups in the game
struct projectile_data; // Data related to projectiles in the game

// Enumeration for different types of enemies
enum class enemy_kind;

// Enumeration for different states of the game
enum game_state {
    START_MENU,  // State when the game is at the start menu
    IN_PROGRESS, // State when the game is in progress
    GAME_OVER    // State when the game is over
};

/**
 * Creates a new game
 *
 * @return The newly created game.
 */
game_data new_game();

// Struct for holding all data related to a game instance
struct game_data {
    player_data player;                     // The player data
    vector<planet_data> planets;            // Vector to hold all the planets in the game
    vector<enemy_data> enemies;             // Vector to hold all the enemies in the game
    vector<projectile_data> projectiles;    // Vector to hold all the projectiles in the game
    vector<power_up_data> power_ups;        // Vector to hold all the power-ups in the game
    vector<explosion_data> explosions;      // Vector to hold all the explosions in the game
    std::map<enemy_kind, float> original_damage_values;  // Map to hold the original damage values of each enemy kind
    int map_width;                          // Width of the game map
    int map_height;                         // Height of the game map
    bool is_kill_streak_active;             // Flag to check if a kill streak is currently active
    double last_kill_streak_tick;           // Time of the last kill streak tick
    timer game_timer;                       // Timer for the game
    game_state state;                       // Current state of the game
    bool intro_music_started;               // Flag to check if the intro music has started
    unsigned int last_enemy_spawn_tick = 0;
    unsigned int enemy_spawn_interval = 3000; // 5 seconds in milliseconds
    unsigned int last_power_up_spawn_tick = 0;
    unsigned int power_up_spawn_interval = 10000; // 10 seconds in milliseconds
    unsigned int last_second_tick = 0;
};

/**
 * Handles the game over state
 *
 * @param game The game data.
 */
void game_over(game_data &game);

#endif // GAME_DATA_H
