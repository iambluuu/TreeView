#pragma once
#include "UI_Element.h"
#include "UIManager.h"
#include "NodeRenderer.h"

class UIManager;
class NodeRenderer;

class ThemeButton : public BaseElement
{
	friend class UIManager;
	friend class NodeRenderer;
private:
	UIManager* m_owner;
	ElementName m_name;
public:
	ThemeButton(UIManager* m_owner);
	~ThemeButton();

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void Reset() {}

	void SetState(ElementState l_state) { m_state = l_state; }
	void SetTheme(int l_themeID);

};