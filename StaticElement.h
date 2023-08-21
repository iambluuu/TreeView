#pragma once
#include "UI_Element.h"
#include "UIManager.h"

class UIManager;

class StaticElement : public BaseElement {
	friend class UIManager;
private:
	UIManager* m_owner;
public:
	StaticElement(UIManager* l_owner, ElementName l_name);

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