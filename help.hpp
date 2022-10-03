#pragma once

#include <iostream>

/**
 * Shows information about game
 */
auto help() -> void {
    std::cout << "-----Terminology\n"
                 "Strength(str) - defines how much damage can creature deal with singular attack\n"
                 "Dexterity(dex) - defines the chance of dodging an incoming attack\n"
                 "Health Points(hp) - defines, how much damage creature can take before fainting\n"
                 "Special  power - defines  the  characteristics  of  special  power  for  a  creature  and  the maximum\n"
                 "number of uses during one battle\n"
                 "Experience Points(EXP) - how  many  Experience  points  a  creature  can  gain  by defeating the enemy\n\n"
                 "-----Preparation part\n"
                 "You should choose 6 creatures from the list of all the creatures. That will be your team. After that\n "
                 "you should choose the difficulty(easy, medium, hard) that will effect number of enemies and number of\n"
                 "their creatures.\n\n"
                 "-----Battle part\n"
                 "Battle consists of pre-defined number of matches(4, 5 or 6, depends on difficulty). At the end of the\n"
                 "match (if you won) you can save game or continue. Every match consists of undefined number of round\n"
                 "(rounds will go unless one team kill another). Every round consists of turns (one creature-one round).\n "
                 "Round ends when last creature had a turn. In your turn you can attack, use a special power or skip.\n"
                 "Game ends when ends last match.\n\n"
                 "-----Creatures\n"
                 "Creatures have name, type (water, earth, air, fire, ice, steel), strength, dexterity, hp and special\n"
                 "power. Different types have different efficiency attacking other types. It could be visualize by this scheme: \n"
                 "Water: steel-increased | water, fire, ice-lessened\n"
                 "Earth: water, ice-increased | air, fire-lessened\n"
                 "Air: steel-increased | earth-lessened\n"
                 "Fire: water, earth-increased | ice, steel-lessened\n"
                 "Ice: earth, air, fire-increased | ice-lessened\n"
                 "Steel: earth, fire-increased | air, steel-lessened\n\n"
                 "-----Special Power\n"
                 "Special power have several characteristics: name, strength, target(this creature, single teammate,\n"
                 "group of teammates, single enemy, group if enemies), attribute it affects(positive for teammates,\n"
                 "negative for enemies)(strength, dexterity, health_points), length in rounds, how many usages left.\n\n"
                 "-----Evolution\n"
                 "After gain 100 xp creature may evolve and you can choose 2 characteristics to increase(strength, \n"
                 "dexterity, hp).\n";
}