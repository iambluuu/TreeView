#pragma once
#include "TextBox.h"
#include "Drawer.h"

class TextBox;
class Drawer;

class RandomButton : public BaseElement {
	friend class Drawer;
private:
	UIManager* m_owner{ nullptr };
	TextBox* m_textBox{ nullptr };
	TextBox* m_textBox2{ nullptr };

public:
	RandomButton(UIManager* m_owner, TextBox* l_textBox, TextBox* l_textBox2);

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
	void SetRandom();

};
