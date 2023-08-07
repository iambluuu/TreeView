#pragma once
#include "SFML/Graphics.hpp"
#include "UI_Element.h"
//#include "NodeRenderer.h"
#include <iostream>

enum class NodeState;
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

	sf::Texture MenuBG;
	sf::Texture MenuWidget;

	sf::Texture DisplayArea1;
	sf::Texture DisplayArea2;

	sf::Texture Elements;
	sf::Font m_font;

	sf::Cursor DefaultCursor;
	sf::Cursor HandCursor;
	sf::Cursor TextCursor;

public:
	ThemeManager();

	void Initiate();
	
	std::tuple<sf::Color, sf::Color, sf::Color>* GetNodeColor(int ThemeID, NodeState l_state);

	sf::Color* GetColor(int ThemeID, ElementName l_name, ElementState l_state);

	sf::Sprite* GetSprite(int ThemeID, ElementName l_name, ElementState l_state);

	sf::Font* GetFont() {
		return &m_font;
	}

	sf::Cursor* GetCursor(sf::Cursor::Type l_type) {
		switch (l_type)
		{	
		case sf::Cursor::Arrow:
			return &DefaultCursor;
			break;
	
		case sf::Cursor::Text:
			return &TextCursor;
			break;
		case sf::Cursor::Hand:
			return &HandCursor;
			break;

		default:
			break;
		}
	}
};