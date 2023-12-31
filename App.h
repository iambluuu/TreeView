#pragma once
#include "Window.h"
#include "StateManager.h"
#include "UIManager.h"

class App {
private:
	StateManager m_stateManager;
	NodeRenderer m_nodeRenderer;
	UIManager m_uiManager;
	Window m_window;

	SharedContext m_context;
	
	sf::Clock m_clock;
	sf::Time m_elapsed;
public:
	App();
	~App();

	sf::Time GetTime() {
		return m_elapsed;
	}

	void RestartClock() {
		m_elapsed = m_clock.restart();
	}

	void HandleInput();
	void Update();
	void Render();
	void LateUpdate();
	Window* GetWindow() { return &m_window; }


};