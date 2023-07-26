#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "StateManager.h"
#include "ThemeManager.h"
#include "UI_Element.h"
#include "TextBox.h"

class BaseElement;
class StateManager;
class ThemeManager;

enum class ElementName{
	DrawerButton = 1, RandomButton, InputButton, TextBox
};

enum class StateType;

class UIManager {
private:
	std::vector<std::vector<BaseElement*>> m_elements;
	std::vector<std::vector<BaseElement*>> m_toolBar;

	StateManager* m_stateManager;
	ThemeManager* m_themeManager;

	sf::Font m_font;
	sf::Cursor DefaultCursor;
	sf::Cursor HandCursor;
	sf::Cursor TextCursor;
	sf::Texture BackgroundTexture1;
	sf::Texture BackgroundTexture2;


	sf::Color DrawerColor = sf::Color(24, 29, 49);
	sf::Color DrawerColorFocused = sf::Color(38, 52, 77);
	sf::Color TextBoxColor = sf::Color(53, 66, 89);
	sf::Color TextBoxColorFocused = sf::Color(240, 233, 210);
	sf::Color CaretColor = sf::Color(24, 29, 49);


	StateType m_uiState;
	int m_theme{ 0 };

public:
	UIManager(StateManager* stateManager);

	~UIManager();

	void PrepareElements();

	void HandleEvent(sf::Event* l_event);

	sf::Font* GetFont();
	sf::Color* GetColor(ElementName l_name, ElementState l_state);
	sf::Sprite* GetSprite(ElementName l_name, ElementState l_state);
	sf::Cursor* GetCursor(sf::Cursor::Type);
	void loadUI(StateType l_type);
	void LoadTheme(int l_ID);
	void AddElement(BaseElement* element);
	void AddToToolbar(BaseElement* element);

	void Update(const sf::Time& l_time);
	void Draw();
	void DrawToolbar();

	StateManager* GetStateManager() { return m_stateManager; }
	StateType GetState() { return m_uiState; }
};