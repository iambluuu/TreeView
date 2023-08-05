#include "TextBox.h"
#include <iostream>

class UIManager;

TextBox::TextBox(UIManager* l_owner) {
	m_owner = l_owner;

	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, ElementName::TextBox, ElementState::Neutral);

	m_caret.setFillColor(sf::Color::Black);
	m_caret.setSize(sf::Vector2f(2, 40));
	m_caret.setOrigin(m_caret.getLocalBounds().left + m_caret.getLocalBounds().width / 2.f, m_caret.getLocalBounds().top + m_caret.getLocalBounds().height / 2.f);

	m_text.setFont(*m_themeManager->GetFont());
	m_text.setCharacterSize(30);
	m_text.setString("");
	
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;
}

void TextBox::SetString(const std::string& l_string) {
	m_string = l_string;
}

void TextBox::OnClick() {
	m_state = ElementState::Clicked;
}
 
void TextBox::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(3);
	m_state = ElementState::Focused;
}

void TextBox::OnRelease() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void TextBox::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void TextBox::HandleEvent(sf::Event* l_event) {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	sf::Vector2i mousePos = sf::Mouse::getPosition(*wind);


	if (m_state == ElementState::Clicked) {
		if (!m_hitBox.contains(mousePos) && l_event->type == sf::Event::MouseButtonPressed) {
			OnRelease();
		}


		if (l_event->type == sf::Event::KeyPressed) {
			auto KeyCode = l_event->key.code;

			if (KeyCode == sf::Keyboard::Backspace) {
				if (!m_string.empty()) {
					m_string.pop_back();
				}

				return;
			}

			if (KeyCode == sf::Keyboard::Escape) {
				OnRelease();
				return;
			}

			if (KeyCode == sf::Keyboard::Return) {
				m_inputButton->OnClick();
				OnRelease();
				return;
			}
		}
		else if (l_event->type == sf::Event::TextEntered) {
			auto KeyCode = l_event->text.unicode;

			if (KeyCode == 13) {
				OnRelease();
				return;
			}

			if (KeyCode >= 32 && KeyCode <= 126) {
				m_string += KeyCode;
			}
		}
	}
	else if (m_hitBox.contains(mousePos)) {
			if (l_event->type == sf::Event::MouseButtonReleased) {
				OnClick();
			}
			else {
				OnHover();
			}
	} else {
		OnLeave();
	}
}

void TextBox::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_hitBox.top = l_pos.y;
	m_hitBox.left = l_pos.x;
}

void TextBox::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}

void TextBox::GetShownLength() {
	int limit = m_sprite->getLocalBounds().width - 14;
	int l = 0, r = m_string.size();
	int res = 0;

	while (l <= r) {
		int mid = (l + r) / 2;
		m_text.setString(m_string.substr(m_string.size() - mid, mid));
		if (m_text.getLocalBounds().width <= limit) {
			res = mid;
			l = mid + 1;
		}
		else {
			r = mid - 1;
		}
	}

	m_text.setString(sf::String(m_string.substr(m_string.size() - res, res)));
	m_text.setOrigin(m_text.getLocalBounds().left, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
}

void TextBox::Update(float l_dT) {
	m_hitBox.left = m_pos.x;
	m_hitBox.top = m_pos.y;

	m_text.setPosition(m_pos.x + 7, m_pos.y + m_sprite->getLocalBounds().height / 2);
	m_text.setFillColor(*m_themeManager->GetColor(m_themeID, ElementName::TextBox, m_state));

	m_sprite = m_themeManager->GetSprite(m_themeID, ElementName::TextBox, m_state);
	m_sprite->setPosition(m_pos);

	GetShownLength();
	m_caret.setPosition(m_text.getPosition().x + m_text.getGlobalBounds().width + 2, m_text.getPosition().y);
}

void TextBox::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	if (!m_sprite)
		std::cerr << "oof\n";

	wind->draw(*m_sprite);
	wind->draw(m_text);
	if (m_state == ElementState::Clicked) {
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