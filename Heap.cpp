#include "Heap.h"

Heap::Heap(StateManager* l_stateManager) : BaseState(l_stateManager) {
	m_arr = &m_maxHeap;
}

Heap::~Heap() {
	//delete m_root;
	ClearTree();
}

void Heap::Activate() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	if (!m_arr->empty())
		renderer->Reset(m_arr->front()->getInfo()->size());
	else
		renderer->Reset(0);

	codeWindow->Clear();
	renderer->OnSkipForward();
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

bool Heap::ValidateIndex(const std::string& l_value, int& index) {
	if (l_value.empty()) {
		return false;
	}

	for (int i = 0; i < l_value.size(); i++) {
		if (l_value[i] < '0' || l_value[i] > '9') {
			return false;
		}
	}
	
	index = std::stoi(l_value);

	if (index < 0 || index >= (*m_arr).size()) {
		return false;
	}

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

	(*m_arr).clear();
}

void Heap::PostProcessing() {
	if (m_removed) {
		delete m_removed;
		m_removed = nullptr;
		m_arr->pop_back();
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	codeWindow->Reset();

	ResetNodes();
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

void Heap::OnCreate(const std::string& l_value) {
	std::vector<int> value;

	if (!ValidateCreate(l_value, value)) {
		std::cerr << "Invalid input\n";
		return;
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//SET UP CODE WINDOW/////////////////////////
	std::vector<std::string> code = std::vector<std::string>();

	codeWindow->LoadCode(code);

	/////////////////////////////////////////////

	PostProcessing();

	ClearTree();

	for (int i = 0; i < value.size(); i++) {
		BuildTree(value[i]);
	}

	Arrange();
	AddNewStep();

	renderer->Reset(m_arr->front()->getInfo()->size());
}

void Heap::OnInsert(const std::string& l_value) {
	std::vector<int> value;
	if (!ValidateInput(l_value, value)) {
		std::cerr << "Invalid input\n";
		return;
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//SET UP CODE WINDOW/////////////////////////
	std::vector<std::string> code;

	if (m_mode == 0) {
		code = {
			"Heap[Heap.size] = value",
			"i = Heap.size - 1",
			"while i > 0 && Heap[i] > Heap[parent(i)]",
			"	swap(Heap[i], Heap[parent(i)])",
			"	i = parent(i)"
		};
	}
	else {
		code = {
			"Heap[Heap.size] = value",
			"i = Heap.size - 1",
			"while i > 0 && Heap[i] < Heap[parent(i)]",
			"	swap(Heap[i], Heap[parent(i)])",
			"	i = parent(i)"
		};
	}
	codeWindow->LoadCode(code);

	/////////////////////////////////////////////

	PostProcessing();

	InsertNode(value[0]);

	renderer->Reset((*m_arr)[0]->getInfo()->size());
}

void Heap::OnRemove(const std::string& l_value) {
	int index = 0;
	if (!ValidateIndex(l_value, index)) {
		std::cerr << "Invalid input\n";
		return;
	}

	if (index >= m_arr->size()) {
		std::cerr << "Index out of range\n";
		return;
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//SET UP CODE WINDOW/////////////////////////
	std::vector<std::string> code;

	if (m_mode == 0) {
		code = {
			"swap( Heap[Heap.size - 1], Heap[key] )",
			"Remove( Heap[Heap.size - 1] )",
			"while i < Heap.size:",
			"	if Heap[i] < Heap[left(i)]:",
			"		swap(Heap[i], Heap[left(i)])",
			"		i = left(i)",
			"	else if Heap[i] < Heap[right(i)]:",
			"		swap(Heap[i], Heap[right(i)])",
			"		i = right(i)"
		};
	}
	else {
		code = {
			"swap( Heap[Heap.size - 1], Heap[key] )",
			"Remove( Heap[Heap.size - 1] )",
			"while i < Heap.size:",
			"	if Heap[i] > Heap[left(i)]:",
			"		swap(Heap[i], Heap[left(i)])",
			"		i = left(i)",
			"	else if Heap[i] > Heap[right(i)]:",
			"		swap(Heap[i], Heap[right(i)])",
			"		i = right(i)"
		};
	}
	codeWindow->LoadCode(code);

	/////////////////////////////////////////////

	PostProcessing();
	codeWindow->Stay();
	RemoveNode(index);

	renderer->Reset(m_arr->front()->getInfo()->size());
}

void Heap::OnSearch(const std::string& l_value) {

}

void Heap::OnGetSize() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//SET UP CODE WINDOW/////////////////////////
	std::vector<std::string> code;
	code = {
		"return Heap.size//"
	};

	code[0] += std::to_string(m_arr->size());

	codeWindow->LoadCode(code);
	/////////////////////////////////////////////

	PostProcessing();

	codeWindow->MoveHighlight(0);
	if (!m_arr->empty()) {
		AddNewStep();
		m_arr->back()->getInfo()->back().is_stateChanging = 1;
		m_arr->back()->getInfo()->back().node_state.second = NodeState::Selected;
	}

	renderer->Reset(1);
}

void Heap::OnGetTop() {
	if (m_arr->empty())
		return;

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//SET UP CODE WINDOW/////////////////////////
	std::vector<std::string> code;
	code = {
		"return Heap[0]//"
	};

	codeWindow->LoadCode(code);
	/////////////////////////////////////////////

	PostProcessing();

	codeWindow->MoveHighlight(0);
	AddNewStep();
	(*m_arr)[0]->getInfo()->back().is_stateChanging = 1;
	(*m_arr)[0]->getInfo()->back().node_state.second = NodeState::Selected;

	renderer->Reset(1);
}

void Heap::BuildTree(int value) {
	Node* Cur = new Node(value);

	Cur->m_save.is_appearing = 1;
	Cur->m_save.is_visible = 1;
	m_arr->push_back(Cur);

	int index = m_arr->size() - 1;

	if (index == 0)
		return;	

	int parent = (index - 1) / 2;

	Node* parent_node = m_arr->at(parent);
	int childIndex = 1 - index % 2;

	parent_node->child[childIndex] = Cur;
	Cur->m_save.is_moving = 1;
	Cur->m_save.m_coord.first = parent_node->m_save.m_coord.first;
	Cur->m_save.m_coord.first.second = parent_node->m_save.m_coord.first.second + 1;
	Cur->m_save.m_coord.second.second = parent_node->m_save.m_coord.second.second + 1;

	parent_node->m_save.m_arrowChange[childIndex] = Cur;
	Cur->par = parent_node;

	Heapify(index);
}

void Heap::InsertNode(int value) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	Node* Cur = new Node(value);

	Cur->m_save.is_appearing = 1;
	Cur->m_save.is_visible = 1;
	Cur->m_save.node_state = std::make_pair(NodeState::Selected, NodeState::Selected);
	m_arr->push_back(Cur);

	int index = m_arr->size() - 1;
	std::cerr << m_arr->at(0)->m_save.m_shownValue[0] << "\n";
	
	AddNewStep();
	codeWindow->MoveHighlight(0);

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
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();
	
	AddNewStep();
	codeWindow->MoveHighlight(0);

	m_arr->back()->getInfo()->back().is_stateChanging = 1;
	m_arr->back()->getInfo()->back().node_state.second = NodeState::Found;

	m_arr->at(value)->getInfo()->back().is_stateChanging = 1;
	m_arr->at(value)->getInfo()->back().node_state.second = NodeState::InRemove;

	Swap(value, m_arr->size() - 1);

	AddNewStep();
	codeWindow->MoveHighlight(1);

	m_arr->back()->getInfo()->back().is_appearing = 2;
	m_removed = m_arr->back();

	int parent = (m_arr->size() - 2) / 2;

	if (parent < 0) {
		return;
	}

	m_arr->at(parent)->child[1 - (m_arr->size() - 1) % 2] = nullptr;
	m_arr->at(parent)->getInfo()->back().m_arrowChange[1 - (m_arr->size() - 1) % 2] = nullptr;

	Arrange();

	HeapifyDown(value);

}

void Heap::SearchNode(int value) {

}

void Heap::Arrange() {
	CalSize(m_arr->front(), 0);
	Align(m_arr->front(), 0, - std::max(m_size[0].first, 1), std::max(m_size[0].second, 1));
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

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	if ((*m_arr)[parent]) {
		if ((!m_mode && (*m_arr)[parent]->getValue()[0] < (*m_arr)[index]->getValue()[0]) || (m_mode && (*m_arr)[parent]->getValue()[0] > (*m_arr)[index]->getValue()[0])) {

			if ((*m_arr).front()->getInfo()->empty())
				BuildSwap(index, parent);
			else {
				codeWindow->MoveHighlight(3);
				Swap(index, parent);
			}
			Heapify(parent);
		}
	}
}

void Heap::HeapifyDown(int index) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	int next = index;
	int left = index * 2 + 1;
	int right = index * 2 + 2;

	if (left < m_arr->size() && left != m_arr->size() - 1) {
		if ((!m_mode && (*m_arr)[left]->getValue()[0] > (*m_arr)[next]->getValue()[0]) || (m_mode && (*m_arr)[left]->getValue()[0] < (*m_arr)[next]->getValue()[0])) {
			next = left;
		}
	}

	if (right < m_arr->size() && right != m_arr->size() - 1) {
		if ((!m_mode && (*m_arr)[right]->getValue()[0] > (*m_arr)[next]->getValue()[0]) || (m_mode && (*m_arr)[right]->getValue()[0] < (*m_arr)[next]->getValue()[0])) {
			next = right;
		}
	}

	if (next != index) {
		if (next == left) {
			codeWindow->MoveHighlight(4);
		}
		else {
			codeWindow->MoveHighlight(7);
		}

		Swap(index, next);
		HeapifyDown(next);
	}
}

void Heap::Swap(int a, int b) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	if (a == b)
		return;

	if (a < b)
		std::swap(a, b);

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

void Heap::BuildSwap(int a, int b) {
	if (a == b)
		return;

	if (a < b)
		std::swap(a, b);

	Node* A = (*m_arr)[a];
	Node* B = (*m_arr)[b];

	for (int i = 0; i < 2; i++) {
		Node* tmp = A->m_save.m_arrowChange[i];
		A->m_save.m_arrowChange[i] = B->m_save.m_arrowChange[i];
		if (A->m_save.m_arrowChange[i] == A) {
			A->m_save.m_arrowChange[i] = B;
		}
		A->child[i] = A->m_save.m_arrowChange[i];

		B->m_save.m_arrowChange[i] = tmp;
		B->child[i] = B->m_save.m_arrowChange[i];
	}

	Node* parA = (*m_arr)[(a - 1) / 2];
	Node* parB = (*m_arr)[(b - 1) / 2];

	for (int i = 0; i < 2; i++) {
		if (parA->child[i] == A) {
			parA->child[i] = B;
			parA->m_save.m_arrowChange[i] = B;
		}

		if (parB->child[i] == B) {
			parB->child[i] = A;
			parB->m_save.m_arrowChange[i] = A;
		}
	}

	A->m_save.is_moving = 1;
	B->m_save.is_moving = 1;

	std::swap(A->m_save.m_coord.second, B->m_save.m_coord.second);

	(*m_arr)[a] = B;
	(*m_arr)[b] = A;
}