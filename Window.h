#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "UIManager.h"
#include "StateManager.h"
#include "Page.h"

#include <Windows.h>
#include <commdlg.h>
#include <shlobj.h>

class Page;
class UIManager;
class StateManager;

class Window {
private:
	const sf::Vector2f DefaultOffset = sf::Vector2f(1500, 0);

	int m_fps;
	int m_cursorType{ 0 }; // 0 - normal, 1 - hand, 2 - text

	sf::Vector2f m_offset;

	std::string m_windowTitle;
	sf::Vector2u m_windowSize;
	sf::RenderWindow m_window;
	bool m_isDone;
	bool m_isFullscreen;
	bool m_isOverlayed{ 0 };

	UIManager* m_uiManager{ nullptr };
	StateManager* m_stateManager{ nullptr };
	Page* m_page{ nullptr };

	sf::View m_view;

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
	void DrawPage();

	void setOverlayed(bool l_overlayed) {
		m_isOverlayed = l_overlayed;
	}

	bool isOverlayed() {
		return m_isOverlayed;
	}

	bool IsDone();
	bool IsFullscreen();
	sf::Vector2i GetWindowSize();

	void ToggleFullscreen();

	void MoveView(sf::Vector2f delta);
	void ResetView();
	
	void SetUIManager(UIManager* l_uiManager) {
		m_uiManager = l_uiManager;
	}

	void SetStateManager(StateManager* l_stateManager) {
		m_stateManager = l_stateManager;
	}

	sf::RenderWindow* GetRenderWindow() {
		return &m_window;
	}

	Page* GetPage();

	void setCursorType(int l_cursorType) {
		m_cursorType = std::max(m_cursorType, l_cursorType);
	}

	sf::Vector2f GetOffset();
};