#pragma once
#include "TextBox.h"
#include "RandomButton.h"
#include "InputButton.h"

class TextBox;
class RandomButton;

class Drawer : public BaseElement {
	friend class UIManager;
private:
	UIManager* m_owner{ nullptr };
	std::vector<std::vector<BaseElement*> > m_elements;
	
	const int ELEMENT_HEIGHT = 56;
	const int VERTICAL_SPACING = 10;
	const int HORIZONTAL_SPACING = 10;
	const float ANIMATION_SPEED = 500.0f;
	sf::Sprite* m_arrow;

	float m_heightSpanned{ 80 };
	bool m_isOpened{ false };
	float m_elapsed{ 0.0f };

	float parametric(float time);

public:
	Drawer(UIManager* l_owner, const std::string& l_title);
	~Drawer();

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void DeactivateLine(int line);
	void ActivateLine(int line);

	void Clear();
	void Reset() {
		m_elapsed = 0;
		m_isOpened = false;
		m_state = ElementState::Neutral;

		for (int i = 0; i < m_elements.size(); ++i) {
			for (int j = 0; j < m_elements[i].size(); ++j) {
				m_elements[i][j]->Reset();
			}
		}

		m_heightSpanned = 80;
	}

	void SetTheme(int l_themeID);
	void AddElement(int level, BaseElement* l_element);
	void SetElementPosition(const float& l_percent);

	float GetHeightSpanned() const;
};