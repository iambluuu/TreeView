#include "UIManager.h"

void UIManager::PrepareElements() {
	
}

void UIManager::HandleInput(sf::Event* l_event) {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			element->HandleInput(l_event);
		}
	}
}



void UIManager::draw() {
	sf::RenderWindow* wind = m_stateManager->GetContext()->m_wind->GetRenderWindow();
	
	for (auto layer : m_elements) {
		for (auto element : layer) {
			element->Draw(wind);
		}
	}
}

sf::Cursor* UIManager::GetCursor(sf::Cursor::Type l_type) {
	switch (l_type) {;
		case sf::Cursor::Type::Hand:
			return &HandCursor;
		case sf::Cursor::Type::Text:
			return &TextCursor;
		case sf::Cursor::Type::Arrow:
		default:
			return &DefaultCursor;
	}
}