#include "MatrixButton.h"
#include <iostream>

class UIManager;

MatrixButton::MatrixButton(UIManager* l_owner) {
	m_owner = l_owner;

	m_name = ElementName::MatrixButton;
	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;
}

void MatrixButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_hitBox.top = m_pos.y;
	m_hitBox.left = m_pos.x;
}

void MatrixButton::OnClick() {
	StateType CurState = m_owner->GetState();
	BaseState* state = nullptr;
	state = m_owner->GetStateManager()->GetState(CurState);

	Page* page = m_owner->GetStateManager()->GetContext()->m_wind->GetPage();
	page->Open();
	
}

void MatrixButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);

	m_state = ElementState::Focused;
}

void MatrixButton::OnRelease() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void MatrixButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void MatrixButton::Update(float l_dT) {
	sf::Vector2f offset = m_owner->GetStateManager()->GetContext()->m_wind->GetOffset();

	m_hitBox.left = m_pos.x;
	m_hitBox.top = m_pos.y;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_sprite->setPosition(m_pos + offset);
}

void MatrixButton::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	if (!m_sprite)
		std::cerr << "oof\n";

	wind->draw(*m_sprite);
}

void MatrixButton::HandleEvent(sf::Event* l_event) {
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

void MatrixButton::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}