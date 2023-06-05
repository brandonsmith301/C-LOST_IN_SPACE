#include "splashkit.h"
#include "enemy.h"

// Constant for projectile firing interval
const int PROJECTILE_FIRE_INTERVAL = 2000; // 2 seconds in milliseconds
const double COLLISION_DISTANCE = 20.0; // Distance to detect potential collisions
const double MINIMUM_DISTANCE = 200.0;
const double STOP_DISTANCE = 200.0; // Add this constant at the beginning of your file

/**
 * This returns the bitmap associated with a specific enemy kind.
 * 
 * @param kind The kind of enemy to get the bitmap for.
 * @return The bitmap associated with the specified enemy kind.
 */
bitmap enemy_bitmap(enemy_kind kind) {
    switch (kind) {
    case enemy_kind::ENEMY_TYPE1:
        return bitmap_named("enemy_type1");
    case enemy_kind::ENEMY_TYPE2:
        return bitmap_named("enemy_type2");
    case enemy_kind::ENEMY_TYPE3:
        return bitmap_named("enemy_type3");
    case enemy_kind::ENEMY_BOSS:
        return bitmap_named("BOSS");
    default:
        return bitmap_named("enemy_type3");
    }
}
/**
* Converts an angle in radians to degrees.
* @param radians: the angle in radians to be converted
* @return the equivalent angle in degrees
*/
double radians_to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}

/**
 * Calculates and returns the center point of a given sprite.
 *
 * @param s The sprite for which to calculate the center point.
 * @return The center point of the sprite.
 */
point_2d sprite_center_point(sprite s) {
    return point_at(sprite_x(s) + sprite_width(s) / 2, sprite_y(s) + sprite_height(s) / 2);
}

/**
 * Calculates and returns the angle in degrees between two points.
 *
 * @param from_pt The starting point.
 * @param to_pt The target point.
 * @return The angle in degrees between the two points.
 */
double angle_to(point_2d from_pt, point_2d to_pt) {
    return radians_to_degrees(atan2(to_pt.y - from_pt.y, to_pt.x - from_pt.x));
}

/**
 * Returns the sign of a double value.
 *
 * @param value The double value to be evaluated.
 * @return 1 if the value is positive, -1 if the value is negative, and 0 if the value is 0.
 */
int sign(double value) {
    return (value > 0) - (value < 0);
}

/**
 * Sets the rotation of a sprite to face a target point.
 *
 * @param s The sprite to be rotated.
 * @param target The target point to face.
 */
void sprite_turn_to_face(sprite s, point_2d target) {
    double angle = angle_to(sprite_center_point(s), target);
    sprite_set_rotation(s, angle);
}

/**
 * Create a new enemy with the specified kind, based on player and existing enemies data.
 *
 * @param kind The kind of enemy to create.
 * @param player The player_data object containing player-related information.
 * @param enemies The vector of enemy_data objects containing existing enemy data.
 * @return The newly created enemy_data object.
 */
enemy_data new_enemy(enemy_kind kind, const player_data &player, const vector<enemy_data> &enemies) {
    enemy_data result;
    bitmap default_bitmap = enemy_bitmap(kind);

    result.enemy_sprite = create_sprite(default_bitmap);
    result.kind = kind;
    result.last_projectile_tick = 0;

    // Define a struct to hold the attributes for each enemy type
    struct enemy_attributes {
        int health;
        int speed;
        int rotation_speed;
        double damage_value;
        damage_type type;
    };

    // Use a map to hold the attributes for each enemy type
    std::map<enemy_kind, enemy_attributes> enemy_type_attributes = {
        {enemy_kind::ENEMY_TYPE1, {100, 2, 3, 0.05, damage_type::DAMAGE_TYPE1}},
        {enemy_kind::ENEMY_TYPE2, {200, 2, 1, 0.3, damage_type::DAMAGE_TYPE2}},
        {enemy_kind::ENEMY_TYPE3, {150, 2, 1, 0.10, damage_type::DAMAGE_TYPE3}},
        {enemy_kind::ENEMY_BOSS, {500, 5, 5, 0.75, damage_type::DAMAGE_BOSS}} 
    };

    // Set the properties of the enemy based on the enemy type
    result.health = enemy_type_attributes[kind].health;
    result.speed = enemy_type_attributes[kind].speed;
    result.rotation_speed = enemy_type_attributes[kind].rotation_speed;
    result.damage_value = enemy_type_attributes[kind].damage_value;
    result.type = enemy_type_attributes[kind].type;

    // Generate a random position and check if it's safe
    point_2d position;
    do {
        position.x = rnd(screen_width());
        position.y = rnd(screen_height());
    } while (!is_safe_distance_from_player(position, player.player_sprite) || !is_safe_distance_from_enemies(position, enemies));

    sprite_set_position(result.enemy_sprite, position);
    sprite_set_rotation(result.enemy_sprite, rnd(360));

    return result;
}


/**
 * Check if the specified position is at a safe distance from the player.
 *
 * @param position The position to check.
 * @param player_sprite The sprite representing the player.
 * @param min_distance The minimum safe distance.
 * @return True if the position is at a safe distance from the player, false otherwise.
 */
bool is_safe_distance_from_player(point_2d position, sprite player_sprite, double min_distance) {
    min_distance = 200;
    double distance = point_point_distance(position, sprite_center_point(player_sprite));
    return distance > min_distance;
}

/**
 * Check if the specified position is at a safe distance from other enemies.
 *
 * @param position The position to check.
 * @param enemies The vector of enemy_data objects representing other enemies.
 * @param min_distance The minimum safe distance.
 * @return True if the position is at a safe distance from other enemies, false otherwise.
 */
bool is_safe_distance_from_enemies(point_2d position, const vector<enemy_data> &enemies, double min_distance) {
    min_distance = 200;
    for (const enemy_data &enemy : enemies) {
        double distance = point_point_distance(position, sprite_center_point(enemy.enemy_sprite));
        if (distance <= min_distance) {
            return false;
        }
    }
    return true;
}

/**
 * Update the state of an enemy based on its distance from the player.
 *
 * @param enemy The enemy_data object to update.
 * @param player The player_data object containing player-related information.
 */
void update_enemy_state(enemy_data& enemy, const player_data& player) {
    double distance_to_player = point_point_distance(sprite_center_point(enemy.enemy_sprite), sprite_center_point(player.player_sprite));

    if (distance_to_player < 300) {
        if (rnd(100) < 30) { // 30% chance to switch between ATTACK and CHASE states
            enemy.state = (enemy.state == ATTACK) ? CHASE : ATTACK;
        }
    } else {
        enemy.state = PATROL;
    }
}


/**
 * Update the distance, angle, and rotation differential for the enemy based on the player's position.
 *
 * @param enemy The enemy_data object to update.
 * @param player The player_data object containing player-related information.
 */
void update_enemy_orientation(enemy_data& enemy, const player_data& player) {
    // Calculate the distance to the player
    double distance_to_player = point_point_distance(sprite_center_point(enemy.enemy_sprite), sprite_center_point(player.player_sprite));

    // Calculate the angle to the player
    double angle_to_player = angle_to(sprite_center_point(enemy.enemy_sprite), sprite_center_point(player.player_sprite));

    // Calculate the rotation differential
    double current_rotation = sprite_rotation(enemy.enemy_sprite);
    double rotation_diff = angle_to_player - current_rotation;

    // Normalise the rotation difference to the range [-180, 180]
    if (rotation_diff > 180) {
        rotation_diff -= 360;
    } else if (rotation_diff < -180) {
        rotation_diff += 360;
    }

    // Update the enemy's orientation properties
    enemy.distance_to_player = distance_to_player;
    enemy.angle_to_player = angle_to_player;
    enemy.rotation_diff = rotation_diff;
}


/**
 * Update the enemy's position based on its current state.
 *
 * @param enemy The enemy_data object to update.
 * @param player The player_data object containing player-related information.
 * @param game The game_data object containing game-related information.
 */
void update_enemy_position(enemy_data& enemy, const player_data& player, game_data& game) {
    double dx = 0.0, dy = 0.0;
    double new_x, new_y;
    double adjusted_speed = 0.0;

    switch (enemy.state) {
        case ATTACK:
        case CHASE:
            // Adjust the enemy's movement speed based on distance to the player
            if (enemy.distance_to_player > STOP_DISTANCE) {
                adjusted_speed = enemy.speed * (1.0 - (enemy.distance_to_player / 600.0));
            } else {
                adjusted_speed = 0.0;
            }

            // Calculate the movement in x and y directions
            dx = cos(sprite_rotation(enemy.enemy_sprite) * M_PI / 180.0) * adjusted_speed;
            dy = sin(sprite_rotation(enemy.enemy_sprite) * M_PI / 180.0) * adjusted_speed;
            new_x = sprite_x(enemy.enemy_sprite) + dx;
            new_y = sprite_y(enemy.enemy_sprite) + dy;

            // Create enemy projectiles if in ATTACK state and no existing projectiles
            if (enemy.state == ATTACK && enemy.projectiles.empty()) {
                create_enemy_projectile(game, game.player, enemy);
            }
            break;
        case PATROL:
        case IDLE:
            // Calculate the movement in x and y directions for PATROL and IDLE states
            dx = cos(sprite_rotation(enemy.enemy_sprite) * M_PI / 180.0) * (enemy.speed * 0.5);
            dy = sin(sprite_rotation(enemy.enemy_sprite) * M_PI / 180.0) * (enemy.speed * 0.5);
            new_x = sprite_x(enemy.enemy_sprite) + dx;
            new_y = sprite_y(enemy.enemy_sprite) + dy;
            break;
    }

    // Update the enemy's new position
    enemy.new_x = new_x;
    enemy.new_y = new_y;
}


/**
 * Check and adjust the enemy's speed if it is too close to the player.
 *
 * @param enemy The enemy_data object to update.
 */
void adjust_speed_near_player(enemy_data& enemy) {
    double adjusted_speed;

    if (enemy.distance_to_player <= MINIMUM_DISTANCE) {
        // Adjust the speed based on the enemy's distance to the player when too close
        adjusted_speed = std::max(0.0, enemy.speed * (enemy.distance_to_player - COLLISION_DISTANCE) / (MINIMUM_DISTANCE - COLLISION_DISTANCE));
    } else {
        // Adjust the speed based on the enemy's distance to the player
        adjusted_speed = enemy.speed * (1.0 - (enemy.distance_to_player / 600.0));
    }

    // Update the enemy's adjusted speed
    enemy.adjusted_speed = adjusted_speed;
}

/**
 * Check and update the enemy's position based on the map boundaries.
 *
 * @param enemy The enemy_data object to update.
 */
void update_enemy_boundaries(enemy_data& enemy) {
    // Check if the new position is within the horizontal map boundaries
    if (enemy.new_x >= MIN_X && enemy.new_x <= MAX_X) {
        sprite_set_x(enemy.enemy_sprite, enemy.new_x);
    } else {
        // Turn around if the enemy hits the horizontal boundary
        sprite_set_rotation(enemy.enemy_sprite, sprite_rotation(enemy.enemy_sprite) + 180);
    }

    // Check if the new position is within the vertical map boundaries
    if (enemy.new_y >= MIN_Y && enemy.new_y <= MAX_Y) {
        sprite_set_y(enemy.enemy_sprite, enemy.new_y);
    } else {
        // Turn around if the enemy hits the vertical boundary
        sprite_set_rotation(enemy.enemy_sprite, sprite_rotation(enemy.enemy_sprite) + 180);
    }
}


/**
 * Check for potential collision between the enemy and the player's ship.
 * If a collision is detected, the enemy is rotated and moved away from the player.
 *
 * @param enemy The enemy_data object to update.
 * @param player The player_data object containing player-related information.
 */
void update_enemy_collision(enemy_data& enemy, const player_data& player) {
    if (point_point_distance(sprite_center_point(enemy.enemy_sprite), sprite_center_point(player.player_sprite)) < COLLISION_DISTANCE) {
        // Rotate the enemy by 180 degrees
        sprite_set_rotation(enemy.enemy_sprite, sprite_rotation(enemy.enemy_sprite) + 180);

        // Move the enemy away from the player based on the adjusted speed
        double dx = cos(sprite_rotation(enemy.enemy_sprite) * M_PI / 180.0) * enemy.adjusted_speed;
        double dy = sin(sprite_rotation(enemy.enemy_sprite) * M_PI / 180.0) * enemy.adjusted_speed;
        sprite_set_x(enemy.enemy_sprite, sprite_x(enemy.enemy_sprite) + dx);
        sprite_set_y(enemy.enemy_sprite, sprite_y(enemy.enemy_sprite) + dy);
    }
}

/**
 * Update the enemy's rotation based on its current state and distance to the player.
 *
 * @param enemy The enemy_data object to update.
 * @param player The player_data object containing player-related information.
 * @param game The game_data object containing game-related information.
 */
void update_enemy_rotation(enemy_data& enemy, const player_data& player, game_data& game) {
    double adjusted_rotation_speed = enemy.rotation_speed * (1.0 - (enemy.distance_to_player / 600.0)) + (rnd(3) - 1);
    double rotation_step;

    switch (enemy.state) {
        case ATTACK:
        case CHASE:
            // Calculate the rotation step based on the adjusted rotation speed and the rotation difference
            rotation_step = adjusted_rotation_speed * sign(enemy.rotation_diff);

            // Update the enemy's rotation and make it face the player
            sprite_set_rotation(enemy.enemy_sprite, sprite_rotation(enemy.enemy_sprite) + rotation_step);
            sprite_turn_to_face(enemy.enemy_sprite, sprite_center_point(player.player_sprite));
            break;
        case PATROL:
        case IDLE:
            // Add a wandering behaviour, causing the enemy to change direction periodically
            static unsigned int wander_timer = 0;
            static unsigned int wander_interval = 2000; // 2 seconds in milliseconds

            if (timer_ticks(game.game_timer) - wander_timer >= wander_interval) {
                wander_timer = timer_ticks(game.game_timer);
                double random_angle = rnd(360);
                sprite_set_rotation(enemy.enemy_sprite, random_angle);
            }
            break;
    }
}


/**
 * This function updates the behaviour of an enemy based on the player's actions and game data.
 * It incorporates various sub-functions to update the enemy's state, orientation, rotation, position,
 * speed adjustment, boundaries, collision, and visual representation.
 * 
 * @param enemy The enemy to update.
 * @param player The player data.
 * @param game The game data.
 */
void update_enemy(enemy_data& enemy, const player_data& player, game_data& game) {
    // Update the enemy's state to reflect the current state of the player.
    update_enemy_state(enemy, player);

    // Adjust the enemy's orientation to face the player.
    update_enemy_orientation(enemy, player);

    // Update the enemy's rotation to match the player's rotation.
    update_enemy_rotation(enemy, player, game);

    // Update the enemy's position relative to the player's position.
    update_enemy_position(enemy, player, game);

    // Fine-tune the enemy's speed to be near the player's speed.
    adjust_speed_near_player(enemy);

    // Ensure the enemy stays within the boundaries of the game world.
    update_enemy_boundaries(enemy);

    // Check for any collisions between the enemy and the player.
    update_enemy_collision(enemy, player);

    // Update the visual sprite representation of the enemy.
    update_sprite(enemy.enemy_sprite);
}

/**
 * This function updates the projectiles in the game.
 * It iterates over each projectile in the vector and updates them with a time interval of 1/60 seconds.
 * 
 * @param projectiles The vector of projectiles to update.
 */
void update_enemies_projectiles(std::vector<projectile_data>& projectiles) {
    // Iterate over each projectile in the vector.
    for (projectile_data& projectile : projectiles) {
        // Update the individual projectile with a time interval of 1/60 seconds.
        update_projectile(projectile, 1.0 / 60.0);
    }
}

/**
 * This function updates the enemies in the game.
 * It iterates over each enemy in the game's enemies vector, updates their behaviour based on the player and game data,
 * updates their projectiles, and creates a new projectile if enough time has passed since the last one was fired.
 * 
 * @param game The game data.
 */
void update_enemies(game_data& game) {
    // Iterate over each enemy in the game's enemies vector.
    for (size_t i = 0; i < game.enemies.size(); ++i) {
        // Get a reference to the current enemy.
        enemy_data& enemy = game.enemies[i];

        // Update the behaviour of the enemy based on the player and game data.
        update_enemy(enemy, game.player, game);

        // Update the projectiles belonging to the enemy.
        update_enemies_projectiles(enemy.projectiles);

        // Create a new projectile if enough time has passed since the last one was fired.
        create_enemy_projectile(game, game.player, enemy);
    }
}


/**
 * This function draws all the enemies in the game.
 * It iterates over each enemy in the game's enemies vector and draws their sprites.
 * It also draws the projectiles belonging to each enemy.
 * 
 * @param game The game data.
 */
void draw_enemies(const game_data& game) {
    // Iterate over each enemy in the game's enemies vector.
    for (const enemy_data& enemy : game.enemies) {
        // Draw the enemy's sprite.
        draw_sprite(enemy.enemy_sprite);

        // Draw the projectiles belonging to the enemy.
        draw_enemies_projectiles(enemy.projectiles);
    }
}

/**
 * This function draws a single enemy.
 * It draws the sprite of the specified enemy.
 * 
 * @param enemy The enemy data.
 */
void draw_enemy(const enemy_data& enemy) {
    // Draw the enemy's sprite.
    draw_sprite(enemy.enemy_sprite);
}

/**
 * This function spawns new enemies in the game.
 * It creates new enemy objects of different types and adds them to the game's enemies vector.
 * 
 * @param game The game data.
 */
void spawn_all_enemies(game_data& game) {
    // Spawn new enemies and add them to the game's enemies vector.
    game.enemies.push_back(new_enemy(enemy_kind::ENEMY_TYPE1, game.player, game.enemies));
    game.enemies.push_back(new_enemy(enemy_kind::ENEMY_TYPE2, game.player, game.enemies));
    game.enemies.push_back(new_enemy(enemy_kind::ENEMY_TYPE3, game.player, game.enemies));
    game.enemies.push_back(new_enemy(enemy_kind::ENEMY_BOSS, game.player, game.enemies));
}


/**
 * This function draws all the projectiles in the game.
 * It iterates over each projectile in the given vector and draws them using the `draw_projectile` function.
 * 
 * @param projectiles The vector of projectile data.
 */
void draw_enemies_projectiles(const std::vector<projectile_data>& projectiles) {
    // Iterate over each projectile in the vector.
    for (const projectile_data& projectile : projectiles) {
        // Draw the individual projectile.
        draw_projectile(projectile);
    }
}


/**
 * This function calculates a delay multiplier used for timing purposes.
 * It generates a random number between 0 and 1, and if the number is greater than 0.5,
 * it sets the delay multiplier to 2.0; otherwise, it remains as 1.0.
 * 
 * @return The calculated delay multiplier.
 */
double calculate_delay() {
    double delay_multiplier = 1.0;
    double random_num = static_cast<double>(rand()) / RAND_MAX;

    // Check if the random number is greater than 0.5.
    if (random_num > 0.5) {
        delay_multiplier = 2.0;
    }

    // Return the calculated delay multiplier.
    return delay_multiplier;
}


/**
 * This function calculates the angle between an enemy point and a player point.
 * It takes the enemy point and player point as input and uses the atan2 function
 * to calculate the angle in radians. The calculated angle is then converted to degrees
 * and returned.
 * 
 * @param enemy_point The enemy point.
 * @param player_point The player point.
 * @return The angle between the enemy point and player point in degrees.
 */
double calculate_angle(const point_2d& enemy_point, const point_2d& player_point) {
    // Calculate the angle in radians using the atan2 function.
    double angle_radians = atan2(player_point.y - enemy_point.y, player_point.x - enemy_point.x);

    // Convert the angle to degrees and return it.
    return angle_radians * (-180.0 / M_PI);
}


/**
 * This function creates a projectile with a given position and angle.
 * It takes the position and angle as input and creates a new projectile object.
 * The properties of the projectile, such as shape, angle, speed, and owner, are set
 * based on the given position and angle. The newly created projectile is then returned.
 * 
 * @param position The position of the projectile.
 * @param angle The angle of the projectile.
 * @return The newly created projectile.
 */
projectile_data create_projectile(const point_2d& position, double angle) {
    // Create a new projectile object.
    projectile_data new_projectile;

    // Set the properties of the projectile.
    new_projectile.shape.radius = 6;
    new_projectile.shape.center = position;
    new_projectile.angle = angle;
    new_projectile.speed = 1000;
    new_projectile.owner = 1; // Set the owner field to 1 (enemy)

    // Return the newly created projectile.
    return new_projectile;
}

/**
 * This function creates a projectile for an enemy.
 * It takes the game data, player data, and enemy data as input.
 * The function calculates the time since the last projectile was fired and checks if the enemy is in the ATTACK state
 * and if enough time has passed since the last projectile.
 * If the conditions are met, the function proceeds to create a new projectile for the enemy.
 * The function calculates the time since the attack started and applies a delay multiplier.
 * It checks if enough time has passed since the attack started, accounting for the delay.
 * If enough time has passed, the function retrieves the position of the enemy's sprite and the center point of the player's sprite.
 * It calculates the angle (in degrees) between the enemy and the player.
 * Using the position and angle, the function creates a new projectile and adds it to the game's projectiles vector.
 * The tick when the projectile was fired is recorded, and a sound effect for the projectile is played.
 * Finally, the attack start time is reset.
 * 
 * @param game The game data.
 * @param player The player data.
 * @param enemy The enemy data.
 */
void create_enemy_projectile(game_data &game, const player_data& player, enemy_data& enemy) {
    // Calculate the time since the last projectile was fired.
    double time_since_last_projectile = timer_ticks(game.game_timer) - enemy.last_projectile_tick;

    // Check if the enemy is in the ATTACK state and enough time has passed since the last projectile.
    if (enemy.state == ATTACK && time_since_last_projectile >= PROJECTILE_FIRE_INTERVAL) {
        // If the attack just started, record the start time and return (don't shoot yet).
        if (enemy.attack_start_time == 0.0) {
            enemy.attack_start_time = timer_ticks(game.game_timer);
            return;
        }

        // Calculate the time since the attack started and apply the delay multiplier.
        double time_since_attack_start = timer_ticks(game.game_timer) - enemy.attack_start_time;
        double delay_multiplier = calculate_delay();

        // Check if enough time has passed since the attack started (accounting for delay).
        if (time_since_attack_start < 1000 * delay_multiplier) {
            return; // Don't shoot yet.
        }

        // Get the position of the enemy's sprite and the center point of the player's sprite.
        point_2d enemy_circle_point = circle_point_position(enemy.enemy_sprite);
        point_2d player_center = sprite_center_point(player.player_sprite);

        // Calculate the angle (in degrees) between the enemy and the player.
        double angle_degrees = calculate_angle(enemy_circle_point, player_center);

        // Create a new projectile with the enemy's position and calculated angle.
        projectile_data new_projectile = create_projectile(enemy_circle_point, angle_degrees);

        // Add the new projectile to the game's projectiles vector.
        game.projectiles.push_back(new_projectile);

        // Record the tick when the projectile was fired.
        enemy.last_projectile_tick = timer_ticks(game.game_timer);

        // Play a sound effect for the projectile.
        play_sound_effect("laser");

        // Reset the attack start time.
        enemy.attack_start_time = 0.0;
    }
}
