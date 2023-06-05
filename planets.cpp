#include "splashkit.h"
#include "lost_in_space.h"
#include "player.h"
#include "power_up.h"
#include "planets.h"

bitmap planet_bitmap(planet_kind kind) {
    switch (kind) {
        case MERCURY: 
            return bitmap_named("mercury");
        case VENUS:   
            return bitmap_named("venus");
        case URANUS:  
            return bitmap_named("uranus");
        case PLUTO:   
            return bitmap_named("pluto");
        case MARS:    
            return bitmap_named("mars");
        case NEPTUNE: 
            return bitmap_named("neptune");
        case JUPITER: 
            return bitmap_named("jupiter");
        case SATURN:  
            return bitmap_named("saturn");
        default:      
            return bitmap_named("earth");
    }
}

/**
* Creates a new planet with a random planet bitmap and sets its position and velocity
* @param x: the x coordinate of the planet
* @param y: the y coordinate of the planet
* @return: the newly created planet with a random bitmap, position and velocity
*/

planet_data new_planet(double x, double y) {
    planet_data result;
    // Randomly select a planet kind using rnd() function
    result.kind = static_cast<planet_kind>(rnd(9));

    // Get the bitmap of the selected planet
    bitmap planet_bmp = planet_bitmap(result.kind);

    // Create a sprite for the planet using the bitmap
    result.planet_sprite = create_sprite(planet_bmp);

    // Set the position of the sprite using the x and y parameters
    sprite_set_x(result.planet_sprite, x);
    sprite_set_y(result.planet_sprite, y);
    return result;
}

/**
 * Draws the planet sprite onto the screen
 * @param planet_to_draw The planet data containing the sprite to be drawn
 */
void draw_planet(const planet_data &planet_to_draw) {
    draw_sprite(planet_to_draw.planet_sprite); // Draw the planet sprite onto the screen
}

/**
 * Updates the planet sprite with its current properties
 * @param planet_to_update The planet data containing the sprite to be updated
 */
void update_planet(const planet_data &planet_to_update) {
    update_sprite(planet_to_update.planet_sprite); // Update the planet sprite with its current properties
}

bool is_unique_kind(const planet_data &planet, const game_data &game) {
    for (const auto &existing_planet : game.planets) {
        if (planet.kind == existing_planet.kind) {
            return false;  // The planet's kind already exists in the game
        }
    }
    return true;
}

bool is_sufficient_distance(const planet_data &planet, const game_data &game) {
    for (const auto &existing_planet : game.planets) {
        double distance = sqrt(pow(sprite_x(planet.planet_sprite) - sprite_x(existing_planet.planet_sprite), 2)
                            + pow(sprite_y(planet.planet_sprite) - sprite_y(existing_planet.planet_sprite), 2));
        if (distance < 500) {
            return false;  // The planet is too close to an existing planet
        }
    }
    return true;
}

void add_planet(game_data &game) {
    // Generate random x and y positions within the range
    int x = rnd(MAX_X - MIN_X + 1) + MIN_X;
    int y = rnd(MAX_Y - MIN_Y + 1) + MIN_Y;

    // Create new planet at the random position
    planet_data planet = new_planet(x, y);

    // Check if the planet's kind is unique and it's sufficiently distant from other planets
    if (is_unique_kind(planet, game) && is_sufficient_distance(planet, game)) {
        // Add planet to game's planets vector
        game.planets.push_back(planet);
    }
}