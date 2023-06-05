#include "splashkit.h"
#include "enemy.h"
#include "game_data.h"
#include "player.h"
#include "lost_in_space.h"
#include "power_up.h"
#include "planets.h"
#include "shooting.h"
#include "kill_streaks.h"
#include "explosion.h"
#include "collision.h"

/**
 * Update the game time remaining by decrementing it by the elapsed time
 * since the last frame. 
 *
 * @param game The game_data object to update.
 */
void update_game_time(game_data &game) {
    // Check if a second has passed since the last time the timer ticked
    if (timer_ticks(game.game_timer) - game.last_second_tick >= 1000) {
        // Decrement the remaining time for the player
        game.player.time_remaining--;
        // Update the last second tick to the current timer value
        game.last_second_tick = timer_ticks(game.game_timer);
    }

    // Check if the player has run out of time
    if (game.player.time_remaining <= 0) {
        // Set the game state to GAME_OVER
        game.state = game_state::GAME_OVER;
    }
}

/**
* Check if a power-up should spawn based on the elapsed time.
* @param game The game_data object containing the player and power-up data.
* @return True if a power-up should spawn, false otherwise.
*/
bool should_spawn_power_up(game_data &game) {
    // Check if the time interval for power-up spawning has passed
    return timer_ticks(game.game_timer) - game.last_power_up_spawn_tick >= game.power_up_spawn_interval;
}

/**
 * Update the start menu of the game.
 *
 * @param game The game_data object containing the game state.
 */
void update_start_menu(game_data &game) {
    // Start playing the introduction music if it hasn't started yet
    if (!game.intro_music_started) {
        play_music("introduction");
        game.intro_music_started = true;
    }

    // Start the game when Enter key is pressed
    if (key_typed(RETURN_KEY)) {
        play_sound_effect("letsgo");
        game.state = game_state::IN_PROGRESS;
        
        // Reset the intro_music_started variable
        game.intro_music_started = false;
        stop_music();
    }
}

/**
 * Update the game state when it is in progress.
 *
 * @param game The game_data object containing the game state.
 * @param time_delta The time difference between frames.
 */
void update_in_progress(game_data &game, double time_delta) {
    update_player(game.player, time_delta);

    // Update power-ups
    for (power_up_data &power_up : game.power_ups) {
        update_power_up(power_up);
    }

    // Update planets
    for (planet_data &planet : game.planets) {
        update_planet(planet);
    }

    // Spawn 10 power-ups every 20 seconds
    spawn_power_ups(game);

    // Spawn 1 enemy every 10 seconds
    spawn_enemy(game);

    // Update projectiles
    for (projectile_data &projectile : game.projectiles) {
        update_projectile(projectile, time_delta);
    }

    // Check for collisions between player's projectiles and enemies
    check_player_projectile_collisions(game);

    // Update enemies
    update_enemies(game);

    // Check for collisions between enemies' projectiles and player
    check_enemy_projectile_collisions(game);

    // Update explosions
    for (explosion_data &explosion : game.explosions) {
        update_explosion(explosion);
    }

    // Update and remove expired explosions
    update_explosions(game);

    // Add a new planet
    add_planet(game);

    // Check for collisions between power-ups and player
    check_player_power_up_collisions(game);

    // Update kill streaks
    update_kill_streaks(game);
}


/**
 * Update the game state when it is in the "game over" state.
 *
 * @param game The game_data object containing the game state.
 */
void update_game_over(game_data &game) {
    game_over(game);
}

/**
 * Update the game state based on the current game state.
 *
 * @param game The game_data object containing the game state.
 */
void update_game(game_data &game) {
    double time_delta = 1.0 / 60.0;  
    switch (game.state) {
        case game_state::START_MENU:
            update_start_menu(game);
            break;
        case game_state::IN_PROGRESS:
            update_in_progress(game, time_delta);
            break;
        case game_state::GAME_OVER:
            update_game_over(game);
            break;
    }
}

// Function to add a new enemy to the game
void add_enemy(game_data &game) {
    // Choose a random enemy type
    enemy_kind kind = static_cast<enemy_kind>(rnd(3));
    game.enemies.push_back(new_enemy(kind, game.player, game.enemies));
}

/**
 * Draw the start menu.
 *
 * @param game The game_data object containing the game state.
 */
void draw_start_menu(const game_data &game) {
    double bitmap_w = bitmap_width("main");
    double bitmap_h = bitmap_height("main");

    double x = (screen_width() - bitmap_w) / 2;
    double y = (screen_height() - bitmap_h) / 2;

    draw_text("Press ENTER to start", COLOR_WHITE, screen_width() / 2 - 100, screen_height() / 2 + 300);
    draw_bitmap(bitmap_named("main"), x, y);
}


/**
 * Draw the game in progress.
 *
 * @param game The game_data object containing the game state.
 */
void draw_in_progress(const game_data &game) {
    // draw background for game
    draw_bitmap("space", 0, 0, option_to_screen());
    
    for (const auto &planet : game.planets) {
        draw_planet(planet);
    }

    draw_player(game.player);

    draw_enemies(game);

    for (const auto &power_up : game.power_ups) {
        draw_power_up(power_up);
    }

    for (const projectile_data &projectile : game.projectiles) {
        draw_projectile(projectile);
    }

    for (const explosion_data &explosion : game.explosions) {
        draw_explosion(explosion);
    }

    draw_hud(game.player, game.enemies, game);
}


/**
 * Draw the game on the screen.
 *
 * @param game The game_data object containing the game state.
 */
void draw_game(const game_data &game) {
    clear_screen(COLOR_BLACK);

    switch (game.state) {
        case game_state::START_MENU:
            draw_start_menu(game);
            break;
        case game_state::IN_PROGRESS:
            draw_in_progress(game);
            break;
        case game_state::GAME_OVER:
            // Do nothing for now
            break;
    }

    refresh_screen(60);
}


/**
 * Spawn power-ups in the game.
 *
 * @param game The game_data object to spawn power-ups in.
 */
void spawn_power_ups(game_data &game) {
    if (should_spawn_power_up(game)) {
        for (int i = 0; i < 10; ++i) {
            add_power_up(game);
        }
        game.last_power_up_spawn_tick = timer_ticks(game.game_timer);
    }
}


/**
 * Spawn an enemy in the game.
 *
 * @param game The game_data object to spawn an enemy in.
 */
void spawn_enemy(game_data &game) {
    if (timer_ticks(game.game_timer) - game.last_enemy_spawn_tick >= game.enemy_spawn_interval) {
        add_enemy(game);
        game.last_enemy_spawn_tick = timer_ticks(game.game_timer);
    }
}



