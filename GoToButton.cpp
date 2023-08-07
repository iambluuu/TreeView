#include "GoToButton.h"

GoToButton::GoToButton(UIManager* l_owner, StateType l_type)
{
	m_owner = l_owner;
	m_themeManager = l_owner->GetThemeManager();
	m_destState = l_type;
	m_sprite = l_owner->GetThemeManager()->GetSprite(m_themeID, ElementName::GoTo, ElementState::Neutral);

	m_name = ElementName::GoTo;
	m_layer = 2;

	m_text.setFont(*m_themeManager->GetFont());
	m_text.setCharacterSize(44);
	m_text.setFillColor(*m_themeManager->GetColor(m_themeID, ElementName::TextBox, ElementState::Neutral));
	
	switch (l_type) {
	case StateType::AVLTree:
		m_text.setString("AVL Tree");
		break;

	case StateType::Heap:
		m_text.setString("Heap");
		break;

	case StateType::HashTable:
		m_text.setString("Hash Table");
		break;

	case StateType::Trie:
		m_text.setString("Trie");
		break;

	case StateType::TTFTree:
		m_text.setString("234 Tree");
		break;
	}

	m_text.setOrigin(m_text.getLocalBounds().left, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2.f);
	m_text.setPosition(m_pos.x + m_sprite->getLocalBounds().width / 2, m_pos.y +  m_sprite->getLocalBounds().height / 4 * 3);

	m_hitBox.width = m_sprite->getTextureRect().width;
	m_hitBox.height = m_sprite->getTextureRect().height;

	SetPosition(sf::Vector2f(32, 12));
}

void GoToButton::OnClick() {
	m_owner->GetStateManager()->SwitchTo(m_destState);
}

void GoToButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);

	m_state = ElementState::Focused;
}

void GoToButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);

	m_state = ElementState::Neutral;
}

void GoToButton::OnRelease() {
	m_state = ElementState::Neutral;
}

void GoToButton::Update(float l_dT) {
	m_sprite = m_owner->GetThemeManager()->GetSprite(m_themeID, ElementName::GoTo, m_state);
	SetPosition(m_pos);
}

void GoToButton::HandleEvent(sf::Event* l_event) {
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

void GoToButton::Draw() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->Draw(*m_sprite);
	wind->Draw(m_text);
}

void GoToButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_sprite->setPosition(l_pos);
	m_hitBox.left = l_pos.x;
	m_hitBox.top = l_pos.y;

	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2.f);
	m_text.setPosition(m_pos.x + m_sprite->getLocalBounds().width / 2, m_pos.y + m_sprite->getLocalBounds().height / 6 * 5);
}