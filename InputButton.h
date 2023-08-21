#pragma once
#include "Drawer.h"

class TextBox;
class Drawer;

enum class Execute {
	Insert = 1, Create, Remove, Search, Clear, Random
};

class InputButton : public BaseElement {
	friend class Drawer;
private:
	UIManager* m_owner{ nullptr };
	TextBox* m_textBox{ nullptr };
	TextBox* m_numberTextbox { nullptr };
	Execute m_execute;

	//std::map<StateType, std::function<void()> > m_func;

public:

	InputButton(UIManager* m_owner, TextBox* l_numberTextbox, TextBox* l_textBox, Execute l_execute);

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

	//void AddFunction(StateType l_state, std::function<void()> l_func);

};
