// TreeView.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

struct B;

struct A {
    int x, y;
    B* objB;

    A() {
        x = 1;
        y = 2;
    }

    void bound(B* l_B) {
        objB = l_B;
    }
};

struct B {
    int x, y;
    A* objA;

    B(A* l_A) {
        objA = l_A;
        x = 3;
        y = 4;
    }
};

template<class T> 
void Out(T obj) {
    std::cout << obj.x << " " << obj.y << "\n";
}

int main()
{
    A oof;
    B ye(&oof);
    oof.bound(&ye);

    oof.x = 69;

    std::cout << oof.objB->x << "\n";
    std::cout << ye.objA->x;
}
    