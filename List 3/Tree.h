//
// Created by kstol on 10.11.2026.
//

#ifndef LIST_3__TREE_H
#define LIST_3__TREE_H

#include <map>

#include "Node.h"

class Tree {
private:
    Node *root;
public:
    Tree();
    ~Tree();
    Tree(const Tree& other);

    Tree& operator=(Tree other);
    Tree operator+(const Tree& other) const;

    void enter(const std::string& formula);
    void print();
    std::vector<std::string> vars();
    void comp(const std::string& formula, std::vector<std::string> vars);

    void join(const std::string& formula);
};


#endif //LIST_3__TREE_H
