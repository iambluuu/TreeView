#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "StateManager.h"
#include "ThemeManager.h"

#include "StaticElement.h"
#include "TextBox.h"
#include "RandomButton.h"
#include "Drawer.h"

class BaseElement;

class StateManager;
class ThemeManager;

enum class StateType;

class UIManager {
private:
	std::vector<std::vector<BaseElement*>> m_elements;

	StateManager* m_stateManager;
	ThemeManager* m_themeManager;

	sf::Font m_font;

	StateType m_uiState;
	int m_theme{ 0 };

public:
	UIManager(StateManager* stateManager);

	~UIManager();

	void PrepareElements();

	void HandleEvent(sf::Event* l_event);

	sf::Font* GetFont();

	void loadUI(StateType l_type);
	void LoadTheme(int l_ID);

	void AddElement(BaseElement* element);
	void AddToToolbar(BaseElement* element);

	void Update(const sf::Time& l_time);
	void Draw();
	void DrawToolbar();

	StateManager* GetStateManager() { return m_stateManager; }
	ThemeManager* GetThemeManager() { return m_themeManager; }
	StateType GetState() { return m_uiState; }
};