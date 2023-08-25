#include "Tab.h"

Tab::Tab(UIManager* l_owner, StateType l_state, int l_mode) {
	m_owner = l_owner;
	m_type = l_state;
	m_name = ElementName::Tab;

	mode = l_mode;

	switch (mode) {
	case 0:
		if (m_type == StateType::Hash_Table)
			m_text.setString("Chaining");
		else if (m_type == StateType::Heap)
			m_text.setString("Max Heap");

		break;

	case 1:
		if (m_type == StateType::Hash_Table)
			m_text.setString("LP");
		else if (m_type == StateType::Heap)
			m_text.setString("Min Heap");
		break;

	case 2:
		if (m_type == StateType::Hash_Table)
			m_text.setString("QP");
		break;
	}

    m_text.setFont(*m_owner->GetThemeManager()->GetFont());
	m_text.setCharacterSize(32);

	if (m_themeID == 0)
		m_text.setFillColor(sf::Color::Black);
	else
		m_text.setFillColor(sf::Color::White);

	m_sprite = m_owner->GetThemeManager()->GetSprite(m_themeID, ElementName::Tab, ElementState::Neutral);
	m_hitBox.width = m_sprite->getTextureRect().width - 128;
	m_hitBox.height = m_sprite->getTextureRect().height;
}

void Tab::HandleEvent(sf::Event* l_event) {
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

void Tab::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);
}

void Tab::OnClick() {
	auto State = m_owner->GetStateManager()->GetState(m_type);

	State->SwitchMode(mode);
	m_state = ElementState::Focused;
	isActivated = true;
}

void Tab::OnRelease() {

}

void Tab::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
}

void Tab::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;

	m_hitBox.left = m_pos.x + 64;
	m_hitBox.top = m_pos.y;	
}

void Tab::Update(float l_dT) {
	m_sprite = m_owner->GetThemeManager()->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.left = m_pos.x + 64;
	m_hitBox.top = m_pos.y;
}

void Tab::Draw() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	sf::Vector2f offset = wind->GetOffset();

	m_sprite->setPosition(m_pos.x + offset.x, m_pos.y + offset.y);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.f, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2.f);
	m_text.setPosition(m_pos.x + m_sprite->getLocalBounds().width / 2 + offset.x, m_pos.y + m_sprite->getLocalBounds().height / 2.f + offset.y);

	wind->GetRenderWindow()->draw(*m_sprite);
	wind->GetRenderWindow()->draw(m_text);
}

void Tab::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}

void Tab::SetLayer(int l_layer) {
	m_layer = l_layer;
}