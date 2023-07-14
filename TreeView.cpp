// TreeView.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

struct A {
    int x, y;
    A() {
        x = 1;
        y = 2;
    }
};

struct B {
    int x, y;
    B() {
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
    B ye;

    Out<A>(oof);
    Out<B>(ye);
}
    