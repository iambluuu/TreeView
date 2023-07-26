#pragma once
#include "SFML/Graphics.hpp"
#include "UIManager.h"
#include <iostream>

enum class ElementName;
enum class ElementState;

using ThemeColor = std::vector<std::map<std::pair<ElementName, ElementState>, sf::Color>>;
using ThemeSprite = std::vector<std::map<std::pair<ElementName, ElementState>, sf::Sprite>>;

class ThemeManager {
private:
	ThemeColor m_themeColor;
	ThemeSprite m_themeSprite;

	sf::Texture BG1;
	sf::Texture BG2;

	sf::Texture DisplayArea1;
	sf::Texture DisplayArea2;

	sf::Texture Elements;

public:
	ThemeManager() {
		
		m_themeColor.resize(2);
		m_themeSprite.resize(2);

		BG1.loadFromFile("Assets/Texture/Background.png");
		//BG2.loadFromFile("Assets/Texture/Background2.png");
		Elements.loadFromFile("Assets/Texture/Elements.png");
		DisplayArea1.loadFromFile("Assets/Texture/DisplayArea.png");

		Initiate();
		std::cerr << "ThemeManager Initiated\n";

	}

	void Initiate();
	
	sf::Color* GetColor(int ThemeID, ElementName l_name, ElementState l_state) {
		ThemeColor* theme = &m_themeColor;
		return &theme->at(ThemeID)[{l_name, l_state}];
	}

	sf::Sprite* GetSprite(int ThemeID, ElementName l_name, ElementState l_state) {
		ThemeSprite* theme = &m_themeSprite;
		return &theme->at(ThemeID)[{l_name, l_state}];
	}
};