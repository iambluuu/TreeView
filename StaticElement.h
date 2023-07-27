#pragma once
#include "UI_Element.h"
#include "UIManager.h"

class UIManager;

class StaticElement : public BaseElement {
private:
	UIManager* m_owner;
public:
	StaticElement(UIManager* l_owner);

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();

	void Update(float l_dT);
	void Draw();

	void SetLayer(int l_layer);
};