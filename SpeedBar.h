#pragma once
#include "UI_Element.h"
#include "UIManager.h"
#include "NodeRenderer.h"

class NodeRenderer;
class UIManager;

class SpeedBar : public BaseElement {
	friend class UIManager;
	friend class NodeRenderer;

private:
	UIManager* m_owner{ nullptr };
	sf::Sprite* m_knob{ nullptr };
	int m_level{ 2 };

public:
	SpeedBar(UIManager* m_owner);
	~SpeedBar();

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
};

