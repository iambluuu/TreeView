#include "ThemeButton.h"

ThemeButton::ThemeButton(UIManager* l_owner) {
	m_owner = l_owner;
	m_name = ElementName::ThemeButton;

	m_themeManager = l_owner->GetThemeManager();

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;

	SetPosition(sf::Vector2f(32, 839));
}

ThemeButton::~ThemeButton() {

}

void ThemeButton::HandleEvent(sf::Event* l_event) {
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

void ThemeButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);
	m_state = ElementState::Focused;
}


void ThemeButton::OnClick() {
	m_owner->LoadTheme(1 - m_themeID);
}

void ThemeButton::OnRelease() {

}

void ThemeButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void ThemeButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;

	m_hitBox.left = l_pos.x;
	m_hitBox.top = l_pos.y;
}

void ThemeButton::Update(float l_dT) {
	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
}

void ThemeButton::Draw() {
	sf::Vector2f offset = m_owner->GetStateManager()->GetContext()->m_wind->GetOffset();
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	m_sprite->setPosition(m_pos + offset);

	wind->draw(*m_sprite);
}

void ThemeButton::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}
