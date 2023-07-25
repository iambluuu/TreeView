// TreeView.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "App.h"

int main()
{
    srand(time(0));
    App mainApp;

    while (!mainApp.GetWindow()->IsDone()) {
        mainApp.HandleInput();
        mainApp.Update();
        mainApp.Render();
        mainApp.LateUpdate();
    }
}
    