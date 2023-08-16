#include "Heap.h"

Heap::Heap(StateManager* l_stateManager) : BaseState(l_stateManager) {
	SwitchMode(0);
}

Heap::~Heap() {
	//delete m_root;
	ClearTree();
}

bool Heap::ValidateCreate(const std::string& l_value, std::vector<int>& res) {
	std::string soFar = "";

	if (l_value.empty()) {
		return false;
	}

	for (int i = 0; i < l_value.size(); i++) {
		if (l_value[i] == ',' || l_value[i] == ' ') {
			if (soFar != "") {
				if (soFar.size() < 3) {
					res.push_back(std::stoi(soFar));
				}
				else {
					std::cerr << "Value too large\n";
					return false;
				}
				soFar.clear();
			}
		}
		else if (!std::isdigit(l_value[i])) {
			std::cerr << "Invalid character: " << l_value[i] << "\n";
			return false;
		}
		else {
			soFar += l_value[i];
		}
	}

	if (!soFar.empty()) {
		if (soFar.size() > 3) {
			std::cerr << "Value too large\n";
			return false;
		}
		res.push_back(std::stoi(soFar));
		soFar.clear();
	}

	return true;
}


bool Heap::ValidateInput(const std::string& l_value, std::vector<int>& res) {
	if (l_value.empty()) {
		return false;
	}

	int num = 0;
	int sign = 1;
	int i = 0;

	if (l_value[0] == '-') {
		sign *= -1;
		i++;
	}

	for (; i < l_value.size(); i++) {
		if (l_value[i] < '0' || l_value[i] > '9') {
			return false;
		}

		num = num * 10 + l_value[i] - '0';
	}

	num *= sign;
	if (num > 999 || num <= 0) {
		return false;
	}

	res.push_back(num);

	return true;
}

void Heap::AddNodeStep(Node* Cur) {
	auto CurInfo = Cur->getInfo();

	if (CurInfo->empty()) {
		CurInfo->push_back(Cur->m_save);

	}
	else {
		NodeInfo clone = CurInfo->back();
		clone.is_splitting = 0;
		clone.splitFromleft = 0;

		if (clone.is_appearing == 2) {
			clone.is_visible = 0;
		}

		clone.is_appearing = 0;

		if (clone.is_moving) {
			clone.is_moving = 0;
			clone.m_coord.first = clone.m_coord.second;
		}

		if (clone.is_stateChanging) {
			if (clone.node_state.first == NodeState::Marked) {
				if (clone.node_state.second == NodeState::InRemove) {
					clone.node_state.first = NodeState::InRemove;
					clone.node_state.second = NodeState::Default;
				}
				else
					std::swap(clone.node_state.first, clone.node_state.second);
			}
			else if (clone.node_state.second == NodeState::Selected) {
				clone.node_state.first = NodeState::Selected;
				clone.node_state.second = NodeState::Visited;
			}
			else {
				clone.node_state.first = clone.node_state.second;
				clone.is_stateChanging = 0;
			}
		}

		if (clone.is_valueChanging) {
			clone.is_valueChanging = 0;
			clone.m_shownValue[clone.m_valueChange.first] = clone.m_valueChange.second;
		}

		if (clone.is_expanding == 2) {
			for (int i = clone.m_valueChange.first; i < clone.value_num - 1; i++) {
				clone.m_shownValue[i] = clone.m_shownValue[i + 1];
			}
			clone.m_shownValue[clone.value_num - 1] = 0;
			clone.value_num--;
		}

		clone.is_expanding = 0;

		for (int i = 0; i < 26; i++) {
			clone.m_arrowCoord[i] = clone.m_arrowChange[i];
		}

		CurInfo->push_back(clone);
	}
}

void Heap::AddNewStep() {
	for (int i = 0; i < (*m_arr).size(); i++) {
		Node* Cur = (*m_arr)[i];
		if (!Cur)
			continue;

		AddNodeStep(Cur);
	}
}

void Heap::ResetNodes() {

	for (int i = 0; i < (*m_arr).size(); i++) {
		Node* Cur = (*m_arr)[i];
		if (!Cur)
			continue;

		Cur->SaveState();
		Cur->getInfo()->clear();
	}
}

void Heap::ClearTree() {
	for (int i = 0; i < (*m_arr).size(); i++) {
		delete (*m_arr)[i];
		(*m_arr)[i] = nullptr;
	}
}

void Heap::PostProcessing() {
	delete m_removed;
	m_removed = nullptr;

	m_arr->pop_back();
}

void Heap::HandleEvent(sf::Event* l_event) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->HandleEvent(l_event);
}

void Heap::Update(const sf::Time& l_time) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->Update(l_time.asMilliseconds());
}

void Heap::Draw() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	for (int i = 0; i < (*m_arr).size(); i++) {
		Node* Cur = (*m_arr)[i];
		if (!Cur)
			continue;

		renderer->DrawNode(Cur, 0);
	}
}

void Heap::OnCreate(const std::string& l_numbers, const std::string& l_value) {
	std::vector<int> value;

	if (!ValidateCreate(l_value, value)) {
		std::cerr << "Invalid input\n";
		return;
	}

	PostProcessing();
	ResetNodes();
	ClearTree();

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void Heap::OnInsert(const std::string& l_value) {
	std::vector<int> value;
	if (!ValidateInput(l_value, value)) {
		std::cerr << "Invalid input\n";
		return;
	}

	PostProcessing();
	ResetNodes();

	InsertNode(value[0]);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset((*m_arr)[0]->getInfo()->size());
}

void Heap::OnRemove(const std::string& l_value) {
	std::vector<int> value;
	if (!ValidateInput(l_value, value)) {
		std::cerr << "Invalid input\n";
		return;
	}

	PostProcessing();
	ResetNodes();
	RemoveNode(value[0]);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void Heap::OnSearch(const std::string& l_value) {
	std::vector<int> value;
	if (!ValidateInput(l_value, value))
		return;

	PostProcessing();
	ResetNodes();

	SearchNode(value[0]);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_arr->front()->getInfo()->size());
}

void Heap::BuildTree(Node* Cur, Node* pre, int index, const std::string& l_string) {

}

void Heap::InsertNode(int value) {
	Node* Cur = new Node(value);

	Cur->m_save.is_appearing = 1;
	Cur->m_save.is_visible = 1;
	Cur->m_save.node_state = std::make_pair(NodeState::Selected, NodeState::Selected);
	m_arr->push_back(Cur);

	int index = m_arr->size() - 1;
	std::cerr << m_arr->at(0)->m_save.m_shownValue[0] << "\n";
	
	AddNewStep();

	if (index == 0)
		return;

	int parent = (index - 1) / 2;

	Node* parent_node = m_arr->at(parent);
	int childIndex = 1 - index % 2;

	parent_node->child[childIndex] = Cur;
	Cur->getInfo()->back().is_moving = 1;
	Cur->getInfo()->back().m_coord.first = parent_node->getInfo()->back().m_coord.first;
	Cur->getInfo()->back().m_coord.second.second = parent_node->getInfo()->back().m_coord.second.second + 1;

	parent_node->getInfo()->back().m_arrowChange[childIndex] = Cur;
	Cur->par = parent_node;

	Arrange();
	Heapify(index);
}

void Heap::RemoveNode(int value) {

}

void Heap::SearchNode(int value) {

}

void Heap::Arrange() {
	CalSize(m_arr->front(), 0);
	Align(m_arr->front(), 0, - std::max(m_size[0].first, 1), std::max(m_size[0].second, 1));

	float delta = m_arr->front()->getInfo()->back().m_coord.second.first;
}

int Heap::CalSize(Node* Cur, int index) {
	if (!Cur)
		return 0;
	
	int left = CalSize(Cur->child[0], index * 2 + 1);
	int right = CalSize(Cur->child[1], index * 2 + 2);

	m_size[index] = { left, right };

	return left + right + 1;
}

void Heap::Align(Node* Cur, int index, const float& left, const float& right) {
	
	if (!Cur)
		return;

	float middle = (left + right) / 2;
	if (index == 0)
		middle = 0;

	if (!Cur->getInfo()->empty()) {
		Cur->getInfo()->back().is_moving = 1;
		Cur->getInfo()->back().m_coord.second.first = middle;
	}
	else {
		Cur->m_save.is_moving = 1;
		Cur->m_save.m_coord.second.first = middle;
	}

	Align(Cur->child[0], index * 2 + 1, left, middle);
	Align(Cur->child[1], index * 2 + 2, middle, right);
}

void Heap::Center(Node* Cur, const float& delta) {
	if (!Cur)
		return;

	if (Cur->getInfo()->empty()) {
		Cur->m_save.m_coord.second.first -= delta;
	}
	else {
		Cur->getInfo()->back().m_coord.second.first -= delta;
	}

	Center(Cur->child[0], delta);
	Center(Cur->child[1], delta);
}

void Heap::Heapify(int index) {
	int parent = (index - 1) / 2;

	if (parent == index)
		return;

	if ((*m_arr)[parent]) {
		if ((!m_mode && (*m_arr)[parent]->getValue()[0] < (*m_arr)[index]->getValue()[0]) || (m_mode && (*m_arr)[parent]->getValue()[0] < (*m_arr)[index]->getValue()[0])) {
			std::cerr << "swap: " << index << " " << parent << "\n";
			Swap(index, parent);
			Heapify(parent);
		}
	}
}

void Heap::Swap(int a, int b) {
	AddNewStep();

	Node* A = (*m_arr)[a];
	Node* B = (*m_arr)[b];

	for (int i = 0; i < 2; i++) {
		A->getInfo()->back().m_arrowChange[i] = B->getInfo()->back().m_arrowCoord[i];
		if (A->getInfo()->back().m_arrowChange[i] == A) {
			A->getInfo()->back().m_arrowChange[i] = B;
		}
		A->child[i] = A->getInfo()->back().m_arrowChange[i];

		B->getInfo()->back().m_arrowChange[i] = A->getInfo()->back().m_arrowCoord[i];
		B->child[i] = B->getInfo()->back().m_arrowChange[i];
	}

	Node* parA = (*m_arr)[(a - 1) / 2];
	Node* parB = (*m_arr)[(b - 1) / 2];

	for (int i = 0; i < 2; i++) {
		if (parA->child[i] == A) {
			parA->child[i] = B;
			parA->getInfo()->back().m_arrowChange[i] = B;
		}

		if (parB->child[i] == B) {
			parB->child[i] = A;
			parB->getInfo()->back().m_arrowChange[i] = A;
		}
	}

	A->getInfo()->back().is_moving = 1;
	B->getInfo()->back().is_moving = 1;

	A->getInfo()->back().m_coord.second = B->getInfo()->back().m_coord.first;
	B->getInfo()->back().m_coord.second = A->getInfo()->back().m_coord.first;

	(*m_arr)[a] = B;
	(*m_arr)[b] = A;
}