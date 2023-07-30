#include "UIManager.h"
#include "AVL_Tree.h"

UIManager::UIManager(StateManager* stateManager) {
	std::cerr << "UIManager safe\n";

	m_elements.resize(5);
	m_themeManager = new ThemeManager;
	m_stateManager = stateManager;
	m_uiState = StateType::AVLTree;

	PrepareElements();

	SwitchState(StateType::AVLTree);
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

	//Insert Drawer
	TextBox *insertBox = new TextBox(this);
	insertBox->max_input_char = 1;
	RandomButton* insertRandom = new RandomButton(this, insertBox);
	InputButton* insertInput = new InputButton(this, insertBox, Execute::Insert);

	Drawer* insertDrawer = new Drawer(this, "Insert");

	insertDrawer->AddElement(0, insertBox);
	insertDrawer->AddElement(0, insertRandom);
	insertDrawer->AddElement(0, insertInput);

	AddToCloset(insertDrawer);
	
	//Remove Drawer
	TextBox* removeBox = new TextBox(this);
	removeBox->max_input_char = 1;
	RandomButton* removeRandom = new RandomButton(this, removeBox);
	InputButton* removeInput = new InputButton(this, removeBox, Execute::Remove);
	Drawer* removeDrawer = new Drawer(this, "Remove");

	removeDrawer->AddElement(0, removeBox);
	removeDrawer->AddElement(0, removeRandom);
	removeDrawer->AddElement(0, removeInput);

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

void UIManager::SwitchState(StateType l_type) {
	m_uiState = l_type;
	LoadUI(l_type);
}

void UIManager::LoadUI(StateType l_type) {
	//Load tabs and closet using bitmap
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
