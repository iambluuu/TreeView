#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "StateManager.h"
#include "ThemeManager.h"

#include "StaticElement.h"
#include "TextBox.h"
#include "RandomButton.h"
#include "InputButton.h"
#include "Drawer.h"
#include "PlayBar.h"
#include "MediaButtons.h"

class BaseElement;

class StateManager;
class ThemeManager;
class MediaButton;

enum class StateType;

class UIManager {
private:
	std::vector<std::vector<BaseElement*>> m_elements;
	std::vector<Drawer*> m_closet;
	std::vector<MediaButton*> m_mediaButtons;

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
	void HandleEventCloset(sf::Event* l_event);
	void HandleEventMediaButtons(sf::Event* l_event);

	sf::Font* GetFont();

	void SwitchState(StateType l_type);
	
	void LoadUI(StateType l_type);
	void LoadTheme(int l_ID);

	void AddElement(BaseElement* element);
	void AddToCloset(Drawer* element);
	void AddMediaButton(MediaButton* element);

	void Update(const sf::Time& l_time);
	void UpdateCloset(const sf::Time& l_time);
	void UpdateMediaButtons(const sf::Time& l_time);
	void Draw();
	void DrawCloset();
	void DrawMediaButtons();
	void CloseCloset();

	StateManager* GetStateManager() { return m_stateManager; }
	ThemeManager* GetThemeManager() { return m_themeManager; }
	StateType GetState() { return m_uiState; }
};