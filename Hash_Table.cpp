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
	Cur->left = nullptr;

	delete Cur;
}

void HashTable::ResetNodes() {
	std::vector<Node*>* nodes;

	if (m_mode) {
		nodes = &m_probingNodes;
	}
	else {
		nodes = &m_chainingNodes;
	}

	for (int i = 0; i < nodes->size(); i++) {
		Node* Cur = (*nodes)[i];

		while (Cur) {
			Cur->SaveState();
			Cur->getInfo()->clear();
			Cur = Cur->left;
		}
	}
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

bool HashTable::ValidateCreate(const std::string& l_numbers, const std::string& l_value, int& n, int& m) {
	if (l_numbers.empty())
		return false;
	
	for (int i = 0; i < l_numbers.size(); i++) {
		if (!isdigit(l_numbers[i]))
			return false;
	}
	
	n = std::stoi(l_numbers);
	if (n < 1 || n > 50)
		return false;

	for (int i = 0; i < l_value.size(); i++) {
		if (!isdigit(l_value[i]))
			return false;
	}

	if (l_value.empty())
		m = 0;
	else
		m = std::stoi(l_value);

	if (m < 1 || (m > n / 2 && m_mode) || (m > 200 && m_mode == 0))
		return false;

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

void HashTable::ShiftUp(Node* Cur) {
	if (!Cur)
		return;

	Cur->getInfo()->back().is_moving = 1;
	Cur->getInfo()->back().m_coord.second.second = Cur->getInfo()->back().m_coord.first.second - 1;

	ShiftUp(Cur->left);
}

void HashTable::InsertNode(int l_value) {
	int n;

	if (m_mode == 0) {
		n = m_chainingNodes.size();

		int index = l_value % n;

		Node* newNode = new Node(l_value);
		Node* Cur = m_chainingNodes[index];

		while (Cur->left) {
			Cur = Cur->left;
		}

		Cur->left = newNode;

	}
	else if (m_mode == 1) {
		n = m_probingNodes.size();

		int index = l_value % n;

		while (m_probingNodes[index]->m_save.m_shownValue[0] != 0) {
			index = (index + 1) % n;
		}

		m_probingNodes[index]->m_save.m_shownValue[0] = l_value;
	}
	else {
		n = m_probingNodes.size();
		int index = l_value % n;

		while (m_probingNodes[index]->m_save.m_shownValue[0] != 0) {
			index = (index * index) % n;
		}

		m_probingNodes[index]->m_save.m_shownValue[0] = l_value;
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
		AddNewStep();
		AddNodeStep(newNode);
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.first = NodeState::Default;
		Cur->getInfo()->back().node_state.second = NodeState::Selected;
	}

	Cur->left = newNode;
	AddNewStep();

	Cur->getInfo()->back().m_arrowCoord[0] = newNode;
	newNode->getInfo()->back().m_coord.first = { Cur->getInfo()->back().m_coord.first.first, Cur->getInfo()->back().m_coord.first.second + 1 };
	newNode->getInfo()->back().m_coord.second = { Cur->getInfo()->back().m_coord.first.first, Cur->getInfo()->back().m_coord.first.second + 1 };
	newNode->getInfo()->back().node_state.first = NodeState::Selected;
	newNode->getInfo()->back().node_state.second = NodeState::Selected;
	newNode->getInfo()->back().is_visible = 1;
	newNode->getInfo()->back().is_appearing = 1;

}


void HashTable::InsertLinearProbing(int l_value) {
	int n = m_probingNodes.size();
	int index = l_value % n;

	AddNewStep();
	m_probingNodes[index]->getInfo()->back().is_stateChanging = 1;
	m_probingNodes[index]->getInfo()->back().node_state.second = NodeState::Selected;

	while (m_probingNodes[index]->getInfo()->back().m_shownValue[0] > 0) {
		index = (index + 1) % n;

		AddNewStep();
		m_probingNodes[index]->getInfo()->back().is_stateChanging = 1;
		m_probingNodes[index]->getInfo()->back().node_state.second = NodeState::Selected;

	}

	AddNewStep();
	m_probingNodes[index]->getInfo()->back().is_stateChanging = 1;
	m_probingNodes[index]->getInfo()->back().node_state.second = NodeState::Selected;
	m_probingNodes[index]->getInfo()->back().m_valueChange.second = l_value;
	m_probingNodes[index]->getInfo()->back().is_valueChanging = 1;
}

void HashTable::InsertQuadraticProbing(int l_value) {
	int n = m_probingNodes.size();
	int index = l_value % n;

	AddNewStep();
	m_probingNodes[index]->getInfo()->back().is_stateChanging = 1;
	m_probingNodes[index]->getInfo()->back().node_state.second = NodeState::Selected;

	int i = 1;
	while (m_probingNodes[index]->getInfo()->back().m_shownValue[0] > 0 && i < n) {
		index = (l_value + i * i) % n;
		i++;

		std::cerr << index << std::endl;

		AddNewStep();
		m_probingNodes[index]->getInfo()->back().is_stateChanging = 1;
		m_probingNodes[index]->getInfo()->back().node_state.second = NodeState::Selected;

	}

	if (m_probingNodes[index]->getInfo()->back().m_shownValue[0] > 0) {
		std::cerr << "Table is full" << std::endl;
		AddNewStep();
		m_probingNodes[index]->getInfo()->back().is_stateChanging = 1;
		m_probingNodes[index]->getInfo()->back().node_state.second = NodeState::NotFound;
		return;
	}

	AddNewStep();
	m_probingNodes[index]->getInfo()->back().is_stateChanging = 1;
	m_probingNodes[index]->getInfo()->back().node_state.second = NodeState::Selected;
	m_probingNodes[index]->getInfo()->back().m_valueChange.second = l_value;
	m_probingNodes[index]->getInfo()->back().is_valueChanging = 1;
}

void HashTable::RemoveChaining(int value) {
	int n = m_chainingNodes.size();
	int index = value % n;

	Node* Cur = m_chainingNodes[index];

	AddNewStep();
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	Node* preCur = nullptr;
	while (Cur && Cur->getInfo()->back().m_shownValue[0] != value) {
		preCur = Cur;
		Cur = Cur->left;

		AddNewStep();
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.first = NodeState::Default;
	}

	if (!Cur) {
		return;
	}

	AddNewStep();
	Cur->getInfo()->back().is_stateChanging = 0;
	Cur->getInfo()->back().is_appearing = 2;
	
	ShiftUp(Cur);

	preCur->left = Cur->left;
	preCur->getInfo()->back().m_arrowChange[0] = Cur->left;
	
}

void HashTable::RemoveLinearProbing(int value) {
	int n = m_probingNodes.size();
	int index = value % n;
	Node* Cur = m_probingNodes[index];

	AddNewStep();
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	int i = 1;
	while (Cur->getInfo()->back().m_shownValue[0] != value && Cur->getInfo()->back().m_shownValue[0] != 0 && i < n) {
		index = (value % n + i) % n;
		i++;
		Cur = m_probingNodes[index];

		AddNewStep();
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::Selected;
	}
	
	if (Cur->getInfo()->back().m_shownValue[0] == value) {
		AddNewStep();
		Cur->getInfo()->back().is_stateChanging = 0;
		Cur->getInfo()->back().is_valueChanging = 1;
		Cur->getInfo()->back().m_valueChange.first = Cur->getInfo()->back().m_shownValue[0];
		Cur->getInfo()->back().m_valueChange.second = -1;
	}
	else {
		AddNewStep();
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::NotFound;
	}

}

void HashTable::RemoveQuadraticProbing(int value) {
	int n = m_probingNodes.size();
	int index = value % n;
	Node* Cur = m_probingNodes[index];

	AddNewStep();
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	int i = 1;
	while (Cur->getInfo()->back().m_shownValue[0] != value && Cur->getInfo()->back().m_shownValue[0] != 0 && i < n) {
		index = (value % n + i * i) % n;
		i++;

		std::cerr << Cur->getInfo()->back().m_shownValue[0] << " " << value << std::endl;

		Cur = m_probingNodes[index];

		AddNewStep();
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::Selected;
	}

	if (Cur->getInfo()->back().m_shownValue[0] == value) {
		AddNewStep();
		Cur->getInfo()->back().is_stateChanging = 0;
		Cur->getInfo()->back().is_valueChanging = 1;
		Cur->getInfo()->back().m_valueChange.first = Cur->getInfo()->back().m_shownValue[0];
		Cur->getInfo()->back().m_valueChange.second = -1;

		std::cerr << "Found and deleting " << Cur->getInfo()->back().m_index << " " << value << std::endl;
	}
	else {
		AddNewStep();
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::NotFound;
	}
}

void SearchChaining(int value);
void SearchLinearProbing(int value);
void SearchQuadraticProbing(int value);

void HashTable::OnCreate(const std::string& l_numbers, const std::string& l_value) {
	int n = 0, m = 0;
	if (!ValidateCreate(l_numbers, l_value, n, m)) {
		std::cerr << "Invalid input\n";
		return;
	}

	if (m_mode)
		ClearNodes(m_probingNodes);
	else
		ClearNodes(m_chainingNodes);

	Create(n, m);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	if (m_mode) {
		renderer->Reset(m_probingNodes[0]->getInfo()->size());
	}
	else {
		renderer->Reset(m_chainingNodes[0]->getInfo()->size());
	}
}

void HashTable::OnInsert(const std::string& l_value) {
	int resValue;

	if (!ValidateInput(l_value, resValue)) {
		std::cerr << "Invalid input\n";
		return;
	}

	if ((m_mode == 0 && m_chainingNodes.size() == 0) || (m_mode && m_probingNodes.size() == 0)) {
		std::cerr << "Create table first\n";
		return;
	}

	ResetNodes();

	if (m_mode == 0) {
		// chaining
		InsertChaining(resValue);
		
	}
	else if (m_mode == 1) {
		// linear probing
		InsertLinearProbing(resValue);
	}
	else {
		// quadratic probing
		InsertQuadraticProbing(resValue);
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	if (m_mode) {
		renderer->Reset(m_probingNodes[0]->getInfo()->size());
	}
	else {
		renderer->Reset(m_chainingNodes[0]->getInfo()->size());
	}
}

void HashTable::OnRemove(const std::string& l_value) {
	int resValue;

	if (!ValidateInput(l_value, resValue)) {
		std::cerr << "Invalid input\n";
		return;
	}

	if ((m_mode == 0 && m_chainingNodes.size() == 0) || (m_mode && m_probingNodes.size() == 0)) {
		std::cerr << "Create table first\n";
		return;
	}

	ResetNodes();

	if (m_mode == 0) {
		// chaining
		RemoveChaining(resValue);
	}
	else if (m_mode == 1) {
		// linear probing
		RemoveLinearProbing(resValue);
	}
	else {
		// quadratic probing
		RemoveQuadraticProbing(resValue);
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	if (m_mode) {
		renderer->Reset(m_probingNodes[0]->getInfo()->size());
	}
	else {
		renderer->Reset(m_chainingNodes[0]->getInfo()->size());
	}

}

void HashTable::Create(int n, int m) {
	std::vector<Node*>* nodes;

	if (m_mode == 0) {
		nodes = &m_chainingNodes;
	}
	else {
		nodes = &m_probingNodes;
	}

	//Create all nodes and add values

	for (int i = 0; i < n; i++) {
		Node* newNode = new Node();
		nodes->push_back(newNode);
	}

	for (int i = 0; i < m; i++) {
		InsertNode(rand() % 100 + 1);
	}

	//Add appearing animation
	AddNewStep();
	for (int i = 0; i < nodes->size(); i++) {
		Node* Cur = (*nodes)[i];

		if (!Cur)
			continue;

		Cur->getInfo()->back().m_index = i;

		if (!m_mode) {
			int curY = DEFAULT_COORD.y;
			int curX = DEFAULT_COORD.x + i;
			while (Cur) {
				Cur->getInfo()->back().m_coord.first = std::make_pair(curX, curY);
				Cur->getInfo()->back().m_coord.second = std::make_pair(curX, curY);
				Cur->getInfo()->back().m_arrowCoord[0] = Cur->left;
				Cur->getInfo()->back().is_visible = 1;
				Cur->getInfo()->back().is_appearing = 1;
				Cur = Cur->left;

				curY++;
			}
		}
		else {
			int curX = DEFAULT_COORD.x + i % NODE_PER_ROW;
			int curY = DEFAULT_COORD.y + i / NODE_PER_ROW;

			Cur->getInfo()->back().m_coord.first = std::make_pair(curX, curY);
			Cur->getInfo()->back().m_coord.second = std::make_pair(curX, curY);
			Cur->getInfo()->back().is_visible = 1;
			Cur->getInfo()->back().is_appearing = 1;
		}

	}
}

void HashTable::HandleEvent(sf::Event* l_event) {
	NodeRenderer* nodeRenderer = m_stateManager->GetContext()->m_nodeRenderer;
	nodeRenderer->HandleEvent(l_event);
}

void HashTable::Update(const sf::Time& l_time) {
	NodeRenderer* nodeRenderer = m_stateManager->GetContext()->m_nodeRenderer;
	nodeRenderer->Update(l_time.asMilliseconds());
}
void HashTable::Draw() {
	NodeRenderer* nodeRenderer = m_stateManager->GetContext()->m_nodeRenderer;

	if (m_mode == 0) {
		for (auto& node : m_chainingNodes) {
			nodeRenderer->DrawTree(node);
		}
	}
	else {
		for (auto& node : m_probingNodes) {
			nodeRenderer->DrawTree(node);
		}
	}
}