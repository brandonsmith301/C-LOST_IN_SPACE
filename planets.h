#ifndef PLANET_H
#define PLANET_H

#include "splashkit.h"
#include "lost_in_space.h"
#include "player.h"
#include "power_up.h"
#include <vector>

/**
 * Enumeration of planet kinds.
 */
enum planet_kind {
    MERCURY, 
    VENUS, 
    EARTH, 
    MARS, 
    JUPITER, 
    SATURN, 
    URANUS, 
    NEPTUNE, 
    PLUTO
};

/**
 * Structure representing planet data.
 * It contains a sprite representing the planet and its kind.
 */
struct planet_data {
    sprite planet_sprite;   // Sprite object representing the planet
    planet_kind kind;       // Kind of the planet
};

/**
 * Create a new planet with the specified position.
 * It takes the x and y coordinates as input and returns a planet_data object.
 *
 * @param x The x coordinate of the planet's position.
 * @param y The y coordinate of the planet's position.
 * @return A planet_data object representing the new planet.
 */
planet_data new_planet(double x, double y);


/**
 * Draws the planet sprite onto the screen
 * @param planet_to_draw The planet data containing the sprite to be drawn
 */
void draw_planet(const planet_data &planet_to_draw);

/**
 * Updates the planet sprite with its current properties
 * @param planet_to_update The planet data containing the sprite to be updated
 */
void update_planet(const planet_data &planet_to_update);

/**
 * Add a planet to the game.
 * It takes the game data as input.
 * The function generates a new planet and adds it to the game's planets vector.
 *
 * @param game The game data.
 */
void add_planet(game_data &game);


#endif 