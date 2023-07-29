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
	for (auto drawer : m_closet) {
		drawer->Clear();
	}

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

	AddToCloset(insertDrawer);
	
	Drawer* removeDrawer = new Drawer(this, "Remove");
	AddToCloset(removeDrawer);


	StaticElement* displayArea = new StaticElement(this, ElementName::DisplayArea);
	displayArea->SetLayer(2);
	displayArea->SetPosition(sf::Vector2f(32, 156));
	AddElement(displayArea);

}

void UIManager::HandleEvent(sf::Event* l_event) {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			if (element->GetState() == ElementState::Hidden || element->GetState() == ElementState::Deactivate)
				continue;

			element->HandleEvent(l_event);
		}
	}

	HandleEventCloset(l_event);
}

void UIManager::HandleEventCloset(sf::Event* l_event) {
	for (auto drawer : m_closet) {
		if (drawer->GetState() == ElementState::Hidden || drawer->GetState() == ElementState::Deactivate)
			continue;

		drawer->HandleEvent(l_event);
	}
}

void UIManager::AddElement(BaseElement* element) {
	m_elements[element->GetLayer()].push_back(element);
}

void UIManager::AddToCloset(Drawer* element) {
	m_closet.push_back(element);
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

	UpdateCloset(l_time);
}

void UIManager::Draw() {
	for (auto layer : m_elements) {
		for (auto element : layer) {
			if (element->GetState() == ElementState::Hidden)
				continue;

			element->Draw();
		}
	}

	DrawCloset();
}

void UIManager::UpdateCloset(const sf::Time& l_time) {
	float time = l_time.asMilliseconds();
	for (auto drawer : m_closet) {
		if (drawer->GetState() == ElementState::Hidden)
			continue;
		drawer->Update(time);
	}
}

void UIManager::DrawCloset() {
	sf::Vector2f pos(1180, 156);
	for (auto drawer : m_closet) {
		drawer->SetPosition(pos);
		drawer->Update(0);
		drawer->Draw();
		pos.y += drawer->GetHeightSpanned();
	}
}

void UIManager::CloseCloset() {
	for (auto drawer : m_closet) {
		drawer->m_isOpened = 0;
	}
}
