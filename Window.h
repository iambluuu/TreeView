#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "UIManager.h"
#include "StateManager.h"

class UIManager;
class StateManager;

class Window {
private:
	int m_fps;
	int m_cursorType{ 0 }; // 0 - normal, 1 - hand, 2 - text

	std::string m_windowTitle;
	sf::Vector2u m_windowSize;
	sf::RenderWindow m_window;
	bool m_isDone;
	bool m_isFullscreen;

	UIManager* m_uiManager{ nullptr };
	StateManager* m_stateManager{ nullptr };

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
	void UpdateCursor();
	void Draw(sf::Drawable& l_drawable);

	bool IsDone();
	bool IsFullscreen();
	sf::Vector2i GetWindowSize();

	void ToggleFullscreen();
	
	void SetUIManager(UIManager* l_uiManager) {
		m_uiManager = l_uiManager;
	}

	void SetStateManager(StateManager* l_stateManager) {
		m_stateManager = l_stateManager;
	}

	sf::RenderWindow* GetRenderWindow() {
		return &m_window;
	}

	void setCursorType(int l_cursorType) {
		m_cursorType |= l_cursorType;
	}
};