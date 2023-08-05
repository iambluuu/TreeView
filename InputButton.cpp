#include "InputButton.h"
#include <iostream>

class UIManager;

InputButton::InputButton(UIManager* l_owner, TextBox* l_textBox, Execute l_execute) {
	m_owner = l_owner;
	m_textBox = l_textBox;
	m_execute = l_execute;

	m_name = ElementName::InputButton;
	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;
}

void InputButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_hitBox.top = m_pos.y;
	m_hitBox.left = m_pos.x;
}

void InputButton::SetRandom() {
	int n = rand() % m_textBox->max_input_char + 1;

	for (int i = 0; i < n; i++) {
		int x = rand() % 99 + 1;
		m_textBox->m_string += std::to_string(x) + ", ";
	}

	m_textBox->m_string.pop_back();
	m_textBox->m_string.pop_back();
}

void InputButton::OnClick() {
	StateType CurState = m_owner->GetState();
	BaseState* state = nullptr;
	state = m_owner->GetStateManager()->GetState(CurState);

	switch (m_execute) {
	case Execute::Insert:
		state->OnInsert(m_textBox->ReadNum());
		break;

	case Execute::Remove:
		state->OnRemove(m_textBox->ReadNum()[0]);
		break;

	default:
		break;
	
	}

}

void InputButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);

	m_state = ElementState::Focused;
}

void InputButton::OnRelease() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void InputButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void InputButton::Update(float l_dT) {
	m_hitBox.left = m_pos.x;
	m_hitBox.top = m_pos.y;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_sprite->setPosition(m_pos);
}

void InputButton::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	if (!m_sprite)
		std::cerr << "oof\n";

	wind->draw(*m_sprite);
}

void InputButton::HandleEvent(sf::Event* l_event) {
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

void InputButton::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}


//void InputButton::AddFunction(StateType l_state, std::function<void()> l_func) {
//	m_func[l_state] = l_func;
//}