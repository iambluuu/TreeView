#include "UIManager.h"
//#include "AVL_Tree.h"

UIManager::UIManager(StateManager* stateManager) {
	std::cerr << "UIManager safe\n";

	m_elements.resize(5);
	m_themeManager = new ThemeManager;
	m_stateManager = stateManager;

	PrepareElements();
	PrepareStateUI();
}

UIManager::~UIManager() {
	for (auto drawer : m_closet) {
		drawer->Clear();
	}

	for (auto mediabutton: m_mediaButtons) {
		delete mediabutton;
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

	//Create Drawer for hash table

	TextBox *h_inputNumbers = new TextBox(this, "n =");
	TextBox *h_inputValue = new TextBox(this, "m =");

	RandomButton* h_randomButton = new RandomButton(this, h_inputNumbers, h_inputValue);
	InputButton* h_inputButton = new InputButton(this, h_inputNumbers, h_inputValue, Execute::Create);
	h_inputNumbers->m_inputButton = h_inputButton;

	Drawer * h_drawer = new Drawer(this, "Create");
	h_drawer->AddElement(0, h_inputNumbers);
	h_drawer->AddElement(1, h_inputValue);
	h_drawer->AddElement(1, h_randomButton);
	h_drawer->AddElement(1, h_inputButton);

	AddToCloset(h_drawer);

	//Create Drawer

	TextBox* createValues= new TextBox(this, "v =");
	RandomButton* createRandom = new RandomButton(this, createValues, nullptr);
	InputButton* createInput = new InputButton(this, nullptr, createValues, Execute::Create);
	createValues->m_inputButton = createInput;

	Drawer* createDrawer = new Drawer(this, "Create");
	createDrawer->AddElement(0, createValues);
	createDrawer->AddElement(0, createRandom);
	createDrawer->AddElement(0, createInput);

	AddToCloset(createDrawer);

	//Insert Drawer
	TextBox *insertBox = new TextBox(this, "v =");
	insertBox->max_input_char = 1;
	RandomButton* insertRandom = new RandomButton(this, insertBox, nullptr);
	InputButton* insertInput = new InputButton(this, nullptr, insertBox, Execute::Insert);
	insertBox->m_inputButton = insertInput;
	Drawer* insertDrawer = new Drawer(this, "Insert");

	insertDrawer->AddElement(0, insertBox);
	insertDrawer->AddElement(0, insertRandom);
	insertDrawer->AddElement(0, insertInput);

	AddToCloset(insertDrawer);
	
	//Remove Drawer
	TextBox* removeBox = new TextBox(this, "v =");
	removeBox->max_input_char = 1;
	RandomButton* removeRandom = new RandomButton(this, removeBox, nullptr);
	InputButton* removeInput = new InputButton(this, nullptr, removeBox, Execute::Remove);
	removeBox->m_inputButton = removeInput;
	Drawer* removeDrawer = new Drawer(this, "Remove");

	removeDrawer->AddElement(0, removeBox);
	removeDrawer->AddElement(0, removeRandom);
	removeDrawer->AddElement(0, removeInput);

	AddToCloset(removeDrawer);

	//Search Drawer
	TextBox* searchBox = new TextBox(this, "v =");
	searchBox->max_input_char = 1;
	RandomButton* searchRandom = new RandomButton(this, searchBox, nullptr);
	InputButton* searchInput = new InputButton(this, nullptr, searchBox, Execute::Search);
	searchBox->m_inputButton = searchInput;
	Drawer* searchDrawer = new Drawer(this, "Search");

	searchDrawer->AddElement(0, searchBox);
	searchDrawer->AddElement(0, searchRandom);
	searchDrawer->AddElement(0, searchInput);

	AddToCloset(searchDrawer);

	//Display

	//StaticElement* displayArea = new StaticElement(this, ElementName::DisplayArea);
	//displayArea->SetLayer(2);
	//displayArea->SetPosition(sf::Vector2f(32, 104));
	//AddElement(displayArea);

	PlayBar* playBar = new PlayBar(this);
	playBar->SetPosition(sf::Vector2f(32, 808));
	AddElement(playBar);

	//Media Buttons
	MediaButton* playButton = new MediaButton(this, ElementName::Pause);
	playButton->SetPosition(sf::Vector2f(567, 839));
	MediaButton* forwardButton = new MediaButton(this, ElementName::Forward);
	forwardButton->SetPosition(sf::Vector2f(667, 839));
	MediaButton* backwardButton = new MediaButton(this, ElementName::Backward);
	backwardButton->SetPosition(sf::Vector2f(467, 839));
	MediaButton* skipForwardButton = new MediaButton(this, ElementName::SkipForward);
	skipForwardButton->SetPosition(sf::Vector2f(767, 839));
	MediaButton* skipBackwardButton = new MediaButton(this, ElementName::SkipBackward);
	skipBackwardButton->SetPosition(sf::Vector2f(357, 839));

	AddMediaButton(playButton);
	AddMediaButton(forwardButton);
	AddMediaButton(backwardButton);
	AddMediaButton(skipForwardButton);
	AddMediaButton(skipBackwardButton);

	//View Buttons
	ViewButton* viewUp = new ViewButton(this, ElementName::ViewUp);
	ViewButton* viewDown = new ViewButton(this, ElementName::ViewDown);
	ViewButton* viewLeft = new ViewButton(this, ElementName::ViewLeft);
	ViewButton* viewRight = new ViewButton(this, ElementName::ViewRight);
	ViewButton* resetView = new ViewButton(this, ElementName::ResetView);

	AddElement(viewUp);
	AddElement(viewDown);
	AddElement(viewLeft);
	AddElement(viewRight);
	AddElement(resetView);

	//GoBack Button
	GoBackButton* goBackButton = new GoBackButton(this);

	AddElement(goBackButton);

	//Menu Background
	StaticElement* menuBackground = new StaticElement(this, ElementName::MenuBackground);
	menuBackground->SetLayer(0);

	AddElement(menuBackground);

	//Menu Buttons
	GoToButton* goToAVLTree = new GoToButton(this, StateType::AVLTree);
	goToAVLTree->SetPosition(sf::Vector2f(628, 262));
	AddElement(goToAVLTree);

	GoToButton* goToHashTable = new GoToButton(this, StateType::Hash_Table);
	goToHashTable->SetPosition(sf::Vector2f(97, 262));
	AddElement(goToHashTable);

	GoToButton* goToTTFTree = new GoToButton(this, StateType::TTFTree);
	goToTTFTree->SetPosition(sf::Vector2f(97, 592));
	AddElement(goToTTFTree);
}

void UIManager::PrepareStateUI() {
	UIData data;

	//Closet Mask: 00001 - Create(HashTable), 00010 - Create(Normal), 00100 - Insert, 01000 - Remove, 10000 - Search

	//Menu
	data.closetMask = 0;
	data.isMenu = 1;
	data.tabMask = 0;

	m_uiData.emplace(StateType::Menu, data);

	//AVL Tree
	//TTF Tree
	//Trie
	data.closetMask = 30;
	data.isMenu = 0;
	data.tabMask = 0;

	m_uiData.emplace(StateType::AVLTree, data);
	m_uiData.emplace(StateType::TTFTree, data);
	m_uiData.emplace(StateType::Trie, data);

	//Hash Table
	data.closetMask = 29;
	data.isMenu = 0;
	data.tabMask = 0; //Subject to change, HT has 3 modes

	m_uiData.emplace(StateType::Hash_Table, data);

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
	HandleEventMediaButtons(l_event);
}

void UIManager::HandleEventCloset(sf::Event* l_event) {
	for (auto drawer : m_closet) {
		if (drawer->GetState() == ElementState::Hidden || drawer->GetState() == ElementState::Deactivate)
			continue;

		drawer->HandleEvent(l_event);
	}
}

void UIManager::HandleEventMediaButtons(sf::Event* l_event) {
	for (auto button : m_mediaButtons) {
		if (button->GetState() == ElementState::Hidden || button->GetState() == ElementState::Deactivate)
			continue;

		button->HandleEvent(l_event);
	}
}

void UIManager::AddElement(BaseElement* element) {
	m_elements[element->GetLayer()].push_back(element);
}

void UIManager::AddToCloset(Drawer* element) {
	m_closet.push_back(element);
}

void UIManager::AddMediaButton(MediaButton* element) {
	m_mediaButtons.push_back(element);
}

void UIManager::SwitchState(StateType l_type) {
	std::cerr << "Switching UI to state: " << (int)l_type << std::endl;

	m_uiState = l_type;
	LoadUI(l_type);
}

void UIManager::LoadUI(StateType l_type) {
	//Load tabs and closet using bitmap

	UIData data = m_uiData[l_type];

	for (auto layer : m_elements) {
		for (auto element : layer) {
			if (data.isMenu) {
				if (element->m_name == ElementName::MenuBackground || element->m_name == ElementName::GoTo) 
					element->SetState(ElementState::Neutral);
				else
					element->SetState(ElementState::Hidden);

				continue;
			}

			if (element->m_name == ElementName::MenuBackground || element->m_name == ElementName::GoTo) {
				element->SetState(ElementState::Hidden);
			}
			else {
				element->SetState(ElementState::Neutral);
			}
		}
	}

	for (int i = 0; i < m_closet.size(); i++) {
		if (data.closetMask & (1 << i))
			m_closet[i]->SetState(ElementState::Neutral);
		else
			m_closet[i]->SetState(ElementState::Hidden);
	}

	for (int i = 0; i < m_mediaButtons.size(); i++) {
		if (!data.isMenu)
			m_mediaButtons[i]->SetState(ElementState::Neutral);
		else
			m_mediaButtons[i]->SetState(ElementState::Hidden);
	}
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
	UpdateMediaButtons(l_time);
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
	DrawMediaButtons();
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
		if (drawer->GetState() == ElementState::Hidden)
			continue;

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

void UIManager::UpdateMediaButtons(const sf::Time& l_time) {
	for (auto button : m_mediaButtons) {
		if (button->GetState() == ElementState::Hidden)
			continue;

		button->Update(l_time.asMilliseconds());
	}
}

void UIManager::DrawMediaButtons() {

	for (auto button : m_mediaButtons) {

		if (button->GetState() == ElementState::Hidden)
			continue;

		button->Draw();
	}
}