#include "Drawer.h"

Drawer::Drawer(UIManager* l_owner, const std::string& l_title) {
	m_owner = l_owner;
	m_themeManager = l_owner->GetThemeManager();

	m_sprite = m_themeManager->GetSprite(m_themeID, ElementName::Drawer, m_state);

	m_arrow = m_themeManager->GetSprite(m_themeID, ElementName::DrawerArrow, ElementState::Neutral);
	m_arrow->setOrigin(m_arrow->getLocalBounds().left + m_arrow->getLocalBounds().width / 2.f, m_arrow->getLocalBounds().top + m_arrow->getLocalBounds().height / 2.f);
	m_arrow->setPosition(m_pos.x + 30, m_pos.y + m_sprite->getLocalBounds().height / 2);
	
	m_text.setFont(*m_themeManager->GetFont());
	m_text.setCharacterSize(30);
	m_text.setFillColor(*m_themeManager->GetColor(m_themeID, ElementName::TextBox, m_state));
	m_text.setString(l_title);
	m_text.setOrigin(m_text.getLocalBounds().left, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2.f);
	m_text.setPosition(m_pos.x + 30 + m_arrow->getLocalBounds().width, m_pos.y + m_sprite->getLocalBounds().height / 2);

	m_elements.resize(3);


	m_hitBox.width = m_sprite->getLocalBounds().width;
	m_hitBox.height = m_sprite->getLocalBounds().height;
}

Drawer::~Drawer() {
	Clear();
}

void Drawer::HandleEvent(sf::Event* l_event) {
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

	for (auto level : m_elements) {
		for (auto element : level) {
			if (element->GetState() == ElementState::Hidden || element->GetState() == ElementState::Deactivate) {
				continue;
			}

			element->HandleEvent(l_event);
		}
	}
}

void Drawer::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);
	m_state = ElementState::Focused;
}

void Drawer::OnClick() {
	bool tmp = m_isOpened;
	m_owner->CloseCloset();
	m_isOpened = 1 - tmp;
}

void Drawer::OnRelease() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void Drawer::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void Drawer::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;

	m_hitBox.left = m_pos.x;
	m_hitBox.top = m_pos.y;
}

void Drawer::Update(float l_dT) {
	if (!m_isOpened) {
		m_elapsed = std::max(0.0f, m_elapsed - l_dT);
	}
	else {
		m_elapsed = std::min(ANIMATION_SPEED, m_elapsed + l_dT);
	}
	
	float percent = m_elapsed / ANIMATION_SPEED;

	m_sprite = m_themeManager->GetSprite(m_themeID, ElementName::Drawer, m_state);
	m_sprite->setPosition(m_pos);


	m_arrow->setPosition(m_pos.x + 30, m_pos.y + m_sprite->getLocalBounds().height / 2);
	m_text.setPosition(m_pos.x + 30 + m_arrow->getLocalBounds().width, m_pos.y + m_sprite->getLocalBounds().height / 2);

	SetElementPosition(percent);
	m_arrow->setRotation(90 * percent);
}

void Drawer::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	
	for (int i = 0; i < m_elements.size(); i++) {
		for (int j = 0; j < m_elements[i].size(); j++) {
			if (m_elements[i][j]->GetState() == ElementState::Hidden) {
				continue;
			}

			m_elements[i][j]->Update(0);
			m_elements[i][j]->Draw();
		}
	}

	wind->draw(*m_sprite);
	wind->draw(*m_arrow);
	wind->draw(m_text);
}

void Drawer::Clear() {
	for (int i = 0; i < m_elements.size(); i++) {
		for (int j = 0; j < m_elements[i].size(); j++) {
			delete m_elements[i][j];
		}
	}
}

void Drawer::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}

void Drawer::AddElement(int level, BaseElement* l_element) {
	m_elements[level].push_back(l_element);
}

void Drawer::SetElementPosition(const float& l_percent) {
	float level_offset = m_sprite->getLocalBounds().height + VERTICAL_SPACING;

	for (int i = 0; i < m_elements.size(); ++i) {
		if (m_elements[i].size() == 0) {
			continue;
		}

		if (l_percent  == 0) {
			for (int j = 0; j < m_elements[i].size(); ++j) {
				m_elements[i][j]->SetState(ElementState::Hidden);
			}

			continue;
		}
		
		if (l_percent < 1) {
			for (int j = 0; j < m_elements[i].size(); ++j) {
				m_elements[i][j]->SetState(ElementState::Deactivate);
			}
		}

		float CurrentHeight = level_offset * l_percent;
		float HorizontalOffset = HORIZONTAL_SPACING;
		for (int j = 0; j < m_elements[i].size(); ++j) {
			if (l_percent == 1 && m_elements[i][j]->GetState() == ElementState::Deactivate) {
				m_elements[i][j]->SetState(ElementState::Neutral);
			}

			m_elements[i][j]->SetPosition(sf::Vector2f(m_pos.x + HorizontalOffset * l_percent, m_pos.y + CurrentHeight));
			HorizontalOffset += m_elements[i][j]->GetSprite()->getLocalBounds().width + HORIZONTAL_SPACING;
		}

		m_heightSpanned = std::max(m_sprite->getLocalBounds().height + VERTICAL_SPACING, level_offset * l_percent + ELEMENT_HEIGHT + VERTICAL_SPACING);

		level_offset += ELEMENT_HEIGHT + VERTICAL_SPACING;
	}

}

float Drawer::GetHeightSpanned() const {
	return m_heightSpanned;
}