#include "MediaButtons.h"

MediaButton::MediaButton(UIManager* l_owner, ElementName l_name) {
	m_owner = l_owner;
	m_name = l_name;

	m_themeManager = l_owner->GetThemeManager();

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;
}

MediaButton::~MediaButton() {

}

void MediaButton::HandleEvent(sf::Event* l_event) {
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

void MediaButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);
	m_state = ElementState::Focused;
}


void MediaButton::OnClick() {
	NodeRenderer* nodeRenderer = m_owner->GetStateManager()->GetContext()->m_nodeRenderer;

	switch (m_name) {
	case ElementName::Play:
		nodeRenderer->OnPlay();
		break;

	case ElementName::Replay:
		nodeRenderer->OnReplay();
		break;

	case ElementName::Pause:
		nodeRenderer->OnPlay();
		break;
	case ElementName::Forward:
		nodeRenderer->OnForward();
		break;
	case ElementName::Backward:
		nodeRenderer->OnBackward();
		break;

	case ElementName::SkipForward:
		nodeRenderer->OnSkipForward();
		break;

	case ElementName::SkipBackward:
		nodeRenderer->OnSkipBackward();
		break;
	}
}

void MediaButton::OnRelease() {

}

void MediaButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void MediaButton::SetPosition(sf::Vector2f l_pos) {	
	m_pos = l_pos;

	m_hitBox.left = l_pos.x;
	m_hitBox.top = l_pos.y;
}

void MediaButton::Update(float l_dT) {
	NodeRenderer* nodeRenderer = m_owner->GetStateManager()->GetContext()->m_nodeRenderer;

	if (nodeRenderer->GetProgress() == 1 && (m_name == ElementName::Play || m_name == ElementName::Pause || m_name == ElementName::Replay)) {
		m_name = ElementName::Replay;
	}
	else if (nodeRenderer->IsPaused() && (m_name == ElementName::Pause || m_name == ElementName::Replay))
		m_name = ElementName::Play;
	else if (!nodeRenderer->IsPaused() && (m_name == ElementName::Play || m_name == ElementName::Replay))
		m_name = ElementName::Pause;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	SetPosition(m_pos);
}

void MediaButton::Draw() {
	sf::Vector2f offset = m_owner->GetStateManager()->GetContext()->m_wind->GetOffset();
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	m_sprite->setPosition(m_pos + offset);

	wind->draw(*m_sprite);
}

void MediaButton::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}
