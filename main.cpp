#include <iostream>
#include <iomanip>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include "creatures.hpp"
#include "enum_strings.hpp"
#include "attack_or_spec.hpp"
#include "help.hpp"
#include "creation_of_game.hpp"


auto main() -> int {
    std::cout << "Start new game or load old one(-new/-load): \n";
    auto str = std::string();
    auto again = true;

    auto match = 1;
    auto my_team = std::vector<creature>();
    auto creatures = create_vector();
    auto difficulty = difficulty_enum::easy;
    while (again) {
        std::getline(std::cin, str);
        if (str == "-new") {
            auto i = 0;
            for (auto const &creature: creatures) {
                auto type = type_string(creature);
                auto target = target_string(creature);
                auto attribute = attribute_string(creature);

                std::cout << ++i << ". " << creature.getName() << " (" << type << ", str: " << creature.getStrength()
                          << ", dex: " << creature.getDexterity() << ", hp: " << creature.get_hp() << ")"
                          << "\n" << creature.getSpecialPowerAtr().getName() << " (str: "
                          << creature.getSpecialPowerAtr().getStrength() << ", target: " << target << ", attribute: "
                          << attribute << ", length in rounds: " << creature.getSpecialPowerAtr().getLength() << ")"
                          << "\n\n";
            }

            std::cout << "Choose your team(numbers seperated by space(ex: 1 4 12 7 18 2)): \n";
            my_team = choose_team(creatures);

            std::cout << "Choose the difficulty(easy, medium, hard): \n";
            difficulty = choose_difficulty();


            match = 1;
            again = false;
        } else if (str == "-load") {
            std::cout << "Write absolute path to save file: \n";
            std::getline(std::cin, str);
            auto load = std::fstream(str, std::ios::in);
            auto i = -2;
            while (std::getline(load, str)) {
                if (i == -2) {
                    match = std::stoi(str);
                } else if (i == -1) {
                    difficulty = string_difficulty(str);
                } else {
                    auto creat = split_string(str, "|");
                    my_team.push_back(
                            creature(creat[0], string_type(creat[1]), std::stoi(creat[2]), std::stoi(creat[3]),
                                     std::stoi(creat[4]), std::stoi(creat[5]),
                                     special_power(creat[6], std::stoi(creat[7]), string_target(creat[8]),
                                                   string_attribute(creat[9]), std::stoi(creat[10]),
                                                   std::stoi(creat[11]))));
                }
                i++;
            }
            again = false;
        } else if (str == "-h" || str == "-help") {
            help();
            std::cout << "Start new game or load old one(-new/-load): \n";
            continue;
        } else {
            std::cout << "Unknown command. Try again: \n";
        }
    }
    auto end_of_game = false;
    while (!end_of_game) {
        auto enemy_team = std::vector<creature>();
        auto enemy_team_num = std::vector<int>();
        auto enemy_team_type = std::vector<creature::type_enum>();

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> dist(0, (int) creatures.size());

        for (auto i = 1; i <=
                         static_cast<std::underlying_type<difficulty_enum>::type>(difficulty); i++) { //Create creatures without repetitions
            auto rand = (int) dist(mt);                                                                //of creatures and types
            if (std::find(enemy_team_num.begin(), enemy_team_num.end(), rand) == enemy_team_num.end() &&
                std::find(enemy_team_type.begin(), enemy_team_type.end(), creatures[rand].getType()) ==
                enemy_team_type.end()) {
                enemy_team.push_back(creatures[rand]);
                enemy_team_num.push_back(rand);
                enemy_team_type.push_back(creatures[rand].getType());
            } else {
                i--;
            }

        }

        auto win = false;

        auto end = false;
        auto focus = 0;
        auto round = 1;
        auto toDo = std::map<int, std::vector<std::function<void()>>>();
        while (!end) {
            std::cout << "Match: " << match << "\n";
            std::cout << "Round: " << round << "\n";
            for (auto i = 0; i < my_team.size(); i++) {
                if (focus % 2 == 0 && focus / 2 == i && my_team[i].is_alive()) {
                    std::cout << std::setw(60) << std::left
                              << std::to_string(i + 1) + "." + my_team[i].getName() + " <--------------";
                } else if (!my_team[i].is_alive()) {
                    std::cout << std::setw(60) << std::left
                              << std::to_string(i + 1) + "." + my_team[i].getName() + " DEAD";
                } else {
                    std::cout << std::setw(60) << std::left << std::to_string(i + 1) + "." + my_team[i].getName();
                }
                if (i < enemy_team.size() && focus % 2 != 0 && focus / 2 == i && enemy_team[i].is_alive()) {
                    std::cout << std::to_string(i + 1) + "." + enemy_team[i].getName() + "<--------------" << "\n";
                } else if (i < enemy_team.size() && !enemy_team[i].is_alive()) {
                    std::cout << std::to_string(i + 1) + "." + enemy_team[i].getName() + " DEAD" << "\n";
                } else if (i < enemy_team.size()) {
                    std::cout << std::to_string(i + 1) + "." + enemy_team[i].getName() << "\n";
                } else {
                    std::cout << "\n";
                }
                std::cout << std::setw(60) << std::left
                          << " (" + type_string(my_team[i]) + ", str: " + std::to_string(my_team[i].getStrength()) +
                             ", dex: " + std::to_string(my_team[i].getDexterity()) + ", xp: " +
                             std::to_string(my_team[i].getExp()) + ", hp: " +
                             std::to_string(my_team[i].get_hp()) + ")";

                if (i < enemy_team.size()) {
                    std::cout << " (" + type_string(enemy_team[i]) + ", str: " +
                                 std::to_string(enemy_team[i].getStrength()) + ", dex: " +
                                 std::to_string(enemy_team[i].getDexterity()) + ", hp: " +
                                 std::to_string(enemy_team[i].get_hp()) + ")" << "\n\n";
                } else {
                    std::cout << "\n\n";
                }
            }
            if (focus % 2 == 0 && my_team[focus / 2].is_alive()) {
                attack_or_spec(my_team[focus / 2], my_team, enemy_team, toDo, round);
            } else if (focus % 2 != 0 && enemy_team[focus / 2].is_alive()) {
                sleep(1);
                attack_or_spec_bot(enemy_team[focus / 2], enemy_team, my_team, toDo, round);
            } else {
                focus++;
            }

            check_if_alive(my_team, enemy_team, focus / 2);

            if (((focus + 1) % 2 == 0 && !my_team[(focus + 1) / 2].is_alive()) ||
                ((focus + 1) % 2 != 0 && !enemy_team[(focus + 1) / 2].is_alive())) {
                focus++;
            }

            if (focus >= (my_team.size()) * 2) {
                focus = 0;
                round++;
                round_special_power(toDo, round);
            } else {
                focus++;
            }


            if (std::ranges::count_if(my_team, [](auto e) -> bool { return !e.is_alive(); }) == my_team.size()) {
                win = false;
                end = true;
            } else if (std::ranges::count_if(enemy_team, [](auto e) -> bool { return !e.is_alive(); }) ==
                       enemy_team.size()) {
                win = true;
                end = true;
            }
        }
        match++;

        if (match > static_cast<std::underlying_type<difficulty_enum>::type>(difficulty)) {
            end_of_game = true;
            break;
        }

        if (win) {
            resurrect_team(my_team);
            for (auto &e: my_team) {
                if (e.getExp() >= 100) {
                    std::cout << "\n\n\nYour creature " + e.getName() + " may evolve. Do you want to do this? (y/n)\n";
                    again = true;
                    while (again) {
                        std::getline(std::cin, str);
                        if (str == "y") {
                            std::cout
                                    << "Choose which 2 characteristics to increase(numbers seperated by space (ex: str hp)): \n"
                                       "str - strength +10\n"
                                       "dex - dex +5\n"
                                       "hp - heal points +20\n";
                            e.evolve();
                            again = false;
                            e.setExp(e.getExp() - 100);
                        } else if (str == "n") {
                            again = false;
                        } else if (str == "-h" || str == "-help") {
                            help();
                            continue;
                        } else {
                            std::cout << "Unknown command. Try again: \n";
                        }
                    }
                }
            }
            std::cout
                    << "\n\n\n\n\n --------You won the match--------\nSave the game or continue or exit(-save/-cont/-exit): \n";
            auto answer = std::string();
            again = true;
            while (again) {
                std::getline(std::cin, answer);
                if (answer == "-save") {
                    std::cout
                            << "Write absolute path to the file, where you want to save the game(file should already exists): \n";
                    std::getline(std::cin, answer);
                    auto save = std::fstream(answer, std::ios::out | std::ios::trunc);
                    save << match << "\n";
                    save << difficulty_string(difficulty) << "\n";
                    for (auto const &e: my_team) {
                        save << e.getName() << "|" << type_string(e) << "|" << e.getStrength() << "|"
                             << e.getDexterity() << "|" << e.get_hp() << "|" << e.getExp() << "|"
                             << e.getSpecialPowerAtr().getName() << "|" << e.getSpecialPowerAtr().getStrength() << "|"
                             << target_string(e) << "|" << attribute_string(e) + "|"
                             << e.getSpecialPowerAtr().getLength() << "|" << e.getSpecialPowerAtr().getUsage() << "|\n";
                    }
                    std::cout << "Saved.\n";
                    std::cout << "Continue or exit(-save/-cont/-exit): \n";
                } else if (answer == "-cont") {
                    again = false;
                } else if (answer == "-exit") {
                    std::exit(0);
                } else if (answer == "-h" || answer == "-help") {
                    help();
                    std::cout
                            << "\n\n\n\n\n --------You won the match--------\nSave the game or continue or exit(-save/-cont/-exit): \n";
                    continue;
                }

            }


            std::cout << "\n\n\n\n\n";
        } else {
            std::cout << "\n\n\n\n\n--------You lose the game--------\n";
            end_of_game = true;
        }
    }

    std::cout << "--------GAME OVER--------\n";
    std::cout << "Do you want to restart(y/n)? \n";
    again = true;
    while (again) {
        std::getline(std::cin, str);
        if (str == "y") {
            again = false;
            main();
            return 0;
        } else if (str == "n") {
            again = false;
            return 0;
        } else {
            std::cout << "Unknown command. Try again: \n";
            continue;
        }
    }
}