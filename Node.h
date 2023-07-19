#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

enum class NodeLink {
	NLeft = 1, NRight, NMiddle
};

struct NodeInfo {
	sf::Vector2f m_coord{ 0, 0 };
	std::vector<int> m_shownValue;
	
	bool isSelected{ 0 };
	bool isVisible{ 1 };
	bool isColorChanging{ 0 };
	bool isSplitting{ 0 };
	int isAppearing{ 0 }; //0 is no, 1 is appearing, 2 is disappearing;
};

const NodeInfo DEFAULT_NODE_INFO;

class Node {
private:
	Node* left{ nullptr }, * right{ nullptr }, * middle{ nullptr };
	std::vector<NodeInfo> m_info;
	std::vector<int> m_value;

public:
	Node(std::vector<int> l_value = std::vector<int>(3), Node* l_left = nullptr, Node* l_right = nullptr, Node* l_middle = nullptr) {
		m_value = l_value;
		left = l_left;
		right = l_right;
		middle = l_middle;
	}
	~Node() {
		if (left || right || middle)
			std::cerr << "oof";
	}

	std::vector<NodeInfo>* getInfo() {
		return &m_info;
	}

	void setLink(Node* l_node, NodeLink l_link) {
		switch (l_link) {
		case NodeLink::NLeft:
			left = l_node;
			break;
		case NodeLink::NRight:
			right = l_node;
			break;
		case NodeLink::NMiddle:
			middle = l_node;
			break;
		}
	}
	
	Node* getLink(NodeLink l_link) {
		switch (l_link) {
		case NodeLink::NLeft:
			return left;
		case NodeLink::NRight:
			return right;
		case NodeLink::NMiddle:
			return middle;
		}
	}

	sf::Vector2f getCoord(int l_step) {
		return m_info[l_step].m_coord;
	}

	int getValue(NodeLink l_pos) {
		switch (l_pos) {
		case NodeLink::NLeft:
			return m_value[0];
		case NodeLink::NRight:
			return m_value[2];
		case NodeLink::NMiddle:
			return m_value[1];
		}
	}

	void setCoord(int l_step, sf::Vector2f l_coord) {
		m_info[l_step].m_coord = l_coord;
	}

};