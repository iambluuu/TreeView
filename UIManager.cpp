#include "UIManager.h"


UIManager::UIManager(StateManager* stateManager) {
	std::cerr << "UIManager safe\n";

	m_elements.resize(5);
	m_themeManager = new ThemeManager();
	m_stateManager = stateManager;
	m_uiState = StateType::AVLTree;

	m_font.loadFromFile("Assets/Font/OpenSans-SemiBold.ttf");
	DefaultCursor.loadFromSystem(sf::Cursor::Arrow);
	HandCursor.loadFromSystem(sf::Cursor::Hand);
	TextCursor.loadFromSystem(sf::Cursor::Text);

	BackgroundTexture1.loadFromFile("Assets/Textures/Background.png");

	PrepareElements();
}

UIManager::~UIManager() {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			delete element;
		}
	}

	delete m_themeManager;
}

void UIManager::PrepareElements() {
	TextBox *box = new TextBox(this);

	AddElement(box);
}

void UIManager::HandleEvent(sf::Event* l_event) {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			element->HandleEvent(l_event);
		}
	}
}

void UIManager::AddElement(BaseElement* element) {
	m_elements[static_cast<int>(element->GetLayer())].push_back(element);
}

void UIManager::LoadTheme(int l_ID) {
	m_theme = l_ID;
}

void UIManager::Update(const sf::Time& l_time) {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			element->Update(l_time.asMilliseconds());
		}
	}
}

void UIManager::Draw() {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			element->Draw();
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

sf::Color* UIManager::GetColor(ElementName l_name, ElementState l_state) {
	return m_themeManager->GetColor(m_theme, l_name, l_state);
}

sf::Sprite* UIManager::GetSprite(ElementName l_name, ElementState l_state) {
	return m_themeManager->GetSprite(m_theme, l_name, l_state);
}

sf::Font* UIManager::GetFont() {
	return &m_font;
}