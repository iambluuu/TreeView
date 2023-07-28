#include "UIManager.h"

UIManager::UIManager(StateManager* stateManager) {
	std::cerr << "UIManager safe\n";

	m_elements.resize(5);
	m_themeManager = new ThemeManager;
	m_stateManager = stateManager;
	m_uiState = StateType::AVLTree;

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
	StaticElement* background = new StaticElement(this, ElementName::Background);
	std::cerr << "background created\n";
	AddElement(background);

	TextBox *box = new TextBox(this);
	RandomButton* randomButton = new RandomButton(this, box);
	Drawer* insertDrawer = new Drawer(this, "Insert");

	insertDrawer->AddElement(0, box);
	insertDrawer->AddElement(0, randomButton);

	AddElement(insertDrawer);
	

	StaticElement* displayArea = new StaticElement(this, ElementName::DisplayArea);
	displayArea->SetLayer(2);
	displayArea->SetPosition(sf::Vector2f(50, 100));
	AddElement(displayArea);

	std::cerr << "box created\n";


	std::cerr << "Elements prepared\n";
}

void UIManager::HandleEvent(sf::Event* l_event) {
	sf::RenderWindow* wind = m_stateManager->GetContext()->m_wind->GetRenderWindow();
	for (auto layer : m_elements) {
		for (auto element : layer) {
			if (element->GetState() == ElementState::Hidden || element->GetState() == ElementState::Deactivate)
				continue;

			element->HandleEvent(l_event);
		}
	}
}

void UIManager::AddElement(BaseElement* element) {
	m_elements[element->GetLayer()].push_back(element);
}

void UIManager::LoadTheme(int l_ID) {
	m_theme = l_ID;
}

void UIManager::Update(const sf::Time& l_time) {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			if (element->GetState() == ElementState::Hidden)
				continue;
			element->Update(l_time.asMilliseconds());
		}
	}
}

void UIManager::Draw() {
	sf::RenderWindow* wind = m_stateManager->GetContext()->m_wind->GetRenderWindow();
	for (auto layer : m_elements) {
		for (auto element : layer) {
			if (element->GetState() == ElementState::Hidden)
				continue;

			element->Draw();
		}
	}
}
