#include "SpeedBar.h"

const float LEFT = 993.f;
const float TOP = 871.f;

SpeedBar::SpeedBar(UIManager* l_owner) {
	m_owner = l_owner;
	m_name = ElementName::SpeedBar;
	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, ElementState::Neutral);
	m_knob = m_themeManager->GetSprite(m_themeID, m_name, ElementState::Focused);

	m_sprite->setOrigin(m_sprite->getLocalBounds().left, m_sprite->getLocalBounds().top + m_sprite->getLocalBounds().height / 2);
	m_knob->setOrigin(m_knob->getLocalBounds().left + m_knob->getLocalBounds().width / 2, m_knob->getLocalBounds().top + m_knob->getLocalBounds().height / 2);

	m_hitBox.height = m_knob->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;

	SetPosition(sf::Vector2f(LEFT, TOP));
}

void SpeedBar::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;

	m_hitBox.top = m_pos.y - m_hitBox.height / 2;
	m_hitBox.left = m_pos.x;
}

void SpeedBar::OnClick() {
	m_state = ElementState::Clicked;
}

void SpeedBar::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);
}

void SpeedBar::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
}

void SpeedBar::OnRelease() {
	NodeRenderer* nodeRenderer = m_owner->GetStateManager()->GetContext()->m_nodeRenderer;

	nodeRenderer->SetSpeedupRate(0.25 + 0.25 * m_level);
	m_state = ElementState::Neutral;
}

void SpeedBar::HandleEvent(sf::Event* l_event) {
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

void SpeedBar::Update(float l_dT) {
	NodeRenderer* nodeRenderer = m_owner->GetStateManager()->GetContext()->m_nodeRenderer;
	sf::Vector2f offset = m_owner->GetStateManager()->GetContext()->m_wind->GetOffset();

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, ElementState::Neutral);
	m_sprite->setOrigin(m_sprite->getLocalBounds().left, m_sprite->getLocalBounds().top + m_sprite->getLocalBounds().height / 2);
	m_knob = m_themeManager->GetSprite(m_themeID, m_name, ElementState::Focused);
	m_knob->setOrigin(m_knob->getLocalBounds().left + m_knob->getLocalBounds().width / 2, m_knob->getLocalBounds().top + m_knob->getLocalBounds().height / 2);

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

		m_level = floor(percent / 0.25f + 0.5f);

		m_sprite->setPosition(m_pos + offset);
		m_knob->setPosition(m_pos.x + m_sprite->getLocalBounds().width * percent + offset.x, m_pos.y + offset.y);
	}
	else {
		m_sprite->setPosition(m_pos + offset);
		m_knob->setPosition(m_pos.x + m_sprite->getLocalBounds().width * 0.25 * m_level + offset.x, m_pos.y + offset.y);
	}
}

void SpeedBar::Draw() {
	Window* window = m_owner->GetStateManager()->GetContext()->m_wind;
	sf::RenderWindow* wind = window->GetRenderWindow();

	wind->draw(*m_sprite);
	wind->draw(*m_knob);
}