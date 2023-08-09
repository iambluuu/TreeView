#include "ViewButton.h"

ViewButton::ViewButton(UIManager* l_owner, ElementName l_name) : m_owner(l_owner) {
	
	m_name = l_name;
	m_sprite = m_owner->GetThemeManager()->GetSprite(m_themeID, m_name, m_state);
	
	switch (l_name) {
	case ElementName::ViewUp:
		SetPosition(sf::Vector2f(82, 646));
		break;
	case ElementName::ViewDown:
		SetPosition(sf::Vector2f(82, 746));
		break;
	
	case ElementName::ViewLeft:
		SetPosition(sf::Vector2f(32, 696));
		break;

	case ElementName::ViewRight:
		SetPosition(sf::Vector2f(132, 696));
		break;
	case ElementName::ResetView:
		SetPosition(sf::Vector2f(82, 696));
		break;
	}

	m_hitBox.width = m_sprite->getTextureRect().width;
	m_hitBox.height = m_sprite->getTextureRect().height;

}

ViewButton::~ViewButton() {

}

void ViewButton::OnClick() {
	if (m_state != ElementState::Clicked) {
		m_state = ElementState::Clicked;
	}

	Window* window = m_owner->GetStateManager()->GetContext()->m_wind;

	switch (m_name) {
	case ElementName::ViewUp:
		window->MoveView(sf::Vector2f(0, -5));
		break;

	case ElementName::ViewDown:
		window->MoveView(sf::Vector2f(0, 5));
		break;

	case ElementName::ViewLeft:
		window->MoveView(sf::Vector2f(-5, 0));
		break;

	case ElementName::ViewRight:
		window->MoveView(sf::Vector2f(5, 0));
		break;

	case ElementName::ResetView:
		window->ResetView();
		break;
	}
}

void ViewButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);
}

void ViewButton::OnRelease() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void ViewButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void ViewButton::HandleEvent(sf::Event* l_event) {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	sf::Vector2i mousePos = sf::Mouse::getPosition(*wind);

	if (m_hitBox.contains(mousePos)) {
		OnHover();

		if (l_event->type == sf::Event::MouseButtonReleased) {
			if (l_event->key.code == sf::Mouse::Left) {
				OnRelease();
			}
		}

		if (l_event->type == sf::Event::MouseButtonPressed) {
			if (l_event->key.code == sf::Mouse::Left)
				OnClick();
		}
	}
	else {
		OnLeave();
	}
}

void ViewButton::Update(float l_dT) {
	if (m_state == ElementState::Clicked) {
		OnClick();
	}
}

void ViewButton::Draw() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	sf::Vector2f offset = m_owner->GetStateManager()->GetContext()->m_wind->GetOffset();

	m_sprite = m_owner->GetThemeManager()->GetSprite(m_themeID, m_name, m_state);
	
	m_sprite->setOrigin(m_sprite->getLocalBounds().left, m_sprite->getLocalBounds().top);
	m_sprite->setPosition(m_pos + offset);

	wind->GetRenderWindow()->draw(*m_sprite);
}

void ViewButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;

	m_hitBox.left = m_pos.x;
	m_hitBox.top = m_pos.y;
}