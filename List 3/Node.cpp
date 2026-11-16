//
// Created by kstol on 10.11.2026.
//

#include "Node.h"

#include <cmath>
#include <iostream>
#include <sstream>

bool isNumber(const std::string& token) {
    if (token.empty()) {
        return false;
    }
    for (int i = 0; i < token.size(); i++) {
        if (!isdigit(token[i])) {
            return false;
        }
    }
    return true;
}

Node::Node(): value("1") {
    determine(value);
}

Node::Node(std::string token): value(token) {
    determine(value);
}

Node::~Node() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
    children.clear();
}

Node::Node(const Node &other):
    value(other.value),
    type(other.type) {
    for (int i = 0; i < other.children.size(); i++) {
        children.push_back(new Node(*other.children[i]));
    }
}

std::vector<Node*> Node::getChildren() {
    return children;
}

void Node::determine(const std::string& token) {
    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "sin" || token == "cos") {
        type = op;
    } else if (isNumber(token)) {
        type = num;
    } else {
        type = var;
    }
}

double Node::getNumericValue() {
    double result;
    std::istringstream(value) >> result;
    return result;
}

void Node::build(const std::vector<std::string> &tokens, int &offset, bool &corrected) {
    if (offset >= tokens.size()) {
        corrected = true;
        value = "1";
        determine(value);
        return;
    }

    value = tokens[offset];
    offset++;
    determine(value);

    int needed = 0;
    if (type == op) {
        if (value == "sin" || value == "cos") {
            needed = 1;
        } else {
            needed = 2;
        }
    }

    for (int i = 0; i < needed; i++) {
        Node* child = new Node();
        child->build(tokens, offset, corrected);
        children.push_back(child);
    }
}

std::string Node::toString() {
    std::string result = value;
    for (int i = 0; i < children.size(); i++) {
        result += " " + children[i]->toString();
    }
    return result;
}

void Node::vars(std::vector<std::string>& vars) {
    if (type == var) {
        bool alreadyIn = false;
        for (int i = 0; i < vars.size(); i++) {
            if (vars[i] == value) {
                alreadyIn = true;
            }
        }
        if (!alreadyIn) {
            vars.push_back(value);
        }
    }

    for (int i = 0; i < children.size(); i++) {
        children[i]->vars(vars);
    }
}

double Node::comp(std::map<std::string, double> &varsMap) {
    if (type == num) {
        return getNumericValue();
    }

    if (type == var) {
        return varsMap[value];
    }

    std::vector<double> args;
    for (int i = 0; i < children.size(); i++) {
        args.push_back(children[i]->comp(varsMap));
    }

    if (args.empty()) {
        return 0.0;
    }

    if (value == "+") {
        double res = 0.0;
        for (int i = 0; i < args.size(); i++) {
            res += args[i];
        }
        return res;
    }

    if (value == "-") {
        double res = args[0];
        for (int i = 1; i < args.size(); i++) {
            res -= args[i];
        }
        return res;
    }

    if (value == "*") {
        double res = 1.0;
        for (int i = 0; i < args.size(); i++) {
            res *= args[i];
        }
        return res;
    }

    if (value == "/") {
        double res = args[0];
        for (int i = 1; i < args.size(); i++) {
            if (args[i] == 0.0) {
                std::cout << "Cant divide by zero" << std::endl;
                return 0.0;
            }
            res /= args[i];
        }
        return res;
    }

    if (value == "sin") {
        return std::sin(args[0]);
    }

    if (value == "cos") {
        return std::cos(args[0]);
    }

    return 0.0;
}

void Node::joinNode(Node* otherRoot) {
    if (!children.empty()) {
        for (int i = 0; i < children.size(); i++) {
            if (children[i]->children.empty()) {
                children[i] = new Node(*otherRoot);
                return;
            }
            children[i]->joinNode(otherRoot);
        }
    }
}
