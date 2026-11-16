//
// Created by kstol on 10.11.2026.
//

#include "Tree.h"

#include <iostream>
#include <sstream>

std::vector<std::string> splitFormula(const std::string& formula) {
    std::vector<std::string> tokens;
    std::string currentToken;


    for (int i = 0; i < formula.length(); i++) {
        if (formula[i] != ' ') {
            currentToken += formula[i];
        } else {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken = "";
            }
        }
    }

    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    return tokens;
}

Tree::Tree() {
    root = new Node("1");
}
Tree::~Tree() {
    delete root;
}

Tree::Tree(const Tree &other) {
    if (other.root != NULL) {
        root = new Node(*other.root);
    } else {
        root = NULL;
    }
}

Tree& Tree::operator=(Tree other) {
    std::swap(root, other.root);
    return *this;
}

Tree Tree::operator+(const Tree &other) const {
    Tree result(*this);
    if (result.root->getChildren().empty()) {
        delete result.root;
        result.root = new Node(*other.root);
    } else {
        result.root->joinNode(other.root);
    }
    return result;
}

void Tree::enter(const std::string &formula) {
    std::vector<std::string> tokens = splitFormula(formula);

    delete root;
    root = new Node();

    int offset = 0;
    bool corrected = false;

    root->build(tokens, offset, corrected);

    if (corrected) {
        std::cout << "Missing arguments, filled with default value 1" << std::endl;
    }

    if (offset < tokens.size()) {
        std::cout << "Too many arguments, overflow arguments skipped" << std::endl;
    }

    if (corrected || offset < tokens.size()) {
        print();
    }
}

void Tree::print() {
    if (root != NULL) {
        std::cout << root->toString() << std::endl;
    }
}

std::vector<std::string> Tree::vars() {
    std::vector<std::string> vars;
    if (root != NULL) {
        root->vars(vars);
    }
    return vars;
}

void Tree::comp(const std::string& formula, std::vector<std::string> vars) {
    std::vector<std::string> tokens = splitFormula(formula);

    if (vars.size() != tokens.size()) {
        std::cout << "Invalid number of arguments" << std::endl;
        return;
    }

    double value;

    std::map<std::string, double> varsMap;
    for (int i = 0; i < tokens.size(); i++) {
        std::istringstream(tokens[i]) >> value;
        varsMap[vars[i]] = value;
    }

    if (root != NULL) {
        double result = root->comp(varsMap);
        std::cout << result << std::endl;
    }
}

void Tree::join(const std::string& formula) {
    Tree otherTree;
    otherTree.enter(formula);
    *this = *this + otherTree;
}
