//
// Created by kstol on 1.11.2026.
//

#include "Number.h"

#include <cmath>
#include <string>
#include <iostream>
#include "utils.h"
#include <stdexcept>

Number::Number(unsigned int size)
  : size(size == 0 ? 1 : size),
    pTab(new int[size == 0 ? 1 : size]),
    negative(false)
{}

Number::Number(const Number& other):
    pTab(new int[other.size]),
    size(other.size),
    negative(other.negative) {
    for (int i = 0; i < size; i++) {
        pTab[i] = other.pTab[i];
    }
}

Number::~Number() {
    delete[] pTab;
}

Number& Number::operator=(const int value) {
    negative = value < 0;
    int abs = negative ? -value : value;
    int temp = abs;
    int length = 0;

    if (abs == 0) {
        delete [] pTab;
        size = 1;
        int* newTab = new int[1];
        newTab[0] = 0;
        negative = false;
        pTab = newTab;
        return *this;
    }

    while (temp > 0) {
        temp /= BASE_NUM;
        length++;
    }

    if (length != size) {
        delete [] pTab;
        size = length == 0 ? 1 : length;
        pTab = new int[size];
    }

    temp = abs;
    for (int i = 0; i < length; i++) {
        pTab[i] = temp % BASE_NUM;
        temp /= BASE_NUM;
    }

    trimTab();
    return *this;
}

// Number &Number::operator=(const Number& other) {
//     if(this != &other) {
//         return *this;
//     }
//     int* newTab = new int[other.size];
//     for(int i = 0; i < other.size; i++) {
//         newTab[i] = other.pTab[i];
//     }
//     delete [] pTab;
//     size = other.size;
//     pTab = newTab;
//     negative = other.negative;
//     return *this;
// }

Number &Number::operator=(Number other) {
    std::swap(size, other.size);
    std::swap(pTab, other.pTab);
    std::swap(negative, other.negative);

    return *this;
}

Number Number::operator+(const Number &other) {
    int length = size > other.size ? size : other.size;
    int carry = 0;
    int num, numOther, sum;
    Number result(length+1);
    result.fillTabWithValue();
    if (negative == other.negative) {
        for (int i = 0; i < result.size; i++) {
            num = i < size ? pTab[i] : 0;
            numOther = i < other.size ? other.pTab[i] : 0;

            sum = num + numOther + carry;

            if (sum >= BASE_NUM) {
                carry = 1;
                sum -= BASE_NUM;
            } else {
                carry = 0;
            }

            result.pTab[i] = sum;
        }
        result.negative = negative;
    } else {
        int borrow = 0;
        const Number& larger = absGreater(other) ? *this : other;
        const Number& smaller = absGreater(other) ? other : *this;

        for (int i = 0; i < length; i++) {
            int num = i < smaller.size ? smaller.pTab[i] : 0;
            int diff = larger.pTab[i] - num - borrow;

            if (diff < 0) {
                diff += BASE_NUM;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.pTab[i] = diff;
        }
        result.negative = larger.negative;
    }
    result.trimTab();
    return result;
}

Number Number::operator-(const Number &other) {
    Number copy(other);
    copy.negative = !copy.negative;
    Number result = (*this) + copy;
    return result;
}

Number Number::operator*(const Number &other) {
    int length = size + other.size;
    int sum;
    Number result(length);
    result.fillTabWithValue();
    for (int i = 0; i < other.size; i++) {
        int carry = 0;
        for (int j = 0; j < size; j++) {
            sum = (other.pTab[i] * pTab[j]) + result.pTab[i+j] + carry;
            result.pTab[i+j] = sum % BASE_NUM;
            carry = sum / BASE_NUM;
        }
        if (carry > 0) {
            result.pTab[i+size] += carry;
        }
    }
    result.negative = negative != other.negative;
    result.trimTab();
    return result;
}

Number Number::operator/(const Number &other) {
    Number result(size);
    result.fillTabWithValue();

    if (other.size == 1 && other.pTab[0] == 0) {
        std::cerr << "Can't divide by zero" << std::endl;
        return Number(0);
    }

    Number absOther(other);
    absOther.negative = false;
    absOther.trimTab();


    Number current(1);
    current.pTab[0] = 0;
    current.negative = false;

    for (int i = size - 1; i >= 0; i--) {
        if (!(current.size == 1 && current.pTab[0] == 0)) {
            int* newTab = new int[current.size + 1];
            newTab[0] = pTab[i];
            for (int j = 0; j < current.size; j++) {
                newTab[j+1] = current.pTab[j];
            }
            delete[] current.pTab;
            current.pTab = newTab;
            current.size += 1;
        } else {
            current.pTab[0] = pTab[i];
        }
        current.trimTab();

        int count = 0;
        while (!absOther.absGreater(current)) {
            current = current - absOther;
            current.trimTab();
            count++;
        }
        result.pTab[i] = count;
    }

    result.negative = negative != other.negative;
    result.trimTab();
    return result;
}

bool Number::operator>(const Number &other) const {
    if (negative != other.negative) {
        return !negative;
    }

    if (negative) {
        return other.absGreater(*this);
    }

    return absGreater(other);
}

bool Number::operator==(const Number &other) const {
    if (negative != other.negative || size != other.size) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        if (pTab[i] != other.pTab[i]) {
            return false;
        }
    }

    return true;
}

Number Number::operator%(const Number &other) const {
    Number result(size);
    result.fillTabWithValue();

    if (other.size == 1 && other.pTab[0] == 0) {
        std::cerr << "Can't divide by zero" << std::endl;
        return Number(0);
    }

    Number absOther(other);
    absOther.negative = false;
    absOther.trimTab();


    Number current(1);
    current.pTab[0] = 0;
    current.negative = false;

    for (int i = size - 1; i >= 0; i--) {
        if (!(current.size == 1 && current.pTab[0] == 0)) {
            int* newTab = new int[current.size + 1];
            newTab[0] = pTab[i];
            for (int j = 0; j < current.size; j++) {
                newTab[j+1] = current.pTab[j];
            }
            delete[] current.pTab;
            current.pTab = newTab;
            current.size += 1;
        } else {
            current.pTab[0] = pTab[i];
        }
        current.trimTab();

        int count = 0;
        while (!absOther.absGreater(current)) {
            current = current - absOther;
            current.trimTab();
            count++;
        }
        result.pTab[i] = count;
    }

    result.negative = negative != other.negative;
    result.trimTab();
    return current;
}

std::string Number::operator()(const int index) const {
    Number modulo(0);
    modulo.fillTabWithValue();

    Number seven(0);
    seven = 7;

    Number copy(0);
    modulo.fillTabWithValue();

    copy = *this;
    copy.negative = false;

    Number condition(0);
    condition = 1;

    std::string r;

    int count = -1;
    while (copy >= condition) {
        modulo = copy % seven;
        copy = copy / seven;
        count++;

        if (count == index) {
            if (negative == true) {
                r += '-';
            }
            r += '0' + modulo.pTab[0];
            return r;
        }
    }

    throw std::out_of_range("Index out of range");
    return "";
}

bool Number::operator>=(const Number& other) const {
    if (*this > other || *this == other) {
        return true;
    }
    return false;
}

std::string Number::toStr() {
    std::string result;
    if (negative) {
        result += '-';
    }
    for (int i = size - 1; i >= 0; i--) {
        result += '0' + pTab[i];
    }
    return result;
}

bool Number::absGreater(const Number &other) const {
    if (size > other.size) {
        return true;
    }
    if (other.size > size) {
        return false;
    }
    for (int i = size - 1; i >= 0; i--) {
        if (pTab[i] != other.pTab[i]) {
            return pTab[i] > other.pTab[i];
        }
    }
    return false;
}

void Number::trimTab() {
    while (size > 1 && pTab[size - 1] == 0) {
        size--;
    }
    if (size == 1 && pTab[0] == 0) {
        negative = false;
    }

    int* newTab = new int[size];
    for (int i = 0; i < size; i++) {
        newTab[i] = pTab[i];
    }

    delete [] pTab;
    pTab = newTab;
}

void Number::fillTabWithValue() {
    for (int i = 0; i < size; i++) {
        pTab[i] = 0;
    }
}

int Number::getSize() const {
    return size;
}


