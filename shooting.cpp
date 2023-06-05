#include "splashkit.h"
#include "shooting.h"
#include "game_data.h"

/**
 * Calculates the location of the point on the circumference of the sprite's circle.
 * The point is determined based on the sprite's rotation.
 * 
 * @param ship The sprite for which to calculate the point position.
 * @return The position of the point on the sprite's circle.
 */
point_2d circle_point_position(const sprite &ship) {
    // Convert sprite rotation to radians
    double rotation_radians = sprite_rotation(ship) * (M_PI / 180.0);

    // Calculate the position of the point on the sprite's circle
    double circle_radius = sprite_width(ship) / 2.0;
    double point_x = circle_radius * cos(rotation_radians);
    double point_y = circle_radius * sin(rotation_radians);

    // Translate the point's position to the sprite's center
    point_2d ship_center = center_point(ship);
    return {ship_center.x + point_x, ship_center.y + point_y};
}

/**
 * Creates a new projectile based on the player's sprite and adds it to the game's projectiles vector.
 * 
 * @param game The game_data object containing all game-related data.
 * @param projectiles The vector of all active projectiles.
 */
void create_projectile(game_data& game, std::vector<projectile_data>& projectiles) {
    projectile_data new_projectile;
    new_projectile.shape.radius = 4;

    // Calculate the angle based on the position of the red dot
    point_2d red_dot_position = circle_point_position(game.player.player_sprite);
    point_2d ship_center = center_point(game.player.player_sprite);
    double angle_radians = atan2(red_dot_position.y - ship_center.y, red_dot_position.x - ship_center.x);

    // Set the initial projectile data
    new_projectile.shape.center = red_dot_position;
    new_projectile.angle = angle_radians * (-180.0 / M_PI);
    new_projectile.speed = 1250;
    new_projectile.owner = 0;

    projectiles.push_back(new_projectile);
}

/**
 * Updates a projectile's position based on its speed and angle.
 * 
 * @param projectile The projectile to update.
 * @param time_delta The time elapsed since the last frame, used to calculate the new position.
 */
void update_projectile(projectile_data &projectile, double time_delta) {
    double dx = cosine(projectile.angle) * projectile.speed * time_delta;
    double dy = sine(projectile.angle) * projectile.speed * time_delta;
    projectile.shape.center.x += dx;
    projectile.shape.center.y -= dy;
}

/**
 * Draws a projectile on the screen.
 * 
 * @param projectile The projectile to draw.
 */
void draw_projectile(const projectile_data &projectile) {
    fill_circle(COLOR_WHITE, projectile.shape);
}