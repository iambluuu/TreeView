#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

class Window {
private:
	int m_fps;

	std::string m_windowTitle;
	sf::Vector2u m_windowSize;
	sf::RenderWindow m_window;
	bool m_isDone;
	bool m_isFullscreen;

	void Destroy();
	void Create();
	void Setup(const std::string& l_title, const sf::Vector2u& l_size);
public:
	Window();
	Window(const std::string& l_title, const sf::Vector2u& l_size);
	~Window();

	void BeginDraw(); //clear the window
	void EndDraw(); //display the changes

	void Update();
	void Draw(sf::Drawable& l_drawable);

	bool IsDone();
	bool IsFullscreen();
	sf::Vector2i GetWindowSize();

	void ToggleFullscreen();
	
	sf::RenderWindow* GetRenderWindow() {
		return &m_window;
	}
};