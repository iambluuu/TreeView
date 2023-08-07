#pragma once

#include "UI_Element.h"
#include "StateManager.h"

enum class StateType;

class GoToButton : public BaseElement {
private:
	UIManager* m_owner;
	sf::Sprite m_illustSprite;
	StateType m_destState;

public:
	GoToButton(UIManager* m_owner, StateType l_type);

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void SetTheme(int l_themeID);
	void SetRandom();
};