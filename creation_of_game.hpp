#pragma once

#include "creatures.hpp"
#include <vector>


/**
 * Split string by pre-defined delimiter
 * @param text Sting to be split
 * @param delimiter Defines by what text should be split (" " - standard)
 * @return vector of strings after splitting
 */
auto split_string(std::string text, std::string const &delimiter = " ") -> std::vector<std::string> {
    text = text + " ";
    std::vector<std::string> to_return{};

    size_t pos;
    while ((pos = text.find(delimiter)) != std::string::npos) {
        to_return.push_back(text.substr(0, pos));
        text.erase(0, pos + delimiter.length());
    }

    return to_return;
}

class creature;

/**
 * Allows to choose team from the vector of creatures
 * @param creatures Vector of creatures
 * @return Vector of chosen creatures
 */
auto choose_team(std::vector<creature> const &creatures) -> std::vector<creature> {
    auto my_team = std::vector<creature>();
    auto again = true;
    while (again) {
        try {
            auto choose = std::string();
            std::getline(std::cin, choose);
            if (choose == "-h" || choose == "-help") {
                help();
                std::cout << "Choose your team(numbers seperated by space(ex: 1 4 12 7 18 2)): \n";
                continue;
            }
            my_team = std::vector<creature>();
            auto my_team_num = std::vector<int>();
            for (auto const &team: split_string(choose, " ")) {
                if (std::stoi(team) < 1 || std::stoi(team) > creatures.size()) {
                    throw std::range_error("Not in the list");
                }
                if (std::find(my_team_num.begin(), my_team_num.end(), std::stoi(team) - 1) == my_team_num.end()) {
                    my_team.push_back(creatures[std::stoi(team) - 1]);
                    my_team_num.push_back(std::stoi(team) - 1);
                } else {
                    throw std::range_error("Choose creatures without repetitions");
                }

            }
            if (my_team.size() != 6) {
                throw std::range_error("You should choose 6 creatures");
            }
            again = false;
        } catch (std::exception const &e) {
            std::cout << "Something went wrong [" << e.what() << "] Try again: \n";
        }
    }

    return my_team;
}

enum class difficulty_enum {
    easy = 4, medium = 5, hard = 6
};

/**
 * Allows player to choose the difficulty for the game
 * @return Chosen difficulty
 */
auto choose_difficulty() -> difficulty_enum {
    auto again = true;
    auto difficulty = difficulty_enum::easy;
    while (again) {
        auto str = std::string();
        std::getline(std::cin, str);
        if (str == "easy") {
            difficulty = difficulty_enum::easy;
            again = false;
        } else if (str == "medium") {
            difficulty = difficulty_enum::medium;
            again = false;
        } else if (str == "hard") {
            difficulty = difficulty_enum::hard;
            again = false;
        } else if (str == "-h" || str == "-help") {
            help();
            std::cout << "Choose the difficulty(easy, medium, hard): \n";
            continue;
        } else {
            std::cout << "Unknown difficulty. Try again: \n";
        }
    }
    return difficulty;
}