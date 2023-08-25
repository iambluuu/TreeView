#include "PlayBar.h"

PlayBar::PlayBar(UIManager* l_owner) {
	m_owner = l_owner;
	m_name = ElementName::PlayBar;
	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_playedSprite = m_themeManager->GetSprite(m_themeID, ElementName::PlayBar, ElementState::Focused);

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
	m_state = ElementState::Clicked;
}

void PlayBar::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);
}

void PlayBar::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
}

void PlayBar::OnRelease() {
	NodeRenderer* nodeRenderer = m_owner->GetStateManager()->GetContext()->m_nodeRenderer;
	nodeRenderer->SetProgress(m_percent);
	m_state = ElementState::Neutral;
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
	else if (l_event->type == sf::Event::MouseButtonReleased) {
		if (l_event->mouseButton.button == sf::Mouse::Left) {
			if (m_hitBox.contains(mousePos.x, mousePos.y) || m_state == ElementState::Clicked) {
				OnRelease();
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
	NodeRenderer* nodeRenderer = m_owner->GetStateManager()->GetContext()->m_nodeRenderer;

	if (m_state == ElementState::Clicked) {
		sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
		sf::Vector2i mousePos = sf::Mouse::getPosition(*wind);
		float percent = (mousePos.x - m_pos.x) / (float)m_hitBox.width;

		if (percent < 0) {
			percent = 0;
		}
		else if (percent > 1) {
			percent = 1;
		}

		m_percent = percent;
		nodeRenderer->SetProgress(percent);
	}
	else {
		m_percent = nodeRenderer->GetProgress();
	}

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_playedSprite->setTextureRect(sf::IntRect(0, 867, m_sprite->getLocalBounds().width * m_percent, 20));
}

void PlayBar::Draw() {
	Window* window = m_owner->GetStateManager()->GetContext()->m_wind;
	sf::RenderWindow* wind = window->GetRenderWindow();

	m_sprite->setPosition(m_pos + window->GetOffset());
	m_playedSprite->setPosition(m_pos + window->GetOffset());
	wind->draw(*m_sprite);
	wind->draw(*m_playedSprite);
}