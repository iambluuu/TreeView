#include "GoBackButton.h"

GoBackButton::GoBackButton(UIManager* l_owner)
{
	m_owner = l_owner;
	m_sprite = l_owner->GetThemeManager()->GetSprite(m_themeID, ElementName::GoBack, ElementState::Neutral);

	m_hitBox.width = m_sprite->getTextureRect().width;
	m_hitBox.height = m_sprite->getTextureRect().height;

	SetPosition(sf::Vector2f(32, 12));
}

void GoBackButton::OnClick() {
	m_owner->GetStateManager()->SwitchTo(StateType::Menu);
}

void GoBackButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);

	m_state = ElementState::Focused;
}

void GoBackButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);

	m_state = ElementState::Neutral;
}

void GoBackButton::OnRelease() {
	m_state = ElementState::Neutral;
}

void GoBackButton::Update(float l_dT) {
	m_sprite = m_owner->GetThemeManager()->GetSprite(m_themeID, ElementName::GoBack, m_state);
	SetPosition(m_pos);
}

void GoBackButton::HandleEvent(sf::Event* l_event) {
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

void GoBackButton::Draw() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->Draw(*m_sprite);
}

void GoBackButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_sprite->setPosition(l_pos);
	m_hitBox.left = l_pos.x;
	m_hitBox.top = l_pos.y;
}