#include "ThemeManager.h"
#include "UI_Element.h"

#include <utility>

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
	//BG2.loadFromFile("Assets/Texture/Background2.png");
	Elements.loadFromFile("Assets/Texture/Elements.png");
	Elements.setSmooth(1);
	DisplayArea1.loadFromFile("Assets/Texture/DisplayArea.png");

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

	//Playbar sprite

	tmpSprite.setTextureRect(sf::IntRect(0, 848, 1120, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Neutral }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Clicked }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(0, 868, 1120, 20));
	ThemeSprite1->emplace(std::pair{ ElementName::PlayBar, ElementState::Focused }, tmpSprite);


}