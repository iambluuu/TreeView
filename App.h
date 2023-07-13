#pragma once
#include "Window.h"

class App {
private:
	Window* m_window;
	sf::Clock m_clock;
public:
	App();
	~App();

	Window* GetWindow();
};