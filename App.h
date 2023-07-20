#pragma once
#include "Window.h"
#include "StateManager.h"

class App {
private:
	StateManager m_stateManager;
	TextureManager m_textureManager;
	NodeRenderer m_nodeRenderer;
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
		m_elapsed += m_clock.restart();
	}

	void HandleInput();
	void Update();
	void Render();
	void LateUpdate();
	Window* GetWindow() { return &m_window; }


};