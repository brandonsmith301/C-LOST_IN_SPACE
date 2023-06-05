#include "splashkit.h"
#include "collision.h"

/**
 * Handle the collision between the player projectile and an enemy.
 * Reduces the enemy's health, removes the enemy if its health reaches zero,
 * adds an explosion effect, increments the player's kill count, plays sound effects,
 * activates the ultimate kill streak if the kill count reaches 10, and removes the player projectile.
 *
 * @param game The game_data object containing the player, projectiles, enemies, and sound effects.
 * @param enemy The enemy_data object involved in the collision.
 * @param enemy_index The index of the enemy in the enemies vector.
 * @param projectile_index The index of the player projectile in the projectiles vector.
 */
void handle_enemy_collision(game_data &game, enemy_data &enemy, int enemy_index, int projectile_index) {
    // Play a sound effect for the collision
    play_sound_effect("hit");
    
    // Reduce the enemy's health based on the player's damage
    enemy.health -= game.player.damage;
    
    // Check if the enemy's health has reached zero
    if (enemy.health <= 0) {
        // Add an explosion effect at the enemy's position
        add_explosion(game, sprite_x(enemy.enemy_sprite), sprite_y(enemy.enemy_sprite));
        
        // Remove the enemy from the enemies vector
        game.enemies.erase(game.enemies.begin() + enemy_index);
        
        // Increment the player's kill count
        game.player.kill_count++;
        
        // Play sound effects based on the kill streak
        play_kill_streak_sound(game);
        
        // Check the kill count and activate the ultimate kill streak if not already active
        if (game.player.kill_count == 10 && !game.is_kill_streak_active) {
            activate_ultimate_kill_streak(game);
        }
        
        // Remove the player projectile
        game.projectiles.erase(game.projectiles.begin() + projectile_index);
    }
}

/**
 * Check for collisions between the player's projectiles and enemies.
 * Calls the handle_enemy_collision function for each collision.
 *
 * @param game The game_data object containing the player, projectiles, and enemies.
 */
void check_player_projectile_collisions(game_data &game) {
    // Loop through all projectiles in reverse order to handle collisions properly
    for (int i = game.projectiles.size() - 1; i >= 0; i--) {
        // Get a reference to the current projectile
        projectile_data &projectile = game.projectiles[i];
        
        // Check if the projectile was fired by the player
        if (projectile.owner == 0) {
            // Loop through all enemies in reverse order to handle collisions properly
            for (int j = game.enemies.size() - 1; j >= 0; j--) {
                // Get a reference to the current enemy
                enemy_data &enemy = game.enemies[j];
                
                // Check for collision between the player projectile and the enemy
                if (circle_collision(projectile.shape, sprite_collision_circle(enemy.enemy_sprite))) {
                    // Handle the collision between the player projectile and the enemy
                    handle_enemy_collision(game, enemy, j, i);
                    break;
                }
            }
        }
    }
}


/**
 * Handle the collision between a projectile and the player.
 * Plays a sound effect, reduces player's shield or fuel percentage based on the collision result,
 * checks if the player's fuel percentage has reached zero, and removes the projectile.
 *
 * @param game The game_data object containing the player, projectiles, and enemies.
 * @param i The index of the projectile involved in the collision.
 */
void handle_projectile_collision(game_data &game, int i) {
    // Play a sound effect for the collision
    play_sound_effect("hit");
    
    // Check if the player has a shield remaining
    if (game.player.shield_pct != 0.0) {
        // Reduce the player's shield percentage by 0.25
        game.player.shield_pct -= 0.15;
    } else {
        // Reduce the player's fuel percentage by the damage value of the enemy projectile
        game.player.fuel_pct -= game.enemies[i].damage_value;
    }
    
    // Check if the player's fuel percentage has reached zero
    if (game.player.fuel_pct <= 0.0) {
        // Call the game_over function to handle the game over state
        game_over(game);
    }
    
    // Remove the projectile from the projectiles vector
    game.projectiles.erase(game.projectiles.begin() + i);
}

/**
 * Check for collisions between enemy projectiles and the player.
 * Calls the handle_projectile_collision function for each collision.
 *
 * @param game The game_data object containing the player, projectiles, and enemies.
 */
void check_enemy_projectile_collisions(game_data &game) {
    // Iterate over the projectiles in reverse order to handle collisions properly
    for (int i = game.projectiles.size() - 1; i >= 0; i--) {
        // Check if the projectile belongs to the enemy and collides with the player
        if (game.projectiles[i].owner == 1 &&
            circle_collision(game.projectiles[i].shape, sprite_collision_circle(game.player.player_sprite))) {
            // Handle the collision between the projectile and the player
            handle_projectile_collision(game, i);
        }
    }
}

/**
 * Handle the collision between the player and a power-up.
 * Applies the power-up effect to the player and removes the power-up from the game.
 *
 * @param game The game_data object containing the player and power-ups.
 * @param i The index of the power-up involved in the collision.
 */
void handle_power_up_collision(game_data &game, int i) {
    // Get a reference to the power-up involved in the collision
    power_up_data &power_up = game.power_ups[i];
    
    // Apply the power-up effect to the player
    apply_power_up(game, power_up);
    
    // Remove the power-up from the power-ups vector
    remove_power_up(game.power_ups, i);
}

/**
 * Check for collisions between the player and power-ups.
 * Calls the handle_power_up_collision function for each collision.
 *
 * @param game The game_data object containing the player and power-ups.
 */
void check_player_power_up_collisions(game_data &game) {
    // Iterate over the power-ups in reverse order to handle collisions properly
    for (int i = game.power_ups.size() - 1; i >= 0; i--) {
        // Check if the player collides with a power-up
        if (sprite_collision(game.player.player_sprite, game.power_ups[i].power_up_sprite)) {
            // Handle the collision between the player and the power-up
            handle_power_up_collision(game, i);
        }
    }
}

/**
 * Check for collisions between game entities.
 * Calls functions to handle player-power-up collisions, player-projectile collisions,
 * and enemy-projectile collisions.
 *
 * @param game The game_data object containing the player, projectiles, and power-ups.
 */
void check_collisions(game_data &game) {
    // Check for collisions between the player and power-ups
    check_player_power_up_collisions(game);
    
    // Check for collisions between the player's projectiles and enemies
    check_player_projectile_collisions(game);
    
    // Check for collisions between enemy projectiles and the player
    check_enemy_projectile_collisions(game);
}

/**
 * Check if two circles are colliding.
 * @param c1 The first circle.
 * @param c2 The second circle.
 * @return True if the circles are colliding, false otherwise.
 */
bool circle_collision(const circle &c1, const circle &c2) {
    // Calculate the distance between the centers of the circles squared
    float distance_squared = pow(c1.center.x - c2.center.x, 2) + pow(c1.center.y - c2.center.y, 2);
    // Calculate the sum of the radii of the circles
    float radius_sum = c1.radius + c2.radius;
    // Check if the distance squared is less than or equal to the squared radius sum
    return distance_squared <= pow(radius_sum, 2);
}