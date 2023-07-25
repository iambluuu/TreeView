#pragma once
#include "UI_Element.h"


class TextBox : public BaseElement {
private:
	std::string m_string;
	sf::RectangleShape m_caret;
	const int MAX_CHARS = 30;

	bool ValidateInput();

public:
	TextBox(UIManager* l_owner) : BaseElement(l_owner) { 
		m_caret.setFillColor(sf::Color::Black);
		m_caret.setSize(sf::Vector2f(1, 20));
	}

	void Draw();
	void Update();
	void HandleInput(sf::Event* l_event);
	void SetText(const std::string& l_text);
	std::vector<int> ReadNum();
	std::string ReadString();
};