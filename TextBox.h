#pragma once
#include "UI_Element.h"
#include "UIManager.h"

class TextBox : public BaseElement {
private:
	UIManager* m_owner{ nullptr };

	std::string m_string;
	sf::RectangleShape m_caret;
	const int MAX_CHARS = 30;
	const int MAX_CHARS_SHOW = 13;

	bool ValidateInput();

public:
	TextBox(UIManager* m_owner);

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