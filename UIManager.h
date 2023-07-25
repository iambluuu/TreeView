#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "StateManager.h"
#include "UI_Element.h"
#include "TextBox.h"

class BaseElement;

class UIManager {
private:
	std::vector<std::vector<BaseElement*>> m_elements;
	StateManager* m_stateManager;
	sf::Font m_font;
	sf::Cursor DefaultCursor;
	sf::Cursor HandCursor;
	sf::Cursor TextCursor;
	sf::Texture BackgroundTexture1;
	sf::Texture BackgroundTexture2;

	sf::Color DrawerColor = sf::Color(24, 29, 49);
	sf::Color DrawerColorFocused = sf::Color(38, 52, 77);

public:
	UIManager(StateManager* stateManager) {
		m_stateManager = stateManager;

		m_font.loadFromFile("Assets/Font/Roboto-Regular.ttf");
		DefaultCursor.loadFromSystem(sf::Cursor::Arrow);
		HandCursor.loadFromSystem(sf::Cursor::Hand);
		TextCursor.loadFromSystem(sf::Cursor::Text);

		BackgroundTexture1.loadFromFile("Assets/Textures/Background1.png");
	}

	void PrepareElements();

	void HandleInput(sf::Event* l_event);

	sf::Cursor* GetCursor(sf::Cursor::Type);
	void loadUI(StateType l_type);
	void addElement(BaseElement* element);
	void draw();

	StateManager* GetStateManager() { return m_stateManager; }
};