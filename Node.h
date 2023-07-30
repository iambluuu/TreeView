#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

enum NodeLink {
	NLeft = 1, NRight, NMiddle
};

enum class NodeState {
	Selected, Visited, Default, New, InRemove
};

struct NodeInfo {
	
	std::vector<int> m_shownValue{ std::vector<int>(3) };
	
	bool is_moving{ 0 };
	bool is_visible{ 0 };
	bool is_stateChanging{ 0 };
	bool is_splitting{ 0 };
	bool is_expanding{ 0 };
	int is_appearing{ 0 }; //0 is no, 1 is appearing, 2 is disappearing;

	int value_num{ 1 };
	std::pair<NodeState, NodeState> node_state{ NodeState::Default, NodeState::Default };
	std::pair<std::pair<int, int>, std::pair<int, int> > m_coord{ {0, 0}, {0, 0} };
	
};

const NodeInfo DEFAULT_NODE_INFO;

class Node {
private:
	std::vector<NodeInfo> m_info;
	std::vector<int> m_value;
	sf::IntRect m_zone;

public:
	NodeInfo m_save;
	int height{ 0 };
	Node* left{ nullptr }, * right{ nullptr }, * middle{ nullptr };
	Node* par{ nullptr };

	Node(std::vector<int> l_value = std::vector<int>(3), Node* l_left = nullptr, Node* l_right = nullptr, Node* l_middle = nullptr) {
		m_save.m_shownValue = l_value;
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

	int GetHeight() {
		int HeightLeft = (left) ? left->height : 0;
		int HeightRight = (right) ? right->height : 0;

		return (HeightLeft > HeightRight) ? HeightLeft + 1 : HeightRight + 1;
	}

	void setValue(int l_val) {
		m_value.push_back(l_val);
	}

	void setValue(std::vector<int> l_val) {
		m_value = l_val;
		sort(m_value.begin(), m_value.end());
	}

	std::vector<int> getValue() {
		return m_value;
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

	void SaveState() {
		m_save = m_info.back();

		m_save.is_appearing = 0;
		m_save.is_expanding = 0;
		m_save.is_stateChanging = 0;

		m_save.node_state = { NodeState::Default, NodeState::Default };

		if (m_save.is_moving) {
			m_save.is_moving = 0;
			m_save.m_coord.first = m_save.m_coord.second;
		}
	}

};