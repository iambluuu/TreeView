#include "Hash_Table.h"



HashTable::~HashTable() {
	ClearNodes(m_probingNodes);
	ClearNodes(m_chainingNodes);
}

void HashTable::ClearNodes(std::vector<Node*>& nodes) {
	for (int i = 0; i < nodes.size(); i++) {
		ClearChain(nodes[i]);
	}

	nodes.clear();
}

void HashTable::ClearChain(Node* Cur) {
	if (!Cur)
		return;

	ClearChain(Cur->left);
	delete Cur;
	Cur = nullptr;
}

bool HashTable::ValidateInput(const std::string& l_value, int& resValue) {
	if (l_value.empty() || l_value.size() > 3)
		return false;

	for (int i = 0; i < l_value.size(); i++) {
		if (!isdigit(l_value[i]))
			return false;
	}

	resValue = std::stoi(l_value);
	return true;
}

void HashTable::AddNodeStep(Node* node) {
	auto CurInfo = node->getInfo();

	if (CurInfo->empty()) {
		CurInfo->push_back(node->m_save);

	}
	else {
		NodeInfo clone = CurInfo->back();
		clone.is_expanding = 0;

		if (clone.is_appearing == 2) {
			clone.is_visible = 0;
		}

		clone.is_appearing = 0;

		if (clone.is_moving) {
			clone.is_moving = 0;
			clone.m_coord.first = clone.m_coord.second;
		}

		if (clone.is_stateChanging == 1) {
			clone.is_stateChanging = 0;
			clone.node_state.first = clone.node_state.second;
		}

		if (clone.node_state.first == NodeState::Selected) {
			clone.is_stateChanging = 1;
			clone.node_state.second = NodeState::Visited;
		}

		if (clone.is_valueChanging) {
			clone.is_valueChanging = 0;
			clone.m_valueChange.first = clone.m_valueChange.second;
			clone.m_shownValue[0] = clone.m_valueChange.first;
		}

		for (int i = 0; i < 3; i++) {
			if (!clone.m_arrowCoord[i])
				continue;

			if (clone.m_arrowCoord[i]->getInfo()->back().is_appearing == 2) {
				clone.m_arrowCoord[i] = nullptr;
			}
		}

		CurInfo->push_back(clone);
	}
}

void HashTable::AddNewStep() {
	std::vector<Node*>* nodes;
	if (m_mode) {
		nodes = &m_probingNodes;
	}
	else {
		nodes = &m_chainingNodes;
	}

	for (int i = 0; i < nodes->size(); i++) {
		Node* Cur = (*nodes)[i];
		if (!Cur)
			continue;

		while(Cur) {
			AddNodeStep(Cur);
			Cur = Cur->left;
		}
	}
}

void HashTable::InsertChaining(int l_value) {
	int n = m_chainingNodes.size();
	int index = l_value % n;

	Node* newNode = new Node(l_value);

	if (!m_chainingNodes[index]) {
		m_chainingNodes[index] = newNode;
		return;
	}

	Node* Cur = m_chainingNodes[index];
	AddNewStep();
	AddNodeStep(newNode);

	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	while (Cur->left) {
		Cur = Cur->left;
	}
	Cur->left = newNode;

	AddNewStep();
	newNode->getInfo()->back().is_visible = 1;
	newNode->getInfo()->back().is_appearing = 1;

}


void HashTable::InsertLinearProbing(int l_value) {
	int n = m_probingNodes.size();
	int index = l_value % n;

	Node* newNode = new Node(l_value);

	if (!m_probingNodes[index]) {
		m_probingNodes[index] = newNode;
		return;
	}

	int i = 1;
	while (m_probingNodes[(index + i) % n]) {
		i++;
	}

	AddNewStep();
	m_probingNodes[(index + i) % n] = newNode;
	newNode->getInfo()->back().is_appearing = 1;
}

void HashTable::OnInsert(const std::string& l_value) {
	int resValue;

	if (!ValidateInput(l_value, resValue)) {
		std::cerr << "Invalid input\n";
		return;
	}

	if (m_mode == 0) {
		// chaining
		
	}
	else if (m_mode == 1) {
		// linear probing
	}
	else {
		// quadratic probing
	}
}