//
// Created by kstol on 1.11.2026.
//

#ifndef LIST_2_NUMBER_H
#define LIST_2_NUMBER_H
#include <string>


class Number {
    public:
        Number(unsigned int size);
        Number(const Number& other);
        ~Number();
        // Number& operator=(const Number& other);
        Number& operator=(Number other);
        Number& operator=(const int value);

        Number operator+(const Number &other);
        Number operator+(const Number& other) const;
        Number operator-(const Number& other) const;
        Number operator*(const Number& other) const;
        Number operator/(const Number& other) const;
        std::string operator()(const int index) const;
        Number operator%(const Number& other) const;
        Number operator-(const Number &other);
        Number operator*(const Number &other);
        Number operator/(const Number &other);
        bool operator>(const Number& other) const;
        bool operator==(const Number& other) const;
        bool operator>=(const Number& other) const;
        std::string toStr();
        bool absGreater(const Number& other) const;
        int getSize() const;

    private:
        int* pTab;
        unsigned int size;
        bool negative;
        void trimTab();
        void fillTabWithValue();
};


#endif //LIST_2_NUMBER_H
