#include "BrowseButton.h"
#include <iostream>

HWND hwnd;
TCHAR szFileName[MAX_PATH];

class UIManager;

BrowseButton::BrowseButton(UIManager* l_owner) {
	m_owner = l_owner;

	m_name = ElementName::BrowseButton;
	m_themeManager = l_owner->GetThemeManager();
	m_layer = 1;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_hitBox.height = m_sprite->getLocalBounds().height;
	m_hitBox.width = m_sprite->getLocalBounds().width;
}

void BrowseButton::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_hitBox.top = m_pos.y;
	m_hitBox.left = m_pos.x;
}

void BrowseButton::OnClick() {
	StateType CurState = m_owner->GetState();
	BaseState* state = nullptr;
	state = m_owner->GetStateManager()->GetState(CurState);

	std::string input = BrowseString();

	state->OnCreate(input);
}

void BrowseButton::OnHover() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(2);

	m_state = ElementState::Focused;
}

void BrowseButton::OnRelease() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void BrowseButton::OnLeave() {
	Window* wind = m_owner->GetStateManager()->GetContext()->m_wind;
	wind->setCursorType(1);
	m_state = ElementState::Neutral;
}

void BrowseButton::Update(float l_dT) {
	sf::Vector2f offset = m_owner->GetStateManager()->GetContext()->m_wind->GetOffset();

	m_hitBox.left = m_pos.x;
	m_hitBox.top = m_pos.y;

	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, m_state);
	m_sprite->setPosition(m_pos + offset);
}

void BrowseButton::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();

	if (!m_sprite)
		std::cerr << "oof\n";

	wind->draw(*m_sprite);
}

void BrowseButton::HandleEvent(sf::Event* l_event) {
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

void BrowseButton::SetTheme(int l_themeID) {
	m_themeID = l_themeID;
}


std::string BrowseButton::BrowseString() {

	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
	ofn.lpstrDefExt = L"txt";

	std::string s;

	if (GetOpenFileName(&ofn) == TRUE) {
		// User selected a file
		std::ifstream file(ofn.lpstrFile);
		std::string line;
		while (std::getline(file, line)) {
			s.append(line);
		}

		file.close();
	}
	else {
		// User cancelled the dialog
	}

	std::cerr << "String from browse: " << s << std::endl;

	return s;

}

//void BrowseButton::AddFunction(StateType l_state, std::function<void()> l_func) {
//	m_func[l_state] = l_func;
//}