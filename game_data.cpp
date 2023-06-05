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

/**
* Creates a new game by initialising a new game_data object, creating a new player with new_player(),
* and assigning the player to the player field of the game_data object.
*  @return The newly created game_data object.
*/
game_data new_game() {
    game_data game;
    game.player = new_player();
    spawn_all_enemies(game);
    game.map_width = MAX_X - MIN_X;
    game.map_height = MAX_Y - MIN_Y;
    game.is_kill_streak_active = false;
    game.game_timer = create_timer("game_timer"); 
    start_timer(game.game_timer);
    return game;
}

// This function handles the game overs
void game_over(game_data &game) {
    game.state = game_state::GAME_OVER;  // Set game state to GAME_OVER
}
