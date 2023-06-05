#ifndef KILL_STREAK_H
#define KILL_STREAK_H

#include "player.h"

// Enumeration for different kill streak sounds
enum class KillStreakSound {
    FIRST_BLOOD,    // Sound for the first kill streak
    DOUBLE_KILL,    // Sound for a double kill
    TRIPLE_KILL,    // Sound for a triple kill
    UNSTOPPABLE,    // Sound for an unstoppable kill streak
    ULTRA_KILL,     // Sound for an ultra kill
    GODLIKE,        // Sound for a godlike kill streak
    LUDICROUS,      // Sound for a ludicrous kill streak
    ULTIMATE        // Sound for the ultimate kill streak
};

/**
 * Plays the kill streak sound based on the current kill streak count in the game.
 *
 * @param game The game data.
 */
void play_kill_streak_sound(game_data& game);

/**
 * Activates the ultimate kill streak in the game.
 *
 * @param game The game data.
 */
void activate_ultimate_kill_streak(game_data& game);

/**
 * Ends the ultimate kill streak in the game.
 *
 * @param game The game data.
 */
void end_ultimate_kill_streak(game_data& game);

/**
 * Updates the kill streaks in the game based on the player's performance.
 *
 * @param game The game data.
 */
void update_kill_streaks(game_data& game);

/**
 * Updates the player attributes in the game based on the damage, rocket count, and player boost.
 *
 * @param game The game data.
 * @param damage The amount of damage inflicted.
 * @param rocket_count The number of rockets.
 * @param player_boost The boost value for the player.
 */
void update_player_attributes(game_data& game, int damage, int rocket_count, double player_boost);


#endif 