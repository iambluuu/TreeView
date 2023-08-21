#pragma once
#include "UI_Element.h"
#include "StateManager.h"

enum class StateType;

class ViewButton : public BaseElement {
private:
	UIManager* m_owner;
	sf::Sprite m_illustSprite;
	StateType m_destState;

public:
	ViewButton(UIManager* m_owner, ElementName l_name);
	~ViewButton();

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void Reset() {}

	void SetTheme(int l_themeID) { m_themeID = l_themeID; }
};