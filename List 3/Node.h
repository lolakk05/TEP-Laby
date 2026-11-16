//
// Created by kstol on 10.11.2026.
//

#ifndef LIST_3__NODE_H
#define LIST_3__NODE_H
#include <map>
#include <string>
#include <vector>

enum Type {
    op,
    num,
    var,
};

class Node {
private:
    std::string value;
    std::vector<Node*> children;
    Type type;

    void determine(const std::string& token);
    double getNumericValue();
public:
    Node();
    Node(std::string value);
    ~Node();
    Node(const Node& other);

    std::vector<Node*> getChildren();

    void build(const std::vector<std::string>& tokens, int& offset, bool& corrected);
    std::string toString();
    void vars(std::vector<std::string>& vars);
    double comp(std::map<std::string, double>& varsMap);

    void joinNode(Node* otherNode);
};


#endif //LIST_3__NODE_H
