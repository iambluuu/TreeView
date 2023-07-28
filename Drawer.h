#pragma once
#include "TextBox.h"
#include "RandomButton.h"

class TextBox;
class RandomButton;

class Drawer : public BaseElement {
private:
	UIManager* m_owner{ nullptr };
	std::vector<std::vector<BaseElement*> > m_elements;
	
	const int ELEMENT_HEIGHT = 56;
	const int VERTICAL_SPACING = 10;
	const int HORIZONTAL_SPACING = 10;
	const float ANIMATION_SPEED = 500.0f;
	sf::Sprite* m_arrow;

	bool m_isOpened{ false };
	float m_elapsed{ 0.0f };

public:
	Drawer(UIManager* l_owner, const std::string& l_title);

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void SetTheme(int l_themeID);
	void AddElement(int level, BaseElement* l_element);
	void SetElementPosition(const float& l_percent);

};