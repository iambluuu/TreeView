#pragma once
#include "ThemeManager.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

enum class ElementName {
	DrawerButton = 1, DrawerArrow, RandomButton, InputButton, Drawer, TextBox, Background, DisplayArea, ActiveTab, InactiveTab,
	PlayBar,

	BackgroundColor,

	Play,
	Replay,
	Pause,
	Forward,
	SkipForward,
	Backward,
	SkipBackward,

	GoBack,
	GoTo,

	MenuBackground,

	ViewUp,
	ViewDown,
	ViewLeft,
	ViewRight,
	ResetView
};

enum class ElementState {
	Neutral = 1, Focused, Clicked, Hidden, Deactivate
};;


class ThemeManager;

class BaseElement {
	friend class UIManager;
protected:
	sf::Sprite* m_sprite{ nullptr };
	sf::Text m_text;
	ElementState m_state{ElementState::Neutral};
	ElementName m_name{ElementName::Background};

	int m_layer{ 0 };
	int m_themeID{ 0 };
	sf::IntRect m_hitBox;
	sf::Vector2f m_pos {0.f, 0.f};
	
	ThemeManager* m_themeManager{ nullptr };
	//UIManager* m_owner{ nullptr };
	
	//std::unordered_map<StateType, std::function<void()>> m_func;

public:
	BaseElement() {}
	BaseElement(ThemeManager* l_themeManager) { m_themeManager = l_themeManager; }

	virtual void HandleEvent(sf::Event* l_event) = 0;

	virtual void OnHover() = 0;
	virtual void OnClick() = 0;
	virtual void OnRelease() = 0;
	virtual void OnLeave() = 0;
	virtual void Update(float l_dT) = 0;
	virtual void Draw() = 0;
	virtual void SetPosition(sf::Vector2f l_pos) {
		m_pos = l_pos;
		m_hitBox.left = l_pos.x;
		m_hitBox.top = l_pos.y;
	}
	virtual void SetState(ElementState l_state) { m_state = l_state; }
	sf::Sprite* GetSprite() { return m_sprite; }
	ElementState GetState() { return m_state; }
	int GetLayer() { return m_layer; }
	void SetTheme(int l_themeID) { m_themeID = l_themeID; }
};
