#pragma once

#include <map>
#include <thread>
#include <functional>
#include "help.hpp"

/**
 * Checks heal points of all creatures of both teams and if the heal point less then 0 kills this creature.
 * If it is creature from enemy_team also adds 50 exp to creature from my_team who killed it
 * @param my_team First vector of creatures to be checked
 * @param enemy_team Second vector of creatures to be checked
 * @param target Defines which creature should get exp
 */
auto check_if_alive(std::vector<creature> &my_team, std::vector<creature> &enemy_team, int target) -> void {
    for (auto &e: my_team) {
        if (e.get_hp() <= 0 || !e.is_alive()) {
            e.kill();
            e.set_zero_hp();
        }
    }

    for (auto &e: enemy_team) {
        if (e.get_hp() <= 0 || !e.is_alive()) {
            if (e.is_alive()) {
                my_team[target].add_exp(50);
            }
            e.kill();
            e.set_zero_hp();
        }
    }
}


/**
 * Check how efficient was the attack by compering type of my_creature and enemy_creature
 * @param my_creature creature who attacks
 * @param enemy_creature creature who defends
 * @param strength number that should be changed depending on types of creatures
 * @return number that corresponds to changed strength
 */
auto efficiency_check(creature const &my_creature, creature const &enemy_creature, int strength) -> int {
    switch (my_creature.getType()) {
        case creature::type_enum::water:
            switch (enemy_creature.getType()) {
                case creature::type_enum::water:
                case creature::type_enum::fire:
                case creature::type_enum::ice:
                    strength = (int) (strength * 0.5);
                    break;
                case creature::type_enum::steel:
                    strength = (int) (strength * 1.5);
                    break;
            }
            break;
        case creature::type_enum::earth:
            switch (enemy_creature.getType()) {
                case creature::type_enum::air:
                case creature::type_enum::fire:
                    strength = (int) (strength * 0.5);
                    break;
                case creature::type_enum::water:
                case creature::type_enum::ice:
                    strength = (int) (strength * 1.5);
                    break;
            }
            break;
        case creature::type_enum::air:
            switch (enemy_creature.getType()) {
                case creature::type_enum::earth:
                    strength = (int) (strength * 0.5);
                    break;
                case creature::type_enum::steel:
                    strength = (int) (strength * 1.5);
                    break;
            }
            break;
        case creature::type_enum::fire:
            switch (enemy_creature.getType()) {
                case creature::type_enum::ice:
                case creature::type_enum::steel:
                    strength = (int) (strength * 0.5);
                    break;
                case creature::type_enum::water:
                case creature::type_enum::earth:
                    strength = (int) (strength * 1.5);
                    break;
            }
            break;
        case creature::type_enum::ice:
            switch (enemy_creature.getType()) {
                case creature::type_enum::ice:
                    strength = (int) (strength * 0.5);
                    break;
                case creature::type_enum::earth:
                case creature::type_enum::air:
                case creature::type_enum::fire:
                    strength = (int) (strength * 1.5);
                    break;
            }
            break;
        case creature::type_enum::steel:
            switch (enemy_creature.getType()) {
                case creature::type_enum::air:
                case creature::type_enum::steel:
                    strength = (int) (strength * 0.5);
                    break;
                case creature::type_enum::earth:
                case creature::type_enum::fire:
                    strength = (int) (strength * 1.5);
                    break;
            }
            break;
    }
    return strength;
}

/**
 * Continuing effect of special powers. Other words, repeats effect if it`s attribute is hp or stops if other attributes.
 * @param creat Creature which use special power
 * @param teammate Creature that will be used if special power has a target "single_team"
 * @param enemy Creature that will be used if special power has a target "single_enemy"
 * @param my_team Vector of creatures that will be used if special power has a target "group_team"
 * @param enemy_team Vector of creature that will be used if special power has a target "group_enemy"
 */
auto continue_spec(creature &creat, creature &teammate, creature &enemy, std::vector<creature> &my_team,
                   std::vector<creature> &enemy_team) -> void {
    auto str = creat.getSpecialPowerAtr().getStrength();
    auto tmp_sp = creat.getSpecialPowerAtr();

    switch (creat.getSpecialPowerAtr().getTarget()) {
        case special_power::target_enum::this_creature:
            switch (creat.getSpecialPowerAtr().getAttribute()) {
                case special_power::attribute_enum::strength:
                    creat.add_strength(-str);
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::dexterity:
                    creat.add_dexterity(-str);
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::health_points:
                    std::cout << "Continuing of effect " << creat.getSpecialPowerAtr().getName() << ": \n";
                    creat.add_hp(str);
                    std::cout << creat.getName() << " " << attribute_string(creat) << " increased on " << str << "\n\n";
                    break;
            }
            break;
        case special_power::target_enum::single_team:
            str = efficiency_check(creat, teammate, creat.getSpecialPowerAtr().getStrength());
            switch (creat.getSpecialPowerAtr().getAttribute()) {
                case special_power::attribute_enum::strength:
                    teammate.add_strength(-str);
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::dexterity:
                    teammate.add_dexterity(-str);
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::health_points:
                    std::cout << "Continuing of effect " << creat.getSpecialPowerAtr().getName() << ": \n";
                    teammate.add_hp(str);
                    std::cout << teammate.getName() << " " << attribute_string(creat) << " increased on " << str
                              << ",because: " << type_string(creat) << " -> " << type_string(teammate) << "\n\n";
                    break;
            }
            break;
        case special_power::target_enum::group_team:
            switch (creat.getSpecialPowerAtr().getAttribute()) {
                case special_power::attribute_enum::strength:
                    for (auto i = 0; i < my_team.size(); i++) {
                        str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                        my_team[i].add_strength(-str);
                    }
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::dexterity:
                    for (auto i = 0; i < my_team.size(); i++) {
                        str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                        my_team[i].add_dexterity(-str);
                    }
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::health_points:
                    std::cout << "Continuing of effect " << creat.getSpecialPowerAtr().getName() << ": \n";
                    for (auto i = 0; i < my_team.size(); i++) {
                        str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                        my_team[i].add_hp(str);
                        std::cout << my_team[i].getName() << " " << attribute_string(creat) << " increased on " << str
                                  << ",because: " << type_string(creat) << " -> " << type_string(my_team[i]) << "\n";
                    }
                    break;
            }
            std::cout << "\n";
            break;
        case special_power::target_enum::single_enemy:
            str = efficiency_check(creat, enemy, creat.getSpecialPowerAtr().getStrength());
            switch (creat.getSpecialPowerAtr().getAttribute()) {
                case special_power::attribute_enum::strength:
                    enemy.add_strength(str);
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::dexterity:
                    enemy.add_dexterity(str);
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::health_points:
                    std::cout << "Continuing of effect " << creat.getSpecialPowerAtr().getName() << ": \n";
                    enemy.add_hp(-str);
                    std::cout << enemy.getName() << " " << attribute_string(creat) << " decreased on " << str
                              << ",because: " << type_string(creat) << " -> " << type_string(enemy) << "\n\n";
                    break;
            }
            break;
        case special_power::target_enum::group_enemy:
            switch (creat.getSpecialPowerAtr().getAttribute()) {
                case special_power::attribute_enum::strength:
                    for (auto i = 0; i < enemy_team.size(); i++) {
                        str = efficiency_check(creat, enemy_team[i], creat.getSpecialPowerAtr().getStrength());
                        enemy_team[i].add_strength(str);
                    }
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::dexterity:
                    for (auto i = 0; i < enemy_team.size(); i++) {
                        str = efficiency_check(creat, enemy_team[i], creat.getSpecialPowerAtr().getStrength());
                        enemy_team[i].add_dexterity(str);
                    }
                    std::cout << "Effect " + creat.getSpecialPowerAtr().getName() + " is stopped\n\n";
                    break;
                case special_power::attribute_enum::health_points:
                    std::cout << "Continuing of effect " << creat.getSpecialPowerAtr().getName() << ": \n";
                    for (auto i = 0; i < enemy_team.size(); i++) {
                        str = efficiency_check(creat, enemy_team[i], creat.getSpecialPowerAtr().getStrength());
                        enemy_team[i].add_hp(-str);
                        std::cout << enemy_team[i].getName() << " " << attribute_string(creat) << " decreased on "
                                  << str << ",because: " << type_string(creat) << " -> " << type_string(enemy_team[i])
                                  << "\n";
                    }
                    break;
            }
            std::cout << "\n";
            break;
    }
}


/**
 * Using for interaction with player. Player can choose what to do (attack, use special power or skip) and also might
 * choose the target.
 * @param creat Creature that will be used for interaction
 * @param my_team Vector of creatures that will be used if special power has a target "group_team"
 * @param enemy_team Vector of creatures that will be used if special power has a target "group_enemy"
 * @param to_do A map with pair(round, vector of functions) are used for storing information about continuing (length > 0)
 *              special powers
 * @param round defines which round is in the main game now
 */
auto attack_or_spec(creature &creat, std::vector<creature> &my_team, std::vector<creature> &enemy_team,
                    std::map<int, std::vector<std::function<void()>>> &to_do, int round) -> void {
    std::cout << "Attack, use special power or skip the round ('a' or 'sp' or '-spec' to see spec.power or 'skip'): \n";

    auto line = std::string();
    std::getline(std::cin, line);
    try {
        if (line == "a") {
            if (creat.getStrength() <= 0) {
                std::cout << "The strength of the creature is 0 or lower, it cannot attack.\n";
                attack_or_spec(creat, my_team, enemy_team, to_do, round);
                return;
            }

            std::cout << "Choose your target (number near name): \n";
            std::getline(std::cin, line);


            if (std::stoi(line) < 1 || std::stoi(line) > enemy_team.size()) {
                std::cout << "--- Target not exists---\n";
                attack_or_spec(creat, my_team, enemy_team, to_do, round);
                return;
            }
            else if (!enemy_team[std::stoi(line) - 1].is_alive()) {
                std::cout << "---This creature is dead---\n";
                attack_or_spec(creat, my_team, enemy_team, to_do, round);
                return;
            }

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<> dist_dex(0, 100);
            if ((int) (dist_dex(mt)) > enemy_team[std::stoi(line) - 1].getDexterity()) {
                auto str = efficiency_check(creat, enemy_team[std::stoi(line) - 1], creat.getStrength());
                enemy_team[std::stoi(line) - 1].add_hp(-str);
                std::cout << "----" << creat.getName() << " attacked " << enemy_team[std::stoi(line) - 1].getName()
                          << " with damage: " << str << ", because: " << type_string(creat) << " -> "
                          << type_string(enemy_team[std::stoi(line) - 1]) << "----\n\n";
            } else {
                std::cout << "----" << creat.getName() << " missed----\n\n";
            }
        }
        else if (line == "-spec") {
            std::cout << "Special: " + creat.getSpecialPowerAtr().getName() + " (" + "str: " +
                         std::to_string(creat.getSpecialPowerAtr().getStrength()) + ", target: " +
                         target_string(creat) + ", attribute: " + attribute_string(creat) + ", length in rounds: " +
                         std::to_string(creat.getSpecialPowerAtr().getLength()) + ", usages left: " +
                         std::to_string(creat.getSpecialPowerAtr().getUsage()) + ")\n";
            attack_or_spec(creat, my_team, enemy_team, to_do, round);
            return;
        } else if (line == "sp") {
            auto str = creat.getSpecialPowerAtr().getStrength();
            if (creat.getSpecialPowerAtr().getUsage() > 0) {
                switch (creat.getSpecialPowerAtr().getTarget()) {
                    case special_power::target_enum::this_creature:
                        std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                                  << " with effect: \n";
                        switch (creat.getSpecialPowerAtr().getAttribute()) {
                            case special_power::attribute_enum::strength:
                                creat.add_strength(str);
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, creat, my_team, enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::dexterity:
                                creat.add_dexterity(str);
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, creat, my_team, enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::health_points:
                                creat.add_hp(str);
                                if (creat.getSpecialPowerAtr().getLength() > 0) {
                                    for (int i = 1; i < creat.getSpecialPowerAtr().getLength(); i++) {
                                        to_do[round + i].push_back([&creat, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, creat, my_team, enemy_team);
                                        });
                                    }
                                }
                                break;
                        }
                        std::cout << creat.getName() << " " << attribute_string(creat) << " increased on " << str
                                  << "\n\n";
                        break;
                    case special_power::target_enum::single_team:
                        std::cout << "Choose your target (number near name): \n";
                        std::getline(std::cin, line);
                        str = efficiency_check(creat, my_team[std::stoi(line) - 1],
                                               creat.getSpecialPowerAtr().getStrength());
                        if (std::stoi(line) < 1 || std::stoi(line) > enemy_team.size()) {
                            std::cout << "--- Target not exists---\n";
                            attack_or_spec(creat, my_team, enemy_team, to_do, round);
                            return;
                        }
                        else if (!my_team[std::stoi(line) - 1].is_alive()) {
                            std::cout << "---This creature is dead---\n";
                            attack_or_spec(creat, my_team, enemy_team, to_do, round);
                            return;
                        }
                        std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                                  << " with effect: \n";
                        switch (creat.getSpecialPowerAtr().getAttribute()) {
                            case special_power::attribute_enum::strength:
                                my_team[std::stoi(line) - 1].add_strength(str);
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &line, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, my_team[std::stoi(line) - 1], creat, my_team,
                                                          enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::dexterity:
                                my_team[std::stoi(line) - 1].add_dexterity(str);
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &line, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, my_team[std::stoi(line) - 1], creat, my_team,
                                                          enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::health_points:
                                my_team[std::stoi(line) - 1].add_hp(str);
                                for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                    auto &teammate = my_team[std::stoi(line) - 1];
                                    to_do[round + i].push_back([&creat, &teammate, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, teammate, creat, my_team, enemy_team);
                                    });
                                }
                                break;
                        }
                        std::cout << my_team[std::stoi(line) - 1].getName() << " " << attribute_string(creat)
                                  << " increased on " << str << ",because: " << type_string(creat) << " -> "
                                  << type_string(my_team[std::stoi(line) - 1]) << "\n\n";
                        break;
                    case special_power::target_enum::group_team:
                        std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                                  << " with effect: \n";
                        switch (creat.getSpecialPowerAtr().getAttribute()) {
                            case special_power::attribute_enum::strength:
                                for (auto i = 0; i < my_team.size(); i++) {
                                    str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                                    my_team[i].add_strength(str);
                                    std::cout << my_team[i].getName() << " " << attribute_string(creat)
                                              << " increased on " << str << ",because: " << type_string(creat) << " -> "
                                              << type_string(my_team[i]) << "\n";
                                }
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, creat, my_team, enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::dexterity:
                                for (auto i = 0; i < my_team.size(); i++) {
                                    str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                                    my_team[i].add_dexterity(str);
                                    std::cout << my_team[i].getName() << " " << attribute_string(creat)
                                              << " increased on " << str << ",because: " << type_string(creat) << " -> "
                                              << type_string(my_team[i]) << "\n";
                                }
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, creat, my_team, enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::health_points:
                                for (auto i = 0; i < my_team.size(); i++) {
                                    str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                                    my_team[i].add_hp(str);
                                    std::cout << my_team[i].getName() << " " << attribute_string(creat)
                                              << " increased on " << str << ",because: " << type_string(creat) << " -> "
                                              << type_string(my_team[i]) << "\n";
                                }
                                for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                    to_do[round + i].push_back([&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                                }
                                break;
                        }
                        std::cout << "\n";
                        break;
                    case special_power::target_enum::single_enemy:
                        std::cout << "Choose your target (number near name): \n";
                        std::getline(std::cin, line);
                        str = efficiency_check(creat, enemy_team[std::stoi(line) - 1],
                                               creat.getSpecialPowerAtr().getStrength());
                        if (std::stoi(line) < 1 || std::stoi(line) > enemy_team.size()) {
                            std::cout << "--- Target not exists---\n";
                            attack_or_spec(creat, my_team, enemy_team, to_do, round);
                            return;
                        }
                        else if (!enemy_team[std::stoi(line) - 1].is_alive()) {
                            std::cout << "---This creature is dead---\n";
                            attack_or_spec(creat, my_team, enemy_team, to_do, round);
                            return;
                        }
                        std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                                  << " with effect: \n";
                        switch (creat.getSpecialPowerAtr().getAttribute()) {
                            case special_power::attribute_enum::strength:
                                enemy_team[std::stoi(line) - 1].add_strength(-str);
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &line, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, enemy_team[std::stoi(line) - 1], my_team,
                                                          enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::dexterity:
                                enemy_team[std::stoi(line) - 1].add_dexterity(-str);
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &line, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, enemy_team[std::stoi(line) - 1], my_team,
                                                          enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::health_points:
                                enemy_team[std::stoi(line) - 1].add_hp(-str);
                                for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                    auto &enemy = enemy_team[std::stoi(line) - 1];
                                    to_do[round + i].push_back([&creat, &enemy, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, enemy, my_team, enemy_team);
                                    });
                                }
                                break;
                        }
                        std::cout << enemy_team[std::stoi(line) - 1].getName() << " " << attribute_string(creat)
                                  << " decreased on " << str << ",because: " << type_string(creat) << " -> "
                                  << type_string(enemy_team[std::stoi(line) - 1]) << "\n\n";
                        break;
                    case special_power::target_enum::group_enemy:
                        std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                                  << " with effect: \n";
                        switch (creat.getSpecialPowerAtr().getAttribute()) {
                            case special_power::attribute_enum::strength:
                                for (auto i = 0; i < enemy_team.size(); i++) {
                                    str = efficiency_check(creat, enemy_team[i],
                                                           creat.getSpecialPowerAtr().getStrength());
                                    enemy_team[i].add_strength(-str);
                                    std::cout << enemy_team[i].getName() << " " << attribute_string(creat)
                                              << " decreased on " << str << ",because: " << type_string(creat) << " -> "
                                              << type_string(enemy_team[i]) << "\n";
                                }
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, creat, my_team, enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::dexterity:
                                for (auto i = 0; i < enemy_team.size(); i++) {
                                    str = efficiency_check(creat, enemy_team[i],
                                                           creat.getSpecialPowerAtr().getStrength());
                                    enemy_team[i].add_dexterity(-str);
                                    std::cout << enemy_team[i].getName() << " " << attribute_string(creat)
                                              << " decreased on " << str << ",because: " << type_string(creat) << " -> "
                                              << type_string(enemy_team[i]) << "\n";
                                }
                                to_do[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                        [&creat, &my_team, &enemy_team]() -> void {
                                            continue_spec(creat, creat, creat, my_team, enemy_team);
                                        });
                                break;
                            case special_power::attribute_enum::health_points:
                                for (auto i = 0; i < enemy_team.size(); i++) {
                                    str = efficiency_check(creat, enemy_team[i],
                                                           creat.getSpecialPowerAtr().getStrength());
                                    enemy_team[i].add_hp(-str);
                                    std::cout << enemy_team[i].getName() << " " << attribute_string(creat)
                                              << " decreased on " << str << ",because: " << type_string(creat) << " -> "
                                              << type_string(enemy_team[i]) << "\n";
                                }

                                for (int i = 1; i < creat.getSpecialPowerAtr().getLength(); i++) {
                                    to_do[round + i].push_back([&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                                }

                                break;
                        }
                        std::cout << "\n";
                        break;
                }
                special_power tmp = creat.getSpecialPowerAtr();
                tmp.used();
                creat.setSpecialPowerAtr(tmp);

            } else {
                std::cout << "No more usages of special ability" << "\n";
                attack_or_spec(creat, my_team, enemy_team, to_do, round);
                return;
            }

        } else if (line == "skip") {
            std::cout << "Skipped.\n";
            return;
        } else if (line == "-h" || line == "-help") {
            help();
            attack_or_spec(creat, my_team, enemy_team, to_do, round);
            return;
        } else {
            std::cout << "Unknown command\n";
            attack_or_spec(creat, my_team, enemy_team, to_do, round);
            return;
        }

    } catch (std::exception const &e) {
        std::cout << "Wrong target or another error. Try again: \n";
        attack_or_spec(creat, my_team, enemy_team, to_do, round);
        return;
    }
}

/**
 * Using for bots to interact with a game. Bots(enemies) automatically(using random) choose what they do
 * (attack, use special power or skip) and choose the target.
 * @param creat Creature that will be used for interaction
 * @param my_team Vector of creatures that will be used if special power has a target "group_team"
 * @param enemy_team Vector of creatures that will be used if special power has a target "group_enemy"
 * @param to_do A map with pair(round, vector of functions) are used for storing information about continuing special
 *              powers (length > 0)
 * @param round defines which round are in the main game now
 */
auto attack_or_spec_bot(creature &creat, std::vector<creature> &my_team, std::vector<creature> &enemy_team,
                        std::map<int, std::vector<std::function<void()>>> &toDo, int round) -> void {


    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<> dist_attack_spec(0, 4);
    std::uniform_real_distribution<> dist_target(0, 6);
    std::uniform_real_distribution<> dist_dex(0, 100);

    auto choose = (int) dist_attack_spec(mt);
    if (choose == 0 && creat.getSpecialPowerAtr().getUsage() > 0) {
        auto str = creat.getSpecialPowerAtr().getStrength();
        auto target = (int) dist_target(mt);
        if (creat.getSpecialPowerAtr().getUsage() > 0) {
            switch (creat.getSpecialPowerAtr().getTarget()) {
                case special_power::target_enum::this_creature:
                    std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                              << " with effect: \n";
                    switch (creat.getSpecialPowerAtr().getAttribute()) {
                        case special_power::attribute_enum::strength:
                            creat.add_strength(str);
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::dexterity:
                            creat.add_dexterity(str);
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::health_points:
                            creat.add_hp(str);
                            if (creat.getSpecialPowerAtr().getLength() > 0) {
                                for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                    toDo[round + i].push_back([&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                                }
                            }
                            break;
                    }
                    std::cout << creat.getName() << " " << attribute_string(creat) << " increased on " << str << "\n\n";
                    break;
                case special_power::target_enum::single_team:
                    while (!my_team[target].is_alive()){
                        target = (int)(dist_target(mt));
                    }
                    str = efficiency_check(creat, my_team[target], creat.getSpecialPowerAtr().getStrength());
                    std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                              << "with effect: \n";
                    if (!my_team[target].is_alive()) {
                        attack_or_spec_bot(creat, my_team, enemy_team, toDo, round);
                        return;
                    }
                    switch (creat.getSpecialPowerAtr().getAttribute()) {
                        case special_power::attribute_enum::strength:
                            my_team[target].add_strength(str);
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &target, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, my_team[target], creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::dexterity:
                            my_team[target].add_dexterity(str);
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &target, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, my_team[target], creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::health_points:
                            my_team[target].add_hp(str);
                            for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                auto &teammate = my_team[target];
                                toDo[round + i].push_back([&creat, &teammate, &my_team, &enemy_team]() -> void {
                                    continue_spec(creat, teammate, creat, my_team, enemy_team);
                                });
                            }
                            break;
                    }
                    std::cout << my_team[target].getName() << " " << attribute_string(creat) << " increased on " << str
                              << ",because: " << type_string(creat) << " -> " << type_string(my_team[target]) << "\n\n";
                    break;
                case special_power::target_enum::group_team:
                    std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                              << " with effect: \n";
                    switch (creat.getSpecialPowerAtr().getAttribute()) {
                        case special_power::attribute_enum::strength:
                            for (auto i = 0; i < my_team.size(); i++) {
                                str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                                my_team[i].add_strength(str);
                                std::cout << my_team[i].getName() << " " << attribute_string(creat) << " increased on "
                                          << str << ",because: " << type_string(creat) << " -> "
                                          << type_string(my_team[i]) << "\n";
                            }
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::dexterity:
                            for (auto i = 0; i < my_team.size(); i++) {
                                str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                                my_team[i].add_dexterity(str);
                                std::cout << my_team[i].getName() << " " << attribute_string(creat) << " increased on "
                                          << str << ",because: " << type_string(creat) << " -> "
                                          << type_string(my_team[i]) << "\n";
                            }
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::health_points:
                            for (auto i = 0; i < my_team.size(); i++) {
                                str = efficiency_check(creat, my_team[i], creat.getSpecialPowerAtr().getStrength());
                                my_team[i].add_hp(str);
                                std::cout << my_team[i].getName() << " " << attribute_string(creat) << " increased on "
                                          << str << ",because: " << type_string(creat) << " -> "
                                          << type_string(my_team[i]) << "\n";
                            }
                            for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                toDo[round + i].push_back([&creat, &my_team, &enemy_team]() -> void {
                                    continue_spec(creat, creat, creat, my_team, enemy_team);
                                });
                            }
                            break;
                    }

                    std::cout << "\n";
                    break;
                case special_power::target_enum::single_enemy:
                    while (!enemy_team[target].is_alive()){
                        target = (int)(dist_target(mt));
                    }
                    str = efficiency_check(creat, enemy_team[target], creat.getSpecialPowerAtr().getStrength());
                    std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                              << " with effect: \n";
                    if (!enemy_team[target].is_alive()) {
                        attack_or_spec_bot(creat, my_team, enemy_team, toDo, round);
                        return;
                    }
                    switch (creat.getSpecialPowerAtr().getAttribute()) {
                        case special_power::attribute_enum::strength:
                            enemy_team[target].add_strength(-str);
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &target, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, enemy_team[target], my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::dexterity:
                            enemy_team[target].add_dexterity(-str);
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &target, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, enemy_team[target], my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::health_points:
                            enemy_team[target].add_hp(-str);
                            for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                auto &enemy = enemy_team[target];
                                toDo[round + i].push_back([&creat, &enemy, &my_team, &enemy_team]() -> void {
                                    continue_spec(creat, creat, enemy, my_team, enemy_team);
                                });
                            }
                            break;
                    }
                    std::cout << enemy_team[target].getName() << " " << attribute_string(creat) << " decreased on "
                              << str << ",because: " << type_string(creat) << " -> " << type_string(enemy_team[target])
                              << "\n\n";
                    break;
                case special_power::target_enum::group_enemy:
                    std::cout << creat.getName() << " used " << creat.getSpecialPowerAtr().getName()
                              << "with effect: \n";
                    switch (creat.getSpecialPowerAtr().getAttribute()) {
                        case special_power::attribute_enum::strength:
                            for (auto i = 0; i < enemy_team.size(); i++) {
                                str = efficiency_check(creat, enemy_team[i], creat.getSpecialPowerAtr().getStrength());
                                enemy_team[i].add_strength(-str);
                                std::cout << enemy_team[i].getName() << " " << attribute_string(creat)
                                          << " decreased on " << str << ",because: " << type_string(creat) << " -> "
                                          << type_string(enemy_team[i]) << "\n";
                            }
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::dexterity:
                            for (auto i = 0; i < enemy_team.size(); i++) {
                                str = efficiency_check(creat, enemy_team[i], creat.getSpecialPowerAtr().getStrength());
                                enemy_team[i].add_dexterity(-str);
                                std::cout << enemy_team[i].getName() << " " << attribute_string(creat)
                                          << " decreased on " << str << ",because: " << type_string(creat) << " -> "
                                          << type_string(enemy_team[i]) << "\n";
                            }
                            toDo[round + creat.getSpecialPowerAtr().getLength() - 1].push_back(
                                    [&creat, &my_team, &enemy_team]() -> void {
                                        continue_spec(creat, creat, creat, my_team, enemy_team);
                                    });
                            break;
                        case special_power::attribute_enum::health_points:
                            for (auto i = 0; i < enemy_team.size(); i++) {
                                str = efficiency_check(creat, enemy_team[i], creat.getSpecialPowerAtr().getStrength());
                                enemy_team[i].add_hp(-str);
                                std::cout << enemy_team[i].getName() << " " << attribute_string(creat)
                                          << " decreased on " << str << ",because: " << type_string(creat) << " -> "
                                          << type_string(enemy_team[i]) << "\n";
                            }
                            for (int i = 1; i <= creat.getSpecialPowerAtr().getLength(); i++) {
                                toDo[round + i].push_back([&creat, &my_team, &enemy_team]() -> void {
                                    continue_spec(creat, creat, creat, my_team, enemy_team);
                                });
                            }

                            break;
                    }
                    std::cout << "\n";
                    break;
            }
            special_power tmp = creat.getSpecialPowerAtr();
            tmp.used();
            creat.setSpecialPowerAtr(tmp);

        }
    } else {
        if (creat.getStrength() <= 0) {
            return;
        }
        auto target = (int) dist_target(mt);
        while (!enemy_team[target].is_alive()) {
            target = (int) dist_target(mt);
        }
        if ((int) (dist_dex(mt)) > enemy_team[target].getDexterity()) {
            auto str = efficiency_check(creat, enemy_team[target], creat.getStrength());
            enemy_team[target].add_hp(-str);
            std::cout << "----" << creat.getName() << " attacked " << enemy_team[target].getName() << " with damage: "
                      << str << ", because: " << type_string(creat) << " -> " << type_string(enemy_team[target])
                      << "----\n\n";
        } else {
            std::cout << "----" << creat.getName() << " missed----\n\n";
        }
    }
}


/**
 * Calls the functions from to_do depends on round
 * @param to_do A map with pair(round, vector of functions) are used for storing information about continuing special
 *              powers (length > 0)
 * @param round defines which round are in the main game now
 */
auto round_special_power(std::map<int, std::vector<std::function<void()>>> &to_do, int round) -> void {
    if (!to_do[round].empty()) {
        for (auto &i: to_do[round]) {
            std::invoke(i);
        }
    }
}


/**
 * Call resurrect() method to all of the creatures of vector team
 * @param team Vector of creatures
 */
auto resurrect_team(std::vector<creature> &team) -> void {
    for (auto &e: team) {
        e.resurrect();
    }
}


