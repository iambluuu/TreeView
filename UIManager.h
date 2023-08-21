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
#include "GoBackButton.h"
#include "GoToButton.h"
#include "ViewButton.h"
#include "Tab.h"

class BaseElement;

class StateManager;
class ThemeManager;
class MediaButton;
class Tab;

enum class StateType;

struct UIData {
	int closetMask{ 0 };
	int tabMask{ 0 };

	bool isMenu{ false };
};

class UIManager {
private:
	std::map<StateType, UIData> m_uiData;

	std::vector<std::vector<BaseElement*>> m_elements;
	std::vector<Drawer*> m_closet;
	std::vector<Tab*> m_tabs;
	std::vector<MediaButton*> m_mediaButtons;
	//CodeWindow* m_codeWindow;

	StateManager* m_stateManager;
	ThemeManager* m_themeManager;

	sf::Font m_font;
	sf::Text m_stateTitle;

	StateType m_uiState;
	int m_theme{ 0 };

public:
	UIManager(StateManager* stateManager);

	~UIManager();

	void PrepareElements();
	void PrepareStateUI();

	void HandleEvent(sf::Event* l_event);
	void HandleEventCloset(sf::Event* l_event);
	void HandleEventTabs(sf::Event* l_event);
	void HandleEventMediaButtons(sf::Event* l_event);

	sf::Font* GetFont();

	void SwitchState(StateType l_type);
	
	void LoadUI(StateType l_type);
	void LoadTheme(int l_ID);

	void AddElement(BaseElement* element);
	void AddToCloset(Drawer* element);
	void AddMediaButton(MediaButton* element);
	void AddToTabs(Tab* element);

	void Update(const sf::Time& l_time);
	void UpdateCloset(const sf::Time& l_time);
	void UpdateTabs(const sf::Time& l_time);
	void UpdateMediaButtons(const sf::Time& l_time);
	void Draw();
	void DrawCloset();
	void DrawTabs();
	void DrawMediaButtons();
	void CloseCloset();

	StateManager* GetStateManager() { return m_stateManager; }
	ThemeManager* GetThemeManager() { return m_themeManager; }
	StateType GetState() { return m_uiState; }

	int GetThemeID() { return m_theme; }
};