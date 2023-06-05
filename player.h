#ifndef LOST_IN_SPACE_PLAYER
#define LOST_IN_SPACE_PLAYER

#include "splashkit.h"
#include "power_up.h"

// Constants can be declared as constexpr which can be evaluated at compile time.
constexpr int MINI_MAP_WIDTH = 125;
constexpr int MINI_MAP_HEIGHT = 125;
constexpr int MINI_MAP_X = 10;
constexpr int RADAR_RANGE = 500;
constexpr int RADAR_FADE_DISTANCE = 200;
constexpr int RADAR_PULSE_DURATION = 1000;
constexpr double MIN = -2000;
constexpr double MAX = 2000;

// Constants
const double PLAYER_ROTATE_SPEED = 3.0;
const double PLAYER_SPEED = 2.0;
const double BOOSTED_SPEED = 6.0;
const double SCREEN_BORDER = 150;

// Add a forward declaration for game_data
struct game_data;
struct enemy_data;

/**
 * Different options for the kind of ship.
 * Adjusts the image used.
 */
enum ship_kind {
    AQUARII,
    GLIESE,
    PEGASI
};

/**
 * The player data keeps track of all of the information related to the player.
 * 
 * @field   player_sprite   The player's sprite - used to track position and movement
 * @field   hud_sprite      The player's HUD sprite
 * @field   kind            Current kind of player ship
 * @field   current_rotation The player's current rotation
 * @field   score           The current score for the player
 * @field   fuel_pct        The player's fuel percentage
 * @field   shield_pct      The player's shield percentage
 * @field   rocket_count    The number of rockets the player has
 * @field   coin_count      The number of coins the player has
 * @field   has_moved       Whether the player has moved
 * @field   time_remaining  The player's remaining time
 * @field   kill_count      The player's kill count
 * @field   damage          The player's damage
 * @field   original_ammo_state The player's original ammo state
 * @field   player_speed    The player's speed
 * @field   player_boost    The player's boost speed
 * @field   player_ultra_boost  The player's ultra-boost speed
 */
struct player_data {
    sprite      player_sprite;
    sprite      hud_sprite;
    ship_kind   kind;
    double      current_rotation;
    double      fuel_pct; 
    double      shield_pct;
    int         rocket_count;
    bool        has_moved;
    int         time_remaining; 
    int         kill_count; 
    int         damage;
    int         original_ammo_state;
    double      player_speed;
    double      player_boost;
    double      player_ultra_boost;
};

/**
 * Creates a new player in the centre of the screen with the default ship.
 * @returns The new player data
 */
player_data new_player();

/**
 * Draws the player to the screen. 
 * @param player_to_draw The player to draw to the screen
 */
void draw_player(const player_data &player_to_draw);

/**
 * Actions a step update of the player - moving them and adjusting the camera.
 * @param player_to_update The player being updated
 * @param time_delta The amount of time that has passed since the last update
 */
void update_player(player_data &player_to_update, const double time_delta);

/**
 * Read user input and update the player based on this interaction.
 * @param game      The game_data struct holding game state information
 */
void handle_input(game_data &game);

/**
 * Draws the Heads Up Display (HUD) for the game. It displays information such as the player's location, coin count, rocket count, time remaining, fuel gauge, and shield gauge.
 * @param player    The player_data struct holding the player's data
 * @param enemies   A vector containing all the enemy_data structs in the game
 * @param game      The game_data struct holding game state information
 */
void draw_hud(const player_data &player, const std::vector<enemy_data> &enemies, const game_data &game);

/**
 * Draws the mini map, including the player, enemies, and power-ups.
 * @param player        The player_data struct holding the player's data
 * @param enemies       A vector containing all the enemy_data structs in the game
 * @param power_ups     A vector containing all the power_up_data structs in the game
 * @param game          The game_data struct holding game state information
 */
void draw_mini_map(const player_data &player, const std::vector<enemy_data> &enemies, const std::vector<power_up_data> &power_ups, const game_data &game);

/**
 * Calculates the distance between two points.
 * @param point1    The first point
 * @param point2    The second point
 * @return          The distance between the two points
 */
double distance_between_points(const point_2d &point1, const point_2d &point2);

/**
 * Update the position of the player's sprite based on its rotation and velocity.
 * 
 * @param player The player data containing the sprite to update.
 */
void update_player_position(player_data &player);

/**
 * Adjust the camera position based on the player's position to keep the player within the screen boundaries.
 * 
 * @param player The player data containing the player's sprite.
 */
void adjust_camera_position(const player_data &player);

/**
 * Set the position of a sprite to the specified coordinates.
 * 
 * @param player_sprite The sprite to set the position of.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 */
void set_sprite_position(sprite &player_sprite, double x, double y);

/**
 * Handle the player switching ships based on the user input.
 * 
 * @param game The game data containing the player to switch ships.
 */
void handle_ship_switch(game_data &game);

/**
 * Handle the rotation of the player's ship based on the user input.
 * 
 * @param game The game data containing the player's ship sprite.
 */
void handle_rotation(game_data &game);

/**
 * Handle the movement of the player's ship based on the user input.
 * 
 * @param game The game data containing the player's ship sprite and movement parameters.
 */
void handle_movement(game_data &game);

/**
 * Handle the firing of projectiles from the player's ship based on the user input.
 * 
 * @param game The game data containing the player's ship sprite and projectile count.
 */
void handle_fire(game_data &game);


#endif 