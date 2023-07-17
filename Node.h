#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

struct NodeInfo {
	sf::Color m_nodeColor{sf::Color::White};
	sf::Color m_borderColor{sf::Color::Black};
	sf::Vector2f m_coord;
	std::vector<int> m_val;
	
	bool isColorChanging{ 0 };
	bool isSplitting{ 0 };
};

class Node {
private:
	Node* left{ nullptr }, * right{ nullptr }, * middle{ nullptr };
	NodeInfo m_info;

public:
	Node();
	~Node() {
		if (left || right || middle)
			std::cerr << "oof";
	}

	void LinkLeft(Node* l_node) {
		left = l_node;
	}

	void LinkRight(Node* l_node) {
		right = l_node;
	}

	void LinkMiddle(Node* l_node) {
		middle = l_node;
	}

	sf::Vector2f getCoord() {
		return m_info.m_coord;
	}

	void setCoord(sf::Vector2f l_coord) {
		m_info.m_coord = l_coord;
	}

	void setColor(sf::Color l_color) {
		m_info.m_nodeColor = l_color;
	} 
	

};