//
// Created by kstol on 15.11.2026.
//

#include "UIInterface.h"
#include <iostream>
#include <map>

#include "Tree.h"

void UIInterface::run() {
    Tree tree;
    std::string command;
    std::string formula;

    std::vector<std::string> vars;
    std::string varsString;

    while (true) {
        std::cout << "Enter command" << std::endl;
        std::getline(std::cin, command);

        int firstSpace = command.find(' ');
        formula = firstSpace == -1 ? "" : command.substr(firstSpace + 1);
        command = command.substr(0, firstSpace);

        if (command == "exit") {
            break;
        }
        if (command == "enter") {
            tree.enter(formula);
            vars = tree.vars();
            varsString = "";
            for (int i = 0; i < vars.size(); i++) {
                varsString += vars[i] + " ";
            }
        } else if (command == "print") {
            tree.print();
        } else if (command == "vars") {
            std::cout << varsString << std::endl;
        } else if (command == "comp") {
            tree.comp(formula, vars);
        } else if (command == "join") {
            tree.join(formula);
            tree.print();
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }
}
