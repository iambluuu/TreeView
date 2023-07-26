#include "ThemeManager.h"

#include <utility>

const sf::Color DarkBlue = sf::Color(45, 55, 73);
const sf::Color LightBlue = sf::Color(53, 66, 89);
const sf::Color LightBeige = sf::Color(240, 233, 210);

void ThemeManager::Initiate()
{
	//First Theme
	auto ThemeSprite1 = &m_themeSprite[0];
	sf::Sprite tmpSprite;
	tmpSprite.setTexture(Elements);
	tmpSprite.setTextureRect(sf::IntRect(0, 164, 234, 56));
	ThemeSprite1->emplace(std::pair{ElementName::TextBox, ElementState::Neutral }, tmpSprite);

	tmpSprite.setTextureRect(sf::IntRect(234, 164, 234, 56));
	ThemeSprite1->emplace(std::pair{ElementName::TextBox, ElementState::Focused }, tmpSprite);
	ThemeSprite1->emplace(std::pair{ElementName::TextBox, ElementState::Clicked }, tmpSprite);


	auto ThemeColor1 = &m_themeColor[0];
	ThemeColor1->emplace(std::pair{ElementName::TextBox, ElementState::Neutral }, LightBeige);
	ThemeColor1->emplace(std::pair{ElementName::TextBox, ElementState::Focused }, DarkBlue);
	ThemeColor1->emplace(std::pair{ElementName::TextBox, ElementState::Clicked }, DarkBlue);
}