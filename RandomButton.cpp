#include "RandomButton.h"
#include <iostream>

class UIManager;


RandomButton::RandomButton(UIManager* l_owner, TextBox* l_textBox) {
	m_owner = l_owner;
	m_textBox = l_textBox;

	m_name = ElementName::RandomButton;
	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;
}

void RandomButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_hitBox.top = m_pos.y;
	m_hitBox.left = m_pos.x;
}

void RandomButton::SetRandom() {
	int n = rand() % m_textBox->max_input_char + 1;

	for (int i = 0; i < n; i++) {
		int x = rand() % 99 + 1;
		m_textBox->m_string += std::to_string(x) + ", ";
	}

	m_textBox->m_string.pop_back();
	m_textBox->m_string.pop_back();
}

void RandomButton::OnClick() {
	m_textBox->m_string.clear();
	SetRandom();
}

void RandomButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Focused;
}

void RandomButton::OnRelease() {
	m_state = ElementState::Neutral;
}

void RandomButton::OnLeave() {
	m_state = ElementState::Neutral;
}

void RandomButton::Update(float l_dT) {
	m_hitBox.left = m_pos.x;
	m_hitBox.top = m_pos.y;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_sprite->setPosition(m_pos);
}

void RandomButton::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	if (!m_sprite)
		std::cerr << "oof\n";

	wind->draw(*m_sprite);
}

void RandomButton::HandleEvent(sf::Event* l_event) {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	sf::Vector2i mousePos = sf::Mouse::getPosition(*wind);

	if (m_hitBox.contains(mousePos)) {
		OnHover();
		if (l_event->type == sf::Event::MouseButtonReleased)
			OnClick();
	}
	else {
		OnLeave();
	}
}

void RandomButton::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}