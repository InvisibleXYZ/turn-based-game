#pragma once

#include <string>
#include <set>
#include <vector>
#include "help.hpp"
#include "creation_of_game.hpp"

/**
 * Defines special power that could be implemented to the creature
 */
class special_power {
public:
    enum class target_enum {
        this_creature, single_team, group_team, single_enemy, group_enemy
    };

    enum class attribute_enum {
        strength, dexterity, health_points
    };

private:
    std::string name;
    int strength;
    target_enum target;
    attribute_enum attribute;
    int length;
    int usage;
    int max_usage;

public:
    special_power(const std::string &name, int strength, target_enum const &target, attribute_enum const &attribute,
                  int length, int usage) : name(name), strength(strength), target(target), attribute(attribute),
                                           length(length), usage(usage), max_usage(usage) {}

    const std::string &getName() const {
        return name;
    }

    int getStrength() const {
        return strength;
    }

    target_enum getTarget() const {
        return target;
    }

    attribute_enum getAttribute() const {
        return attribute;
    }

    int getLength() const {
        return length;
    }

    int getUsage() const {
        return usage;
    }

    void used() {
        usage -= 1;
    }

    void restore() {
        usage = max_usage;
    }
};

/**
 * Defines creatures
 */
class creature {
public:
    enum class type_enum {
        water, earth, air, fire, ice, steel
    };

private:
    std::string name;
    type_enum type;
    int strength;
    int standard_strength;
    int dexterity;
    int standard_dexterity;
    int health_points;
    int max_hp;
    special_power special_power_atr;
    int exp = 0;
    bool alive = true;

public:
    creature(const std::string &name, type_enum type, int strength, int dexterity, int healthPoints, int exp,
             const special_power &specialPower) : name(name), type(type), strength(strength), dexterity(dexterity),
                                                  health_points(healthPoints), special_power_atr(specialPower),
                                                  exp(exp), max_hp(healthPoints), standard_strength(strength),
                                                  standard_dexterity(dexterity) {}

    const std::string &getName() const {
        return name;
    }

    type_enum getType() const {
        return type;
    }

    int getStrength() const {
        return strength;
    }

    int getDexterity() const {
        return dexterity;
    }

    int get_hp() const {
        return health_points;
    }

    special_power getSpecialPowerAtr() const {
        return special_power_atr;
    }

    void setSpecialPowerAtr(const special_power &specialPowerAtr) {
        special_power_atr = specialPowerAtr;
    }

    auto add_hp(int amount) -> void {
        health_points += amount;
    }

    auto add_strength(int amount) -> void {
        strength += amount;
    }

    auto add_dexterity(int amount) -> void {
        dexterity += amount;
    }

    auto add_exp(int amount) -> void {
        exp += amount;
    }

    int getExp() const {
        return exp;
    }

    void setExp(int exp) {
        creature::exp = exp;
    }

    auto kill() -> void {
        alive = false;
    }

    /**
     * Restores characteristics of creature to their standard or maximal values
     */
    auto resurrect() -> void {
        alive = true;
        health_points = max_hp;
        strength = standard_strength;
        dexterity = standard_dexterity;
        auto tmp_spec_power = getSpecialPowerAtr();
        tmp_spec_power.restore();
        setSpecialPowerAtr(tmp_spec_power);
    }

    void set_zero_hp() {
        health_points = 0;
    }

    bool is_alive() {
        return alive;
    }

    /**
     * Allows player choose what characteristics will be increased and increasing them
     */
    auto evolve() -> void {
        auto str = std::string();
        auto again = true;
        while (again) {
            auto str_vec = std::vector<std::string>();
            std::getline(std::cin, str);
            for (auto const &e: split_string(str, " ")) {
                for (auto const &e_in: split_string(str, " ")) {
                    if (!(e_in == "str" || e_in == "dex" || e_in == "hp" || e_in == "-h" || e_in == "-hello")) {
                        std::cout << "Some characteristic is unknown. Try again: \n";
                        evolve();
                        return;
                    }
                }
                if (e == "-h" || e == "-help") {
                    help();
                    std::cout
                            << "Choose which 2 characteristics to increase(numbers seperated by space (ex: str hp)): \n"
                               "str - strength +10\n"
                               "dex - dex +5\n"
                               "hp - heal points +20\n"
                               "sp - number of usages of special power +1\n";
                    break;
                } else if (split_string(str, " ").size() != 2) {
                    std::cout << "You need to choose exactly 2 characteristics\n";
                    break;
                } else if (e == "str") {
                    standard_strength += 10;
                    strength = standard_strength;
                    again = false;
                } else if (e == "dex") {
                    standard_dexterity += 5;
                    dexterity = standard_dexterity;
                    again = false;
                } else if (e == "hp") {
                    max_hp += 20;
                    health_points = max_hp;
                    again = false;
                }
            }
        }
    }
};

/**
 * Creates a vector of creatures
 * @return Vector of creatures
 */
auto create_vector() -> std::vector<creature> {
    auto special_power_vector = std::vector<special_power>();

    auto to_return = std::vector<creature>();
    to_return.push_back(creature("Siren", creature::type_enum::water, 20, 10, 200, 0,
                                 special_power("Sing", 10, special_power::target_enum::group_enemy,
                                               special_power::attribute_enum::strength, 3, 1)));
    to_return.push_back(creature("Golem", creature::type_enum::earth, 30, 10, 300, 0,
                                 special_power("Earthquake", 20, special_power::target_enum::group_enemy,
                                               special_power::attribute_enum::health_points, 0, 3)));
    to_return.push_back(creature("Giant Bird", creature::type_enum::air, 20, 10, 250, 0,
                                 special_power("Tornado", 10, special_power::target_enum::single_enemy,
                                               special_power::attribute_enum::health_points, 3, 2)));
    to_return.push_back(creature("Dragon", creature::type_enum::fire, 30, 10, 300, 0,
                                 special_power("Fire Breath", 40, special_power::target_enum::single_enemy,
                                               special_power::attribute_enum::health_points, 0, 2)));
    to_return.push_back(creature("Alive Blizzard", creature::type_enum::ice, 20, 10, 250, 0, // Not after last few years
                                special_power("Ice Wall", 30, special_power::target_enum::group_team,
                                                special_power::attribute_enum::dexterity, 3, 1)));
    to_return.push_back(creature("H.A.B.D.", creature::type_enum::steel, 30, 10, 300, 0,  // Heavy Attack Battle Droid
                                 special_power("Overcharging", 20, special_power::target_enum::this_creature,
                                               special_power::attribute_enum::strength, 3, 1)));
    to_return.push_back(creature("Healer", creature::type_enum::water, 15, 10, 200, 0,
                                 special_power("Healing rain", 30, special_power::target_enum::group_team,
                                               special_power::attribute_enum::health_points, 3, 1)));
    to_return.push_back(creature("Dwarf", creature::type_enum::earth, 20, 10, 250, 0,
                                 special_power("Underground system of spying", 30,
                                               special_power::target_enum::group_enemy,
                                               special_power::attribute_enum::dexterity, 3, 1)));
    to_return.push_back(creature("Harpy", creature::type_enum::air, 30, 10, 250, 0,
                                 special_power("Pack Raid", 30, special_power::target_enum::group_enemy,
                                               special_power::attribute_enum::health_points, 0, 2)));
    to_return.push_back(creature("Ifrit", creature::type_enum::fire, 20, 10, 300, 0,
                                 special_power("Fireball", 50, special_power::target_enum::single_enemy,
                                               special_power::attribute_enum::health_points, 0, 2)));
    to_return.push_back(creature("Jotun", creature::type_enum::ice, 30, 10, 350, 0,
                                 special_power("Word to the gods", 10, special_power::target_enum::group_team,
                                               special_power::attribute_enum::strength, 2, 1)));
    to_return.push_back(creature("Mercury", creature::type_enum::steel, 20, 10, 200, 0,
                                 special_power("Poisoning", 10, special_power::target_enum::group_enemy,
                                               special_power::attribute_enum::health_points, 5, 1)));
    to_return.push_back(creature("Hydra", creature::type_enum::water, 30, 10, 200, 0,
                                 special_power("Regeneration", 50, special_power::target_enum::this_creature,
                                               special_power::attribute_enum::health_points, 10, 1)));
    to_return.push_back(creature("Troll", creature::type_enum::earth, 30, 10, 350, 0,
                                 special_power("Use a tree as a weapon", 20, special_power::target_enum::this_creature,
                                               special_power::attribute_enum::strength, 3, 2)));
    to_return.push_back(creature("Angel", creature::type_enum::air, 30, 10, 300, 0,
                                 special_power("Brightness of the Sun", 30, special_power::target_enum::group_enemy,
                                               special_power::attribute_enum::dexterity, 3, 1)));
    to_return.push_back(creature("Phoenix", creature::type_enum::fire, 30, 10, 300, 0,
                                 special_power("Rebirth", 100, special_power::target_enum::this_creature,
                                               special_power::attribute_enum::health_points, 0, 1)));
    to_return.push_back(creature("Yeti", creature::type_enum::ice, 30, 10, 300, 0,
                                 special_power("Avalanche", 30, special_power::target_enum::group_enemy,
                                               special_power::attribute_enum::health_points, 0, 2)));
    to_return.push_back(creature("T.C.S.C", creature::type_enum::steel, 0, 10, 400, 0, //Trajectory Calculating Super Computer
                                special_power("Calculating Trajectory", 30, special_power::target_enum::group_enemy,
                                              special_power::attribute_enum::dexterity, 2, 5)));

    return to_return;
}