#pragma once
#include "UI_Element.h"
#include "UIManager.h"

class UIManager;

class Tab : public BaseElement {
	friend class UIManager;
private:
	StateType m_type;
	UIManager* m_owner;
	int mode;

public:
	bool isActivated{ false };

	Tab(UIManager* l_owner, StateType l_state, int l_mode);

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void Reset() {}

	void SetTheme(int l_themeID);
	void SetLayer(int l_layer);

};