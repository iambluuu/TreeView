#pragma once
#include "UI_Element.h"
#include "UIManager.h"

class UIManager;

class TextBox : public BaseElement {
private:
	std::string m_string;
	sf::RectangleShape m_caret;
	const int MAX_CHARS = 30;
	const int MAX_CHARS_SHOW = 13;

	bool ValidateInput();

public:
	TextBox(UIManager* l_owner);

	void HandleEvent(sf::Event* l_event);

	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void Update(float l_dT);
	void Draw();

	void SetText(const std::string& l_text);
	std::vector<int> ReadNum();
	std::string ReadString();
};