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
 * Load the game images, sounds, etc.
 */
void load_resources() {
    load_resource_bundle("game_bundle", "lost_in_space.txt");
}

/**
 * Entry point.
 * 
 * Manages the initialisation of data, the event loop, and quitting.
 */
int main() {
    open_window("Lost in Space", 800, 800);
    load_resources();

    game_data game = new_game();
    bool game_over = false;

    do {
        process_events();

        if (!game_over) {
            update_game(game);
            update_game_time(game);
            handle_input(game);
            draw_game(game);

            if (game.state == game_state::GAME_OVER) {
                play_music("game_over");
                
                // Draw "Game Over" text
                draw_text("Game Over", COLOR_WHITE, screen_width() / 2 - 40, screen_height() / 2, option_to_screen());

                refresh_screen();   // Update the screen
                delay(5000);        // wait for 5 seconds
                game_over = true;
            }
        }

    } while (not quit_requested());

    close_window("Lost in Space");

    return 0;
}

