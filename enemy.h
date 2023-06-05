#ifndef ENEMY_H
#define ENEMY_H

#include "splashkit.h"
#include "lost_in_space.h"
#include "shooting.h"
#include "player.h"
#include <vector>

// Forward declaration of the game_data struct
struct game_data;

// Enumeration for the different kinds or types of enemies in the game.
enum class enemy_kind {
    ENEMY_TYPE1,    // Represents enemy type 1.
    ENEMY_TYPE2,    // Represents enemy type 2.
    ENEMY_TYPE3,    // Represents enemy type 3.
    ENEMY_BOSS       // Represents the BOSS..
};

// Enumeration for the different types of damage inflicted by enemies.
enum damage_type {
    DAMAGE_TYPE1,   // Represents damage type 1.
    DAMAGE_TYPE2,   // Represents damage type 2.
    DAMAGE_TYPE3,    // Represents damage type 3.
    DAMAGE_BOSS     // Represents the BOSS
};

// Enumeration for the possible states or behaviors of enemies.
enum enemy_state {
    IDLE,           // The enemy is idle or not actively engaged in any behavior.
    PATROL,         // The enemy is patrolling a designated area.
    CHASE,          // The enemy is actively chasing the player.
    ATTACK          // The enemy is attacking the player.
};

// This struct represents the data and properties of an enemy character in the game.
struct enemy_data {
    sprite enemy_sprite;                   // The sprite representing the enemy.
    enemy_kind kind;                       // The type or kind of enemy.
    int last_projectile_tick;              // The tick count when the last projectile was fired.
    int health;                            // The current health of the enemy.
    double speed;                          // The movement speed of the enemy.
    double rotation_speed;                 // The rotation speed of the enemy.
    enemy_state state;                     // The state or behaviour of the enemy.
    double attack_start_time;              // The time when the attack started.
    std::vector<projectile_data> projectiles;  // A vector of projectiles fired by the enemy.
    circle shape;                          // The shape or hitbox of the enemy.
    damage_type type;                      // The type of damage inflicted by the enemy.
    float damage_value;                     // The value or amount of damage inflicted by the enemy.
    float original_damage;                  // The original damage value before any modifications.
    double distance_to_player;              // The distance between the enemy and the player.
    double angle_to_player;                 // The angle between the enemy and the player.
    double rotation_diff;                   // The difference in rotation between the enemy and the player.
    double new_x;                           // The new x-coordinate of the enemy after an update.
    double new_y;                           // The new y-coordinate of the enemy after an update.
    double adjusted_speed;                  // The adjusted speed of the enemy based on player speed.
};

/**
 * This function creates a new enemy with the specified kind and initializes its properties.
 * It takes the enemy kind, player data, and a vector of existing enemies as input.
 * The function creates an enemy_data struct, sets its properties based on the enemy kind,
 * generates a random position for the enemy, and sets its position and rotation in the sprite.
 * The newly created enemy is returned.
 *
 * @param kind The kind of enemy to create.
 * @param player The player data.
 * @param enemies The vector of existing enemies.
 * @return The newly created enemy.
 */
enemy_data new_enemy(enemy_kind kind, const player_data &player, const vector<enemy_data> &enemies);

/**
 * This function checks if a given position is a safe distance away from the player.
 * It takes the position, player sprite, and an optional minimum distance as input.
 * The function calculates the distance between the position and the center point of the player sprite
 * and returns true if the distance is greater than the minimum distance, indicating a safe distance.
 *
 * @param position The position to check.
 * @param player_sprite The player sprite.
 * @param min_distance The minimum distance (default: 200).
 * @return True if the position is a safe distance from the player, false otherwise.
 */
bool is_safe_distance_from_player(point_2d position, sprite player_sprite, double min_distance = 200);

/**
 * This function checks if a given position is a safe distance away from all other enemies.
 * It takes the position, a vector of existing enemies, and an optional minimum distance as input.
 * The function iterates over each enemy in the vector and calculates the distance between the position
 * and the center point of each enemy's sprite. If any distance is less than or equal to the minimum distance,
 * indicating a collision, the function returns false. Otherwise, it returns true, indicating a safe distance.
 *
 * @param position The position to check.
 * @param enemies The vector of existing enemies.
 * @param min_distance The minimum distance (default: 200).
 * @return True if the position is a safe distance from all other enemies, false otherwise.
 */
bool is_safe_distance_from_enemies(point_2d position, const vector<enemy_data> &enemies, double min_distance = 200);

/**
 * This function updates the orientation of the enemy to face the player.
 * It takes the enemy data and player data as input.
 * The function calculates the angle in degrees between the center point of the enemy's sprite
 * and the center point of the player's sprite using the angle_to function.
 * It then sets the rotation of the enemy's sprite to the calculated angle using sprite_set_rotation.
 *
 * @param enemy The enemy data.
 * @param player The player data.
 */
void update_enemy_orientation(enemy_data& enemy, const player_data& player);

/**
 * This function updates the position of the enemy based on the player's position and game data.
 * It takes the enemy data, player data, and game data as input.
 * The function adjusts the enemy's position relative to the player's position, considering the speed
 * and current state of the enemy. It calls other helper functions to fine-tune the speed, ensure the
 * enemy stays within the boundaries of the game world, and handle collision detection with the player.
 *
 * @param enemy The enemy data.
 * @param player The player data.
 * @param game The game data.
 */
void update_enemy_position(enemy_data& enemy, const player_data& player, game_data& game);

/**
 * This function adjusts the enemy's speed to be near the player's speed.
 * It takes the enemy data as input.
 * The function calculates an adjusted speed for the enemy based on the distance to the player.
 * If the enemy is too close to the player, the adjusted speed is reduced to avoid collisions.
 *
 * @param enemy The enemy data.
 */
void adjust_speed_near_player(enemy_data& enemy);

/**
 * This function updates the enemy's position to ensure it stays within the boundaries of the game world.
 * It takes the enemy data as input.
 * The function checks if the new position of the enemy is within the predefined minimum and maximum
 * values for the X and Y coordinates. If the new position exceeds the boundaries, the enemy is turned around.
 *
 * @param enemy The enemy data.
 */
void update_enemy_boundaries(enemy_data& enemy);

/**
 * This function handles collision detection between the enemy and the player.
 * It takes the enemy data and player data as input.
 * The function checks if the enemy is in close proximity to the player and adjusts the enemy's position
 * accordingly to avoid collisions. If a collision occurs, the enemy is rotated by 180 degrees.
 *
 * @param enemy The enemy data.
 * @param player The player data.
 */
void update_enemy_collision(enemy_data& enemy, const player_data& player);

/**
 * This function updates the overall behaviour of the enemy by calling various sub-functions.
 * It takes the enemy data, player data, and game data as input.
 * The function updates the state, orientation, rotation, position, speed, boundaries, collision,
 * and visual representation of the enemy, allowing it to interact with the player and game environment.
 *
 * @param enemy The enemy data.
 * @param player The player data.
 * @param game The game data.
 */
void update_enemy(enemy_data& enemy, const player_data& player, game_data& game);

/**
 * This function updates the rotation of the enemy to match the player's rotation.
 * It takes the enemy data, player data, and game data as input.
 * The function adjusts the rotation speed of the enemy based on the distance to the player and randomly
 * adds a slight variation to the rotation. It updates the rotation of the enemy's sprite and ensures
 * it faces the player's sprite using the sprite_turn_to_face function.
 *
 * @param enemy The enemy data.
 * @param player The player data.
 * @param game The game data.
 */
void update_enemy_rotation(enemy_data& enemy, const player_data& player, game_data& game);

/**
 * This function updates all enemies in the game.
 * It takes the game data as input.
 * The function iterates over each enemy in the game's enemies vector and calls the update_enemy function
 * to update the behaviour and properties of each enemy. It also updates the projectiles belonging to each enemy.
 *
 * @param game The game data.
 */
void update_enemies(game_data& game);

/**
 * This function draws a single enemy on the screen.
 * It takes the enemy data as input.
 * The function draws the visual representation of the enemy's sprite on the screen.
 *
 * @param enemy The enemy data.
 */
void draw_enemy(const enemy_data &enemy);

/**
 * This function draws all enemies on the screen.
 * It takes the game data as input.
 * The function iterates over each enemy in the game's enemies vector and calls the draw_enemy function
 * to draw each enemy on the screen.
 *
 * @param game The game data.
 */
void draw_enemies(const game_data &game);

/**
 * This function spawns all enemies in the game, either by creating new ones or respawning dead ones.
 * It takes the game data as input.
 * The function adds new enemy instances to the game's enemies vector, using the new_enemy function.
 * It ensures a variety of enemy types are spawned in the game.
 *
 * @param game The game data.
 */
void spawn_all_enemies(game_data& game);

/**
 * This function creates a new projectile for an enemy to fire at the player.
 * It takes the game data, player data, and enemy data as input.
 * The function checks the state of the enemy and the time since the last projectile was fired.
 * If the conditions are met, it calculates the position and angle for the new projectile and creates it.
 * The projectile is then added to the game's projectiles vector.
 *
 * @param game The game data.
 * @param player The player data.
 * @param enemy The enemy data.
 */
void create_enemy_projectile(game_data &game, const player_data& player, enemy_data& enemy);

/**
 * This function updates the behaviour and properties of projectiles.
 * It takes the vector of projectiles as input.
 * The function iterates over each projectile in the vector and calls the update_projectile function
 * to update the movement and lifetime of each projectile.
 *
 * @param projectiles The vector of projectiles.
 */
void update_enemies_projectiles(std::vector<projectile_data>& projectiles);

/**
 * This function draws the visual representation of projectiles on the screen.
 * It takes the vector of projectiles as input.
 * The function iterates over each projectile in the vector and calls the draw_projectile function
 * to draw each projectile on the screen.
 *
 * @param projectiles The vector of projectiles.
 */
void draw_enemies_projectiles(const std::vector<projectile_data>& projectiles);


#endif // ENEMY_H