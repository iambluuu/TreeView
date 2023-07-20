#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

const std::string cfg_path = "Resource/Texture.cfg";

class TextureManager {
private:
	sf::Texture BG1;
	sf::Texture BG2;
	sf::Texture NodeTexture;
	std::vector<std::pair<sf::Sprite, sf::Sprite> > NodeSprite;

	sf::Color m_selected;
	sf::Color m_new;
	sf::Color m_remove;

public:
	TextureManager() {

	}

	~TextureManager() {

	}


	std::pair<sf::Sprite, sf::Sprite> GetNodeSprite(int val_num) {
		return NodeSprite[val_num - 1];
	}
};