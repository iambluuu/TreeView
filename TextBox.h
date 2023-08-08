#pragma once
#include "UI_Element.h"
#include "UIManager.h"
#include "RandomButton.h"
#include "InputButton.h"
#include "Drawer.h"

class Drawer;
class RandomButton;
class InputButton;

class TextBox : public BaseElement {
	friend class UIManager;
	friend class InputButton;
	friend class RandomButton;
	friend class Drawer;
private:
	UIManager* m_owner{ nullptr };
	InputButton* m_inputButton{ nullptr };

	std::string m_string;
	std::string m_defaultString;
	sf::RectangleShape m_caret;
	const int MAX_CHARS = 30;
	int max_input_char = 15;

	bool ValidateInput();

public:
	TextBox(UIManager* m_owner, const std::string& l_defaultString);

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void SetTheme(int l_themeID);
	void SetString(const std::string& l_text);
	void GetShownLength();


	std::vector<int> ReadNum();
	std::string ReadString();
};