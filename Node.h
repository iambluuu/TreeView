#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

enum NodeLink {
	NLeft = 1, NRight, NMiddle
};

enum class NodeState {
	Selected, Visited, Default, New, InRemove, Found, NotFound, Marked
};

class Node;

struct NodeInfo {
	
	std::vector<int> m_shownValue{ std::vector<int>(3) };
	std::vector<Node*> m_arrowCoord{ std::vector<Node*>(26, nullptr)};
	std::vector<Node*> m_arrowChange{ std::vector<Node*>(26, nullptr)};

	bool is_moving{ 0 };
	bool is_visible{ 0 };
	bool is_stateChanging{ 0 };
	int is_splitting{ 0 }; //0 is no, 1 is splitting, 2 is merging
	int is_expanding{ 0 }; //0 is no, 1 is expanding, 2 is shrinking;
	int is_appearing{ 0 }; //0 is no, 1 is appearing, 2 is disappearing;
	bool is_valueChanging{ 0 };

	int value_num{ 1 };
	int m_bf{ 0 };
	int m_index{ -1 };
	std::pair<NodeState, NodeState> node_state{ NodeState::Default, NodeState::Default };
	std::pair<std::pair<float, float>, std::pair<float, float> > m_coord{ {0, 0}, {0, 0} };
	std::pair<int, int> m_valueChange{ 0, 0 };
	bool splitFromleft{ 0 };
};

struct GraphNodeInfo {
	std::vector<int> edge_state; //First is index, Second is change type: 0 - flow from u, 1 - flow from v, 2 - change color, -1 - do nothing

	bool is_visible{ 0 };
	bool is_stateChanging{ 0 };
	int is_appearing{ 0 }; //0 is no, 1 is appearing, 2 is disappearing;

	std::pair<int, int> m_dist{ -1, -1 };
	std::pair<NodeState, NodeState> node_state{ NodeState::Default, NodeState::Default };
};

const NodeInfo DEFAULT_NODE_INFO;

class Node {
private:
	std::vector<NodeInfo> m_info;
	std::vector<int> m_value;

public:
	NodeInfo m_save;
	int height{ 0 };
	int child_num{ 0 };

	Node* left{ nullptr }, * right{ nullptr }, * middle{ nullptr };
	Node* child[26];
	Node* par{ nullptr };

	Node(std::vector<int> l_value = std::vector<int>(3), Node* l_left = nullptr, Node* l_right = nullptr, Node* l_middle = nullptr) {
		memset(child, 0, sizeof(child));
		m_save.m_shownValue = l_value;
		left = l_left;
		right = l_right;
		middle = l_middle;
	}

	Node(int value) {
		m_save.m_shownValue[0] = value;
		m_save.value_num = 1;

		left = nullptr;
		right = nullptr;
		middle = nullptr;
	}

	~Node() {
		if (left || right || middle)
			std::cerr << "You forgot to clean the tree";

		for (int i = 0; i < 26; ++i)
			if (child[i])
				std::cerr << "You forgot to clean the tree";

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

	int GetBalance() {
		int HeightLeft = (left) ? left->height : 0;
		int HeightRight = (right) ? right->height : 0;

		return HeightLeft - HeightRight;
	}

	void setValue(int l_val) {
		m_value.push_back(l_val);
	}

	void setValue(std::vector<int> l_val) {
		m_value = l_val;
		sort(m_value.begin(), m_value.end());
	}

	std::vector<int> getValue() {
		if (m_info.empty())
			return m_save.m_shownValue;

		return m_info.back().m_shownValue;
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
		if (m_info.empty())
			return;

		m_save = m_info.back();

		m_save.is_appearing = 0;
		m_save.splitFromleft = 0;
		m_save.is_splitting = 0;

		if (m_save.node_state.second == NodeState::Marked || (m_save.node_state.first == NodeState::Marked && m_save.node_state.second != NodeState::InRemove)) {
			m_save.node_state = { NodeState::Marked, NodeState::Marked };
		} else
			m_save.node_state = { NodeState::Default, NodeState::Default };

		m_save.is_stateChanging = 0;

		if (m_save.is_moving) {
			m_save.is_moving = 0;
			m_save.m_coord.first = m_save.m_coord.second;
		}

		if (m_save.is_valueChanging) {
			m_save.is_valueChanging = 0;

			m_save.m_shownValue[m_save.m_valueChange.first] = m_save.m_valueChange.second;
		}

		if (m_save.is_expanding == 2) {
			for (int i = m_save.m_valueChange.first; i < m_save.value_num - 1; i++) {
				m_save.m_shownValue[i] = m_save.m_shownValue[i + 1];
			}
			m_save.m_shownValue[m_save.value_num - 1] = 0;
			m_save.value_num--;
		}

		m_save.is_expanding = 0;

		for (int i = 0; i < 26; i++) {
			m_save.m_arrowCoord[i] = m_save.m_arrowChange[i];
		}
	}

};

class GraphNode {
private:
	void OnClick();
	void OnHover();
	void OnRelease();

	bool isHeld{ 0 };

public:
	std::vector<GraphNodeInfo> m_info;
	GraphNodeInfo m_save;

	std::string m_value{ "" };

	std::vector<std::pair<GraphNode*, int> > m_edges; // first - node, second - weight
	GraphNode* par{ this };

	const float m_mass{ 100 };
	sf::Vector2f pos{ 0, 0 };
	sf::Vector2f m_acc{ 0, 0 };
	sf::Vector2f m_vel{ 0, 0 };
	sf::Vector2f m_force{ 0, 0 };

	bool visited{ 0 };
	bool isDrawn{ 0 };
	bool isFixed{ 0 };

	GraphNode(std::string val = "", sf::Vector2f l_pos = {0, 0}) {
		pos = l_pos;

		m_value = val;
		m_save.is_visible = 1;
	}

	bool isHover(sf::Vector2f mousePos);
	void HandleEvent(const sf::Event* l_event, sf::RenderWindow* l_target, sf::Vector2f offset);
	void SaveState() {
		if (m_info.empty())
			return;

		m_save = m_info.back();
		m_save.is_appearing = 0;
		m_save.m_dist = { -1, -1 };

		for (int i = 0; i < m_save.edge_state.size(); i++) {
			m_save.edge_state[i] = -1;
		}

		if (m_save.node_state.second == NodeState::Marked || (m_save.node_state.first == NodeState::Marked && m_save.node_state.second != NodeState::InRemove)) {
			m_save.node_state = { NodeState::Marked, NodeState::Marked };
		}
		else
			m_save.node_state = { NodeState::Default, NodeState::Default };

		m_save.is_stateChanging = 0;
	}

	void ChangeState(NodeState l_state) {
		if (m_info.empty())
			return;

		m_info.back().is_stateChanging = 1;
		m_info.back().node_state.second = l_state;
	}
};