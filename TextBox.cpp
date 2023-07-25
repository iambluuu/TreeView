#include "TextBox.h"

void TextBox::SetText(const std::string& l_string) {
	m_text.setString(l_string);
}

void TextBox::HandleInput(sf::Event* l_event) {
	sf::Vector2f mousePos = sf::Vector2f(l_event->mouseButton.x, l_event->mouseButton.y);
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	if (m_hitBox.contains(sf::Vector2i(mousePos))) {
		wind->setMouseCursor(*m_owner->GetCursor(sf::Cursor::Type::Text));
		m_state = ElementState::Focused;
	}
	
	if (l_event->type == sf::Event::MouseButtonPressed) {
		if (m_state == ElementState::Clicked && !m_hitBox.contains(sf::Vector2i(mousePos))) {
			m_state = ElementState::Neutral;
		}
		else if (m_state == ElementState::Focused && m_hitBox.contains(sf::Vector2i(mousePos))) {
			m_state = ElementState::Clicked;
		}
	}

	if (m_state == ElementState::Clicked && l_event->type == sf::Event::TextEntered && m_string.size() < MAX_CHARS) {
		char c = l_event->text.unicode;
		m_string += c;
		m_text.setString(m_string);
	}

}

void TextBox::Update() {
	
}

void TextBox::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	wind->draw(m_text);

	if (m_state == ElementState::Clicked) {
		m_caret.setPosition(m_text.getPosition().x + m_text.getGlobalBounds().width + 2, m_text.getPosition().y);
		wind->draw(m_caret);
	}
}

bool TextBox::ValidateInput() {
	for (auto c : m_string) {
		if ((c < '0' || c > '9') && c != ',' && c != ' ') {
			return false;
		}
	}

	return true;
}

std::vector<int> TextBox::ReadNum() {
	std::string soFar = "";
	std::vector<int> ret;

	if (!ValidateInput()) {
		return ret;
	}

	for (int i = 0; i < m_string.size(); i++) {
		if (m_string[i] == ',' || m_string[i] == ' ') {
			if (soFar != "") {
				if (soFar.size() < 3) {
					ret.push_back(std::stoi(soFar));
				}
				soFar = "";
			}
		}
		else {
			soFar += m_string[i];
		}
	}

	if (soFar != "") {
		ret.push_back(std::stoi(soFar));
		soFar = "";
	}

	return ret;
}

std::string TextBox::ReadString() {
	return m_string;
}