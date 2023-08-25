#include "ThemeManager.h"
#include "UI_Element.h"

#include <utility>

const sf::Color Navy = sf::Color(24, 29, 49, 255);
const sf::Color DarkBlue = sf::Color(45, 55, 73);
const sf::Color LightBlue = sf::Color(53, 66, 89);
const sf::Color LightBeige = sf::Color(240, 233, 210);	

ThemeManager::ThemeManager() {
	DefaultCursor.loadFromSystem(sf::Cursor::Arrow);
	HandCursor.loadFromSystem(sf::Cursor::Hand);
	TextCursor.loadFromSystem(sf::Cursor::Text);

	m_font.loadFromFile("Assets/Font/monofonto rg.otf");

	m_themeColor.resize(2);
	m_themeSprite.resize(2);

	BG1.loadFromFile("Assets/Texture/Background.png");
	BG2.loadFromFile("Assets/Texture/Background-2.png");

	MenuBG.loadFromFile("Assets/Texture/MenuBackground.png");

	MenuWidget.loadFromFile("Assets/Texture/MenuWidget.png");

	Elements.loadFromFile("Assets/Texture/Elements.png");
	Elements.setSmooth(1);

	Elements2.loadFromFile("Assets/Texture/Elements-2.png");
	Elements2.setSmooth(1);

	DisplayArea1.loadFromFile("Assets/Texture/DisplayArea.png");
	DisplayArea1.setSmooth(1);

	DisplayArea2.loadFromFile("Assets/Texture/DisplayArea-2.png");
	DisplayArea2.setSmooth(1);

	Initiate();
	std::cerr << "ThemeManager Initiated\n";
}

sf::Color* ThemeManager::GetColor(int ThemeID, ElementName l_name, ElementState l_state) {
	ThemeColor* theme = &m_themeColor;
	return &theme->at(ThemeID)[{l_name, l_state}];
}

sf::Sprite* ThemeManager::GetSprite(int ThemeID, ElementName l_name, ElementState l_state) {
	ThemeSprite* theme = &m_themeSprite;

	if (theme->at(ThemeID).find({l_name, l_state}) == theme->at(ThemeID).end()) {
		std::cerr << "Did not find sprite\n";
		//return nullptr;
	}

	return &theme->at(ThemeID)[{l_name, l_state}];
}

void ThemeManager::Initiate()
{
	//First Theme

	//Sprite
	auto ThemeSprite1 = &m_themeSprite[0];
	sf::Sprite tmpSprite;

	//TextBox Sprite
	tmpSprite.setTexture(Elements);
	tmpSprite.setTextureRect(sf::IntRect(0, 164, 234, 56));
	ThemeSprite1->emplace(std::pair{ElementName::TextBox, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ElementName::TextBox, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(234, 164, 234, 56));
	ThemeSprite1->emplace(std::pair{ElementName::TextBox, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ElementName::TextBox, ElementState::Clicked }, tmpSprite);

	//TextBox text color
	auto ThemeColor1 = &m_themeColor[0];
	ThemeColor1->emplace(std::pair{ElementName::TextBox, ElementState::Neutral }, LightBeige);
	ThemeColor1->emplace(std::pair{ElementName::TextBox, ElementState::Deactivate }, LightBeige);
	ThemeColor1->emplace(std::pair{ElementName::TextBox, ElementState::Focused }, DarkBlue);
	ThemeColor1->emplace(std::pair{ElementName::TextBox, ElementState::Clicked }, DarkBlue);

	//BrowseButton Sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ElementName::BrowseButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ElementName::BrowseButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(142, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ElementName::BrowseButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ElementName::BrowseButton, ElementState::Clicked }, tmpSprite);

	//Matrix Sprite

	tmpSprite.setTextureRect(sf::IntRect(284, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(426, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Clicked }, tmpSprite);

	//Randombutton Sprite
	tmpSprite.setTextureRect(sf::IntRect(0, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(104, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Clicked }, tmpSprite);

	//InputButton Sprite
	tmpSprite.setTextureRect(sf::IntRect(52, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(156, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Clicked }, tmpSprite);

	//Drawer sprite
	tmpSprite.setTextureRect(sf::IntRect(0, 220, 226, 70));
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Deactivate }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(226, 220, 226, 70));
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Clicked }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(0, 113, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::DrawerArrow, ElementState::Neutral }, tmpSprite);

	//Background sprite
	tmpSprite.setTexture(BG1, true);
	ThemeSprite1->emplace(std::pair{ ElementName::Background, ElementState::Neutral }, tmpSprite);

	//Display Area sprite

	tmpSprite.setTexture(DisplayArea1);
	tmpSprite.setTextureRect(sf::IntRect(0, 157, 1120, 691));
	ThemeSprite1->emplace(std::pair{ ElementName::DisplayArea, ElementState::Neutral }, tmpSprite);

	//Tab sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 0, 277, 59));
	ThemeSprite1->emplace(std::pair{ ElementName::Tab, ElementState::Clicked }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::Tab, ElementState::Focused }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(277, 0, 277, 59));
	ThemeSprite1->emplace(std::pair{ ElementName::Tab, ElementState::Neutral }, tmpSprite);

	//Playbar sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 848, 1120, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Clicked }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(0, 868, 1120, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Focused }, tmpSprite);

	//Media Buttons sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Play, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(350, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Play, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(50, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipBackward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(400, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipBackward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(100, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipForward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(450, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipForward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(150, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Backward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(500, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Backward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(200, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Forward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(550, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Forward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(250, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Pause, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(600, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Pause, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(300, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Replay, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(650, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Replay, ElementState::Focused }, tmpSprite);

	//Theme Button sprite
	tmpSprite.setTextureRect(sf::IntRect(950, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ThemeButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ThemeButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ThemeButton, ElementState::Clicked }, tmpSprite);

	//View direction buttons sprite
	tmpSprite.setTextureRect(sf::IntRect(700, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewUp, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewUp, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewUp, ElementState::Clicked }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(750, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewDown, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewDown, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewDown, ElementState::Clicked }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(800, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewLeft, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewLeft, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewLeft, ElementState::Clicked }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(850, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewRight, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewRight, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewRight, ElementState::Clicked }, tmpSprite);
	
	tmpSprite.setTextureRect(sf::IntRect(900, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ResetView, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ResetView, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ResetView, ElementState::Clicked }, tmpSprite);

	//Speed bar sprite

	tmpSprite.setTextureRect(sf::IntRect(150, 971, 160, 10));
	ThemeSprite1->emplace(std::pair{ ElementName::SpeedBar, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(310, 967, 20, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::SpeedBar, ElementState::Focused }, tmpSprite);

	//Go back Button
	tmpSprite.setTextureRect(sf::IntRect(0, 938, 75, 75));
	ThemeSprite1->emplace(std::pair{ ElementName::GoBack, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(75, 938, 75, 75));
	ThemeSprite1->emplace(std::pair{ ElementName::GoBack, ElementState::Focused }, tmpSprite);

	//Menu background

	tmpSprite.setTexture(MenuBG, true);
	ThemeSprite1->emplace(std::pair{ ElementName::MenuBackground, ElementState::Neutral }, tmpSprite);

	//Menu buttons
	tmpSprite.setTexture(MenuWidget);
	tmpSprite.setTextureRect(sf::IntRect(0, 0, 394, 293));
	ThemeSprite1->emplace(std::pair{ ElementName::GoTo, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(394, 0, 394, 293));
	ThemeSprite1->emplace(std::pair{ ElementName::GoTo, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(0, 0, 394, 293));
	ThemeSprite1->emplace(std::pair{ ElementName::GoTo, ElementState::Clicked }, tmpSprite);

	//Background color
	ThemeColor1->emplace(std::pair{ElementName::BackgroundColor, ElementState::Neutral }, LightBeige);

	//Second Theme/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Sprite
	ThemeSprite1 = &m_themeSprite[1];

	//TextBox Sprite
	tmpSprite.setTexture(Elements2);
	tmpSprite.setTextureRect(sf::IntRect(0, 164, 234, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::TextBox, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::TextBox, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(234, 164, 234, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::TextBox, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::TextBox, ElementState::Clicked }, tmpSprite);

	//TextBox text color
	ThemeColor1 = &m_themeColor[1];
	ThemeColor1->emplace(std::pair{ ElementName::TextBox, ElementState::Neutral }, Navy);
	ThemeColor1->emplace(std::pair{ ElementName::TextBox, ElementState::Deactivate }, Navy);
	ThemeColor1->emplace(std::pair{ ElementName::TextBox, ElementState::Focused }, LightBeige);
	ThemeColor1->emplace(std::pair{ ElementName::TextBox, ElementState::Clicked }, LightBeige);

	//BrowseButton Sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::BrowseButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::BrowseButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(142, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::BrowseButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::BrowseButton, ElementState::Clicked }, tmpSprite);

	//Matrix Sprite

	tmpSprite.setTextureRect(sf::IntRect(284, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(426, 56, 142, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::MatrixButton, ElementState::Clicked }, tmpSprite);

	//Randombutton Sprite
	tmpSprite.setTextureRect(sf::IntRect(0, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(104, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::RandomButton, ElementState::Clicked }, tmpSprite);

	//InputButton Sprite
	tmpSprite.setTextureRect(sf::IntRect(52, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Deactivate }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(156, 0, 52, 56));
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::InputButton, ElementState::Clicked }, tmpSprite);

	//Drawer sprite
	tmpSprite.setTextureRect(sf::IntRect(0, 220, 226, 70));
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Deactivate }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(226, 220, 226, 70));
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::Drawer, ElementState::Clicked }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(0, 113, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::DrawerArrow, ElementState::Neutral }, tmpSprite);

	//Background sprite
	tmpSprite.setTexture(BG2, true);
	ThemeSprite1->emplace(std::pair{ ElementName::Background, ElementState::Neutral }, tmpSprite);

	//Display Area sprite

	tmpSprite.setTexture(DisplayArea2);
	tmpSprite.setTextureRect(sf::IntRect(0, 157, 1120, 691));
	ThemeSprite1->emplace(std::pair{ ElementName::DisplayArea, ElementState::Neutral }, tmpSprite);

	//Tab sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 0, 277, 59));
	ThemeSprite1->emplace(std::pair{ ElementName::Tab, ElementState::Clicked }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::Tab, ElementState::Focused }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(277, 0, 277, 59));
	ThemeSprite1->emplace(std::pair{ ElementName::Tab, ElementState::Neutral }, tmpSprite);

	//Playbar sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 848, 1120, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Clicked }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(0, 868, 1120, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Focused }, tmpSprite);

	//Media Buttons sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Play, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(350, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Play, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(50, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipBackward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(400, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipBackward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(100, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipForward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(450, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::SkipForward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(150, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Backward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(500, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Backward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(200, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Forward, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(550, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Forward, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(250, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Pause, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(600, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Pause, ElementState::Focused }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(300, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Replay, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(650, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::Replay, ElementState::Focused }, tmpSprite);

	//Theme Button sprite
	tmpSprite.setTextureRect(sf::IntRect(950, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ThemeButton, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ThemeButton, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ThemeButton, ElementState::Clicked }, tmpSprite);

	//View direction buttons sprite
	tmpSprite.setTextureRect(sf::IntRect(700, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewUp, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewUp, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewUp, ElementState::Clicked }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(750, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewDown, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewDown, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewDown, ElementState::Clicked }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(800, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewLeft, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewLeft, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewLeft, ElementState::Clicked }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(850, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ViewRight, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewRight, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ViewRight, ElementState::Clicked }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(900, 888, 50, 50));
	ThemeSprite1->emplace(std::pair{ ElementName::ResetView, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ResetView, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::ResetView, ElementState::Clicked }, tmpSprite);

	//Speed bar sprite

	tmpSprite.setTextureRect(sf::IntRect(150, 971, 160, 10));
	ThemeSprite1->emplace(std::pair{ ElementName::SpeedBar, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(310, 967, 20, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::SpeedBar, ElementState::Focused }, tmpSprite);

	//Go back Button
	tmpSprite.setTextureRect(sf::IntRect(0, 938, 75, 75));
	ThemeSprite1->emplace(std::pair{ ElementName::GoBack, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(75, 938, 75, 75));
	ThemeSprite1->emplace(std::pair{ ElementName::GoBack, ElementState::Focused }, tmpSprite);

	//Menu background

	tmpSprite.setTexture(MenuBG, true);
	ThemeSprite1->emplace(std::pair{ ElementName::MenuBackground, ElementState::Neutral }, tmpSprite);

	//Menu buttons
	tmpSprite.setTexture(MenuWidget);
	tmpSprite.setTextureRect(sf::IntRect(0, 0, 394, 293));
	ThemeSprite1->emplace(std::pair{ ElementName::GoTo, ElementState::Neutral }, tmpSprite);
	tmpSprite.setTextureRect(sf::IntRect(394, 0, 394, 293));
	ThemeSprite1->emplace(std::pair{ ElementName::GoTo, ElementState::Focused }, tmpSprite);

	//Background color
	ThemeColor1->emplace(std::pair{ ElementName::BackgroundColor, ElementState::Neutral }, Navy);
}