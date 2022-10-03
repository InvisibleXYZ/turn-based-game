#pragma once

#include "creation_of_game.hpp"

/**
 * Convert the value of enumerator 'type' to string
 * @param creature Creature from which type will be taken
 * @return String corresponding to enum
 */
auto type_string(creature const &creature) -> std::string {
    auto type = std::string();
    switch (creature.getType()) {
        case creature::type_enum::water:
            type = "water";
            break;
        case creature::type_enum::earth:
            type = "earth";
            break;
        case creature::type_enum::air:
            type = "air";
            break;
        case creature::type_enum::fire:
            type = "fire";
            break;
        case creature::type_enum::ice:
            type = "ice";
            break;
        case creature::type_enum::steel:
            type = "steel";

    }
    return type;
}

/**
 * Convert the value of enumerator 'target' to string
 * @param creature Creature from which type will be taken
 * @return String corresponding to enum
 */
auto target_string(creature const &creature) -> std::string {
    auto target = std::string();

    switch (creature.getSpecialPowerAtr().getTarget()) {
        case special_power::target_enum::this_creature:
            target = "this creature";
            break;
        case special_power::target_enum::single_team:
            target = "teammate";
            break;
        case special_power::target_enum::group_team:
            target = "my group";
            break;
        case special_power::target_enum::single_enemy:
            target = "enemy";
            break;
        case special_power::target_enum::group_enemy:
            target = "enemy group";
            break;
    }


    return target;
}

/**
 * Convert the value of enumerator 'attribute' to string
 * @param creature Creature from which type will be taken
 * @return String corresponding to enum
 */
auto attribute_string(creature const &creature) -> std::string {
    auto attribute = std::string();

    switch (creature.getSpecialPowerAtr().getAttribute()) {
        case special_power::attribute_enum::strength:
            attribute = "strength";
            break;
        case special_power::attribute_enum::dexterity:
            attribute = "dexterity";
            break;
        case special_power::attribute_enum::health_points:
            attribute = "hp";
            break;
    }
    return attribute;
}

/**
 * Convert the value of enumerator 'difficulty' to string
 * @param creature Creature from which type will be taken
 * @return String corresponding to enum
 */
auto difficulty_string(difficulty_enum const &difficulty) -> std::string {
    auto to_return = std::string();
    switch (difficulty) {
        case difficulty_enum::easy :
            to_return = "easy";
            break;
        case difficulty_enum::medium :
            to_return = "medium";
            break;
        case difficulty_enum::hard :
            to_return = "hard";
            break;
    }
    return to_return;
}

/**
 * Convert the value of string to enumerator 'type'
 * @param string String to be converted
 * @return Enumerator that corresponds to string
 */
auto string_type(std::string const &string) {
    if (string == "water") {
        return creature::type_enum::water;
    } else if (string == "earth") {
        return creature::type_enum::earth;
    } else if (string == "air") {
        return creature::type_enum::air;
    } else if (string == "fire") {
        return creature::type_enum::fire;
    } else if (string == "ice") {
        return creature::type_enum::ice;
    } else if (string == "steel") {
        return creature::type_enum::steel;
    }
    return creature::type_enum::water;
}

/**
 * Convert the value of string to enumerator 'target'
 * @param string String to be converted
 * @return Enumerator that corresponds to string
 */
auto string_target(std::string const &string) {
    if (string == "this creature") {
        return special_power::target_enum::this_creature;
    } else if (string == "teammate") {
        return special_power::target_enum::single_team;
    } else if (string == "my group") {
        return special_power::target_enum::group_team;
    } else if (string == "enemy") {
        return special_power::target_enum::single_enemy;
    } else if (string == "enemy group") {
        return special_power::target_enum::group_enemy;
    }
    return special_power::target_enum::this_creature;
}

/**
 * Convert the value of string to enumerator 'attribute'
 * @param string String to be converted
 * @return Enumerator that corresponds to string
 */
auto string_attribute(std::string const &string) {
    if (string == "strength") {
        return special_power::attribute_enum::strength;
    } else if (string == "dexterity") {
        return special_power::attribute_enum::dexterity;
    } else if (string == "hp") {
        return special_power::attribute_enum::health_points;
    }
    return special_power::attribute_enum::strength;
}

/**
 * Convert the value of string to enumerator 'difficulty'
 * @param string String to be converted
 * @return Enumerator that corresponds to string
 */
auto string_difficulty(std::string const &string) {
    if (string == "easy") {
        return difficulty_enum::easy;
    } else if (string == "medium") {
        return difficulty_enum::medium;
    } else if (string == "hard") {
        return difficulty_enum::hard;
    }
    return difficulty_enum::easy;
}