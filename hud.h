
// Header file for a game's HUD and mini-map functions
#ifndef HUD_H
#define HUD_H

#include "game_data.h"
#include "enemy.h"
#include "power_up.h"

// Structure to represent the mini-map on the screen
struct MiniMap {
    int x;      // X-coordinate of the mini-map
    int y;      // Y-coordinate of the mini-map
    int width;  // Width of the mini-map
    int height; // Height of the mini-map

    MiniMap(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}
};

/**
 * Calculate the scale factor for mapping coordinates from the game map to the mini-map.
 *
 * @param map_dimension The dimension of the game map (width or height).
 * @param mini_map_dimension The dimension of the mini-map (width or height).
 * @return The calculated scale factor.
 */
double calculate_scale(int map_dimension, int mini_map_dimension);

/**
 * Scale a coordinate value based on the map and mini-map dimensions.
 *
 * @param entity_coordinate The coordinate value of an entity in the game map.
 * @param map_dimension The dimension of the game map (width or height).
 * @param mini_map_dimension The dimension of the mini-map (width or height).
 * @return The scaled coordinate value.
 */
double scale_coordinate(int entity_coordinate, int map_dimension, int mini_map_dimension);

/**
 * Convert the coordinates of an entity to mini-map coordinates.
 *
 * @param entity The sprite representing the entity.
 * @param game The game_data object containing the game-related information.
 * @param mini_map_width The width of the mini-map.
 * @param mini_map_height The height of the mini-map.
 * @return The mini-map coordinates of the entity.
 */
point_2d mini_map_coordinate(const sprite& entity, const game_data& game, int mini_map_width, int mini_map_height);

/**
 * Calculate the alpha value for fading enemies based on their distance from the player.
 *
 * @param distance The distance between the player and an enemy.
 * @return The calculated alpha value for fading the enemy.
 */
double calculate_alpha(double distance);

/**
 * Draw an entity on the mini-map as a filled circle.
 *
 * @param entity_sprite The sprite representing the entity to be drawn.
 * @param game The game_data object containing the game-related information.
 * @param entity_color The color to be used for drawing the entity.
 * @param mini_map The MiniMap object containing information about the mini-map.
 */
void draw_entity_on_minimap(const sprite& entity_sprite, const game_data& game, color entity_color, const MiniMap& mini_map);

/**
 * Draw the mini-map on the screen.
 *
 * @param player The player_data object containing player-related information.
 * @param enemies The vector of enemy_data objects containing enemy-related information.
 * @param power_ups The vector of power_up_data objects containing power-up-related information.
 * @param game The game_data object containing game-related information.
 */
void draw_mini_map(const player_data& player, const std::vector<enemy_data>& enemies, const std::vector<power_up_data>& power_ups, const game_data& game);

/**
 * Draw an item in the heads-up display (HUD).
 *
 * @param bitmap_name The name of the bitmap to draw.
 * @param value The value associated with the item.
 * @param x The x-coordinate of the item.
 * @param y The y-coordinate of the item.
 * @param x1 The x-coordinate of the value text.
 * @param y1 The y-coordinate of the value text.
 */
void draw_hud_item(const std::string& bitmap_name, const std::string& value, int x, int y, int x1, int y1);

/**
 * Draw a progress bar on the heads-up display (HUD).
 *
 * @param empty_bitmap The name of the bitmap for the empty part of the bar.
 * @param filled_bitmap The name of the bitmap for the filled part of the bar.
 * @param x The x-coordinate of the bar.
 * @param y The y-coordinate of the bar.
 * @param width The width of the bar.
 * @param pct The percentage filled of the bar.
 */
void draw_hud_bar(const std::string& empty_bitmap, const std::string& filled_bitmap, int x, int y, int width, double pct);

/**
 * Draw the heads-up display (HUD) on the screen.
 *
 * @param player The player_data object containing player-related information.
 * @param enemies The vector of enemy_data objects containing enemy-related information.
 * @param game The game_data object containing game-related information.
 */
void draw_hud(const player_data& player, const std::vector<enemy_data>& enemies, const game_data& game);

#endif // HUD_H