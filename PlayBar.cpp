#include "PlayBar.h"

PlayBar::PlayBar(UIManager* l_owner) {
	m_owner = l_owner;
	m_name = ElementName::PlayBar;
	m_nodeRenderer = l_owner->GetStateManager()->GetContext()->m_nodeRenderer;
	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;
}

void PlayBar::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;

	m_sprite->setPosition(m_pos);
	m_playedSprite->setPosition(m_pos);

	m_hitBox.top = m_pos.y;
	m_hitBox.left = m_pos.x;
}

void PlayBar::OnClick() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	sf::Vector2i mousePos = sf::Mouse::getPosition(*wind);
	float percent = (mousePos.x - m_pos.x) / (float)m_hitBox.width;

	m_nodeRenderer->SetProgress(percent);
}

void PlayBar::OnHover() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	wind->setMouseCursor(*m_themeManager->GetCursor(sf::Cursor::Hand));
}

void PlayBar::OnLeave() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	wind->setMouseCursor(*m_themeManager->GetCursor(sf::Cursor::Hand));
}

void PlayBar::OnRelease() {

}

void PlayBar::HandleEvent(sf::Event* l_event) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow());

	if (l_event->type == sf::Event::MouseButtonPressed) {
		if (l_event->mouseButton.button == sf::Mouse::Left) {
			if (m_hitBox.contains(mousePos.x, mousePos.y)) {
				OnClick();
			}
		}
	}
	else if (l_event->type == sf::Event::MouseMoved) {
		if (m_hitBox.contains(mousePos.x, mousePos.y)) {
			OnHover();
		}
		else {
			OnLeave();
		}
	}
}

void PlayBar::Update(float l_dT) {
	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_playedSprite = m_themeManager->GetSprite(m_themeID, ElementName::PlayBarPlayed, m_state);
	m_playedSprite->setScale(sf::Vector2f(1, m_nodeRenderer->GetProgress()));
}

void PlayBar::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	wind->draw(*m_sprite);
	wind->draw(*m_playedSprite);
}