#pragma once
#include "Drawer.h"

#include <fstream>
#include <sstream>

class TextBox;
class Drawer;

class MatrixButton : public BaseElement {
	friend class Drawer;
private:
	UIManager* m_owner{ nullptr };
	TextBox* m_textBox{ nullptr };
	TextBox* m_numberTextbox{ nullptr };

	//std::map<StateType, std::function<void()> > m_func;

public:

	MatrixButton(UIManager* l_owner);

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
