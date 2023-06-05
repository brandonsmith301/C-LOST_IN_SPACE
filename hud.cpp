#include "splashkit.h"
#include "hud.h"
#include "lost_in_space.h"
#include "game_data.h"
#include "enemy.h"
#include "power_up.h"
#include <algorithm>
#include <unordered_map>

/**
 * Calculate the scale factor for mapping coordinates from the game map to the mini-map.
 *
 * @param map_dimension The dimension of the game map (width or height).
 * @param mini_map_dimension The dimension of the mini-map (width or height).
 * @return The calculated scale factor.
 */
double calculate_scale(int map_dimension, int mini_map_dimension) {
    return static_cast<double>(mini_map_dimension) / (map_dimension * 2);
}

/**
 * Scale a coordinate value based on the map and mini-map dimensions.
 *
 * @param entity_coordinate The coordinate value of an entity in the game map.
 * @param map_dimension The dimension of the game map (width or height).
 * @param mini_map_dimension The dimension of the mini-map (width or height).
 * @return The scaled coordinate value.
 */
double scale_coordinate(int entity_coordinate, int map_dimension, int mini_map_dimension) {
    double scale = calculate_scale(map_dimension, mini_map_dimension);
    return (entity_coordinate + map_dimension) * scale;
}

/**
 * Convert the coordinates of an entity to mini-map coordinates.
 *
 * @param entity The sprite representing the entity.
 * @param game The game_data object containing the game-related information.
 * @param mini_map_width The width of the mini-map.
 * @param mini_map_height The height of the mini-map.
 * @return The mini-map coordinates of the entity.
 */
point_2d mini_map_coordinate(const sprite& entity, const game_data& game, int mini_map_width, int mini_map_height) {
    double x = scale_coordinate(sprite_x(entity), game.map_width, mini_map_width);
    double y = scale_coordinate(sprite_y(entity), game.map_height, mini_map_height);
    return point_at(x, y);
}

/**
 * Draw an entity on the mini-map as a filled circle.
 *
 * @param entity_sprite The sprite representing the entity to be drawn.
 * @param game The game_data object containing the game-related information.
 * @param entity_color The color to be used for drawing the entity.
 * @param mini_map The MiniMap object containing information about the mini-map.
 */
void draw_entity_on_minimap(const sprite& entity_sprite, const game_data& game, color entity_color, const MiniMap& mini_map) {
    point_2d entity_point = mini_map_coordinate(entity_sprite, game, mini_map.width, mini_map.height);
    fill_circle(entity_color, entity_point.x + mini_map.x, entity_point.y + mini_map.y, 3, option_to_screen());
}

/**
 * Draw the radar pulse around the player on the mini-map.
 *
 * @param player_point The 2D point representing the player's position on the mini-map.
 * @param mini_map The MiniMap object containing information about the mini-map.
 * @return The radius of the radar pulse.
 */
double draw_radar_pulse(const point_2d& player_point, const MiniMap& mini_map) {
    static clock_t radar_pulse_start_time = clock();
    double elapsed_time = static_cast<double>(clock() - radar_pulse_start_time) / CLOCKS_PER_SEC * 1000;
    double max_pulse_radius = std::min(mini_map.width, mini_map.height) / 2.0;
    double pulse_radius = (elapsed_time / RADAR_PULSE_DURATION) * max_pulse_radius;

    // Reset the radar pulse start time if the duration has passed
    if (elapsed_time >= RADAR_PULSE_DURATION) {
        radar_pulse_start_time = clock();
    }

    color radar_pulse_color = rgba_color(0, 255, 0, 64);
    draw_circle(radar_pulse_color, player_point.x + mini_map.x, player_point.y + mini_map.y, pulse_radius, option_to_screen());

    return pulse_radius;
}

/**
 * Calculate the alpha value for fading enemies based on their distance from the player.
 *
 * @param distance The distance between the player and an enemy.
 * @return The calculated alpha value for fading the enemy.
 */
double calculate_alpha(double distance) {
    double alpha = 255;
    if (distance > RADAR_RANGE - RADAR_FADE_DISTANCE) {
        alpha = 255 * std::exp(-(distance - RADAR_RANGE + RADAR_FADE_DISTANCE));
    }
    return std::max(0.0, alpha);
}


/**
 * Draw the mini-map on the screen.
 *
 * @param player The player_data object containing player-related information.
 * @param enemies The vector of enemy_data objects containing enemy-related information.
 * @param power_ups The vector of power_up_data objects containing power-up-related information.
 * @param game The game_data object containing game-related information.
 */
void draw_mini_map(const player_data& player, const std::vector<enemy_data>& enemies, const std::vector<power_up_data>& power_ups, const game_data& game) {
    MiniMap mini_map(MINI_MAP_X, screen_height() - MINI_MAP_HEIGHT - 10, MINI_MAP_WIDTH, MINI_MAP_HEIGHT);

    // Draw the semi-transparent mini map background
    color semi_transparent_bg = rgba_color(204, 204, 255, 128);
    fill_rectangle(semi_transparent_bg, mini_map.x, mini_map.y, mini_map.width, mini_map.height, option_to_screen());

    // Draw the player as a green dot
    point_2d player_point = mini_map_coordinate(player.player_sprite, game, mini_map.width, mini_map.height);
    draw_entity_on_minimap(player.player_sprite, game, COLOR_GREEN, mini_map);

    // Draw the radar pulse around the player
    double pulse_radius = draw_radar_pulse(player_point, mini_map);  // Update to return pulse_radius

    // Draw enemies as red dots with fading effect based on radar range
    for (const auto &enemy : enemies) {
        point_2d enemy_point = mini_map_coordinate(enemy.enemy_sprite, game, mini_map.width, mini_map.height);
        double distance = distance_between_points(player_point, enemy_point);

        if (distance <= pulse_radius) {
            double alpha = calculate_alpha(distance);
            color enemy_color = rgba_color(255.0, 0.0, 0.0, alpha);
            draw_entity_on_minimap(enemy.enemy_sprite, game, enemy_color, mini_map);
        }
    }
}


/**
 * Calculate the distance between two 2D points.
 *
 * @param point1 The first point.
 * @param point2 The second point.
 * @return The distance between the two points.
 */
double distance_between_points(const point_2d &point1, const point_2d &point2) {
    double dx = point1.x - point2.x;
    double dy = point1.y - point2.y;
    return std::sqrt(dx * dx + dy * dy);
}

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
void draw_hud_item(const std::string& bitmap_name, const std::string& value, int x, int y, int x1, int y1) {
    // Draw the bitmap
    draw_bitmap(bitmap_name, x, y, option_to_screen());
    
    // Draw the value text
    draw_text(value, COLOR_SNOW, x1, y1, option_to_screen());
}

/**
 * Draw a bar in the heads-up display (HUD).
 *
 * @param empty_bitmap The name of the bitmap for the empty part of the bar.
 * @param filled_bitmap The name of the bitmap for the filled part of the bar.
 * @param x The x-coordinate of the bar.
 * @param y The y-coordinate of the bar.
 * @param width The width of the bar.
 * @param pct The percentage filled of the bar.
 */
void draw_hud_bar(const std::string& empty_bitmap, const std::string& filled_bitmap, int x, int y, int width, double pct) {
    int part_width = width * pct;
    
    // Draw the empty bar
    draw_bitmap(empty_bitmap, x, y, option_part_bmp(0, 0, width, bitmap_height(filled_bitmap), option_to_screen()));
    
    // Draw the filled bar
    draw_bitmap(filled_bitmap, x, y, option_part_bmp(0, 0, part_width, bitmap_height(filled_bitmap), option_to_screen()));
}


/**
 * Draw the heads-up display (HUD) elements on the screen.
 *
 * @param player The player_data object containing player-related information.
 * @param enemies The vector of enemy_data objects containing enemy-related information.
 * @param game The game_data object containing game-related information.
 */
void draw_hud(const player_data &player, const std::vector<enemy_data> &enemies, const game_data &game) {
    // Draw Player Location
    std::string location_str = "LOCATION: " + point_to_string(center_point(player.player_sprite));
    draw_text(location_str, COLOR_SNOW, 20, 20, option_to_screen());

    // Draw other HUD items
    draw_hud_item("kill_count", std::to_string(player.kill_count), 20, 40, 50, 50);
    draw_hud_item("HUD_bullet", std::to_string(player.rocket_count), 20, 70, 50, 80);
    draw_hud_item("HUD_time", std::to_string(player.time_remaining), 20, 100, 50, 110);

    // Right align fuel and shield bars
    int bar_width = 300;
    int bar_x = 500;

    // Draw the fuel gauge bar
    draw_hud_bar("empty", "full", bar_x, 10, bar_width, player.fuel_pct);

    // Draw the shield gauge bar
    draw_hud_bar("empty", "purple_bar", bar_x, 60, bar_width, player.shield_pct);

    // Draw the mini-map
    draw_mini_map(player, enemies, game.power_ups, game);
}

