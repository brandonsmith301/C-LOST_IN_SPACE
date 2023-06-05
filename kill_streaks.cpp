#include "splashkit.h"
#include "game_data.h"
#include "kill_streaks.h"

// Map to associate each KillStreakSound with its corresponding sound effect
std::map<KillStreakSound, std::string> killStreakSounds = {
    { KillStreakSound::FIRST_BLOOD, "first_blood" },
    { KillStreakSound::DOUBLE_KILL, "double_kill" },
    { KillStreakSound::TRIPLE_KILL, "triple_kill" },
    { KillStreakSound::UNSTOPPABLE, "unstoppable" },
    { KillStreakSound::ULTRA_KILL, "ultrakill" },
    { KillStreakSound::GODLIKE, "godlike" },
    { KillStreakSound::LUDICROUS, "ludicrouskill" },
    { KillStreakSound::ULTIMATE, "comegetsome" }
};

/**
 * Plays the appropriate kill streak sound effect based on the player's kill count.
 * If the kill count reaches a certain threshold, it may activate the ultimate kill streak.
 *
 * @param game The game data.
 */
void play_kill_streak_sound(game_data& game) {
    std::map<int, KillStreakSound> killStreaks = {
        {1, KillStreakSound::FIRST_BLOOD},
        {2, KillStreakSound::DOUBLE_KILL},
        {3, KillStreakSound::TRIPLE_KILL},
        {4, KillStreakSound::UNSTOPPABLE},
        {5, KillStreakSound::ULTRA_KILL},
        {6, KillStreakSound::GODLIKE},
        {7, KillStreakSound::LUDICROUS},
        {10, KillStreakSound::ULTIMATE}
    };

    if (killStreaks.count(game.player.kill_count) > 0) {
        KillStreakSound sound = killStreaks[game.player.kill_count];
        if (sound == KillStreakSound::ULTIMATE) {
            activate_ultimate_kill_streak(game); // Activate the ultimate kill streak
        } else {
            play_sound_effect(killStreakSounds[sound]); // Play the corresponding kill streak sound effect
        }
    }
}

/**
 * Updates the player's attributes in the game.
 *
 * @param game         The game data.
 * @param damage       The new damage value for the player.
 * @param rocket_count The new rocket count for the player.
 * @param player_boost The new player boost value.
 */
void update_player_attributes(game_data& game, int damage, int rocket_count, double player_boost) {
    game.player.damage = damage; // Update the player's damage value
    game.player.rocket_count = rocket_count; // Update the player's rocket count
    game.player.player_boost = player_boost; // Update the player's boost value
}

/**
 * Activates the ultimate kill streak in the game.
 * It plays sound effects and music, updates player attributes, and modifies enemy damage values.
 *
 * @param game The game data.
 */
void activate_ultimate_kill_streak(game_data& game) {
    play_sound_effect("comegetsome"); // Play sound effect for ultimate kill streak activation
    play_music("killstreaks"); // Play music for kill streaks

    // Save previous ammo
    game.player.original_ammo_state = game.player.rocket_count;

    update_player_attributes(game, 500, 1000, 10.0); // Update player attributes for ultimate kill streak

    // Save the original damage value for each enemy and set their damage to 0.0
    for (auto& enemy : game.enemies) {
        game.original_damage_values[enemy.kind] = enemy.damage_value;
        enemy.damage_value = 0.0;
    }

    game.is_kill_streak_active = true; // Activate kill streak
    game.last_kill_streak_tick = timer_ticks(game.game_timer); // Start the kill streak
}

/**
 * Ends the ultimate kill streak in the game.
 * It updates player attributes and restores enemy damage values.
 *
 * @param game The game data.
 */
void end_ultimate_kill_streak(game_data& game) {
    update_player_attributes(game, 100, game.player.original_ammo_state, 6.0); // Update player attributes to end kill streak

    // Reset the damage value for each enemy to their original values
    for (auto& enemy : game.enemies) {
        enemy.damage_value = game.original_damage_values[enemy.kind];
    }

    game.is_kill_streak_active = false; // Deactivate kill streak
    game.last_kill_streak_tick = timer_ticks(game.game_timer); // End the kill streak
    stop_music(); // Stop the kill streak music
}

/**
 * Checks if the kill streak should end based on the game state and the time elapsed since the last kill streak tick.
 *
 * @param game The game data.
 * @return True if the kill streak should end, false otherwise.
 */
bool should_end_kill_streak(const game_data& game) {
    return game.is_kill_streak_active && timer_ticks(game.game_timer) - game.last_kill_streak_tick >= 15000;
}

/**
 * Updates the kill streaks in the game.
 * If the kill streak should end, it calls the function to end the ultimate kill streak and plays a sound effect.
 *
 * @param game The game data.
 */
void update_kill_streaks(game_data& game) {
    if (should_end_kill_streak(game)) {
        end_ultimate_kill_streak(game);
        play_sound_effect("oh-yeah-2");
    }
}
