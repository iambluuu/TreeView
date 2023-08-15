#include "AVL_Tree.h"
#include "StateManager.h"
#include <assert.h> 


AVL_Tree::~AVL_Tree() {
	PostProcessing();
	ClearTree(m_root);
	ClearAlign();
}

Node* AVL_Tree::BuildTree(Node* Cur, int value, int hor_depth, int ver_depth) {
	if (!Cur) {
		Cur = new Node(value);

		Cur->m_save.is_moving = 1;
		Cur->m_save.m_coord.first = {ver_depth, hor_depth};
		Cur->m_save.m_coord.second = {ver_depth, hor_depth};
		Cur->m_save.is_visible = 1;
		Cur->m_save.is_appearing = 1;

		if (m_root) {
			if (value < m_root->m_save.m_shownValue[0])
				m_leftWidth++;
			else if (value > m_root->m_save.m_shownValue[0])
				m_rightWidth++;
		}


		if (m_align[ver_depth + ALIGN_OFFSET]) {

			if (value > m_align[ver_depth + ALIGN_OFFSET]->m_save.m_shownValue[0]) {
				if (value > m_root->m_save.m_shownValue[0]) {
					std::cerr << "Value collision: " << value << " " << m_align[ver_depth + ALIGN_OFFSET]->m_save.m_shownValue[0] << "\n";
					std::cerr << "Collision at: " << ver_depth << "\n";
					ShiftRightFrom(ver_depth + 1);
					m_align[ver_depth + ALIGN_OFFSET + 1] = Cur;
					Cur->m_save.m_coord.second.first = ver_depth + 1;
				}
				else {
					ShiftLeftFrom(ver_depth);
					m_align[ver_depth + ALIGN_OFFSET] = Cur;
					Cur->m_save.m_coord.second.first = ver_depth;
				}

			}
			else {
				if (value > m_root->m_save.m_shownValue[0]) {
					ShiftRightFrom(ver_depth);
					m_align[ver_depth + ALIGN_OFFSET] = Cur;
					Cur->m_save.m_coord.second.first = ver_depth;

				}
				else {
					ShiftLeftFrom(ver_depth - 1);
					m_align[ver_depth + ALIGN_OFFSET - 1] = Cur;
					Cur->m_save.m_coord.second.first = ver_depth - 1;
				}
			}
		}
		else {
			m_align[ver_depth + ALIGN_OFFSET] = Cur;
		}

		Cur->height = 1;

		return Cur;
	}

	if (value < Cur->m_save.m_shownValue[0]) {
		Cur->left = BuildTree(Cur->left, value, Cur->m_save.m_coord.second.second + 1, Cur->m_save.m_coord.second.first - 1);
		Cur->m_save.m_arrowChange[0] = Cur->left;
	}
	else if (value > Cur->m_save.m_shownValue[0]) {
		Cur->right = BuildTree(Cur->right, value, Cur->m_save.m_coord.second.second + 1, Cur->m_save.m_coord.second.first + 1);
		Cur->m_save.m_arrowChange[2] = Cur->right;
	}

	Cur->height = Cur->GetHeight();
	int balance = Cur->GetBalance();

	Cur->m_save.m_bf = balance;

	if (balance > 1 && value < Cur->left->getValue()[0]) {
		return BuildRotateRight(Cur);
	}

	if (balance < -1 && value > Cur->right->getValue()[0]) {
		return BuildRotateLeft(Cur);
	}

	if (balance > 1 && value > Cur->left->getValue()[0]) {
		Cur->left = BuildRotateLeft(Cur->left);
		Cur->m_save.m_arrowChange[0] = Cur->left;
		return BuildRotateRight(Cur);
	}

	if (balance < -1 && value < Cur->right->getValue()[0]) {
		Cur->right = BuildRotateRight(Cur->right);
		Cur->m_save.m_arrowChange[2] = Cur->right;
		return BuildRotateLeft(Cur);
	}

	return Cur;
}

Node* AVL_Tree::BuildRotateLeft(Node* Cur) {
	Node* Right = Cur->right;
	Node* RightsLeft = Right->left;

	Right->left = nullptr;
	ShiftUp(Right);

	Right->left = Cur;
	Right->m_save.m_arrowChange[0] = Right->left;
	Cur->right = nullptr;
	ShiftDown(Right->left);

	Cur->right = RightsLeft;
	Cur->m_save.m_arrowChange[2] = Cur->right;

	Cur->height = Cur->GetHeight();
	Right->height = Right->GetHeight();

	if (Cur == m_root) {
		m_rightWidth -= Right->m_save.m_coord.second.first;
		m_leftWidth += Right->m_save.m_coord.second.first;
	}

	return Right;
}

Node* AVL_Tree::BuildRotateRight(Node* Cur) {
	Node* Left = Cur->left;
	Node* LeftsRight = Left->right;

	Left->right = nullptr;
	ShiftUp(Left);

	Left->right = Cur;
	Left->m_save.m_arrowChange[2] = Left->right;
	Cur->left = nullptr;
	ShiftDown(Left->right);

	Cur->left = LeftsRight;
	Cur->m_save.m_arrowChange[0] = Cur->left;

	Cur->height = Cur->GetHeight();
	Left->height = Left->GetHeight();

	if (Cur == m_root) {
		m_leftWidth -= std::abs(Left->m_save.m_coord.second.first);
		m_rightWidth += std::abs(Left->m_save.m_coord.second.first);
	}

	return Left;
}

void AVL_Tree::BuildCentering() {
	int delta = m_root->m_save.m_coord.second.first;

	if (delta > 0) {
		for (int i = -m_leftWidth + ALIGN_OFFSET; i <= m_rightWidth + ALIGN_OFFSET; i++) {
			m_align[i] = m_align[i + delta];
			m_align[i]->m_save.is_moving = 1;
			m_align[i]->m_save.m_coord.second.first = i - ALIGN_OFFSET;
		}

		for (int i = m_rightWidth + ALIGN_OFFSET + 1; i <= m_rightWidth + ALIGN_OFFSET + delta; i++) {
			m_align[i] = nullptr;
		}

	}
	else if (delta < 0) {
		for (int i = m_rightWidth + ALIGN_OFFSET; i >= -m_leftWidth + ALIGN_OFFSET; i--) {
			m_align[i] = m_align[i + delta]; //Error something here
			m_align[i]->m_save.is_moving = 1;
			m_align[i]->m_save.m_coord.second.first = i - ALIGN_OFFSET;
		}

		for (int i = -m_leftWidth + ALIGN_OFFSET - 1; i >= -m_leftWidth + ALIGN_OFFSET + delta; i--) {
			m_align[i] = nullptr;
		}
	}
}

void AVL_Tree::OnCreate(const std::string& l_numbers, const std::string& l_value) {
	std::vector<int> values;
	if (!ValidateCreate(l_value, values)) {
		std::cerr << "Failed to create tree. Invalid input.\n";
		return;
	}

	std::cerr << "Create\n";

	ClearTree(m_root);
	ClearAlign();
	PostProcessing();
	m_leftWidth = 0;
	m_rightWidth = 0;

	std::cerr << m_align.size() << "\n";
	m_root = nullptr;

	values.resize(std::unique(values.begin(), values.end()) - values.begin());

	for (auto& value : values) {
		m_nodeNum++;
		std::cerr << value << " ";
		m_root = BuildTree(m_root, value, 0, 0);
		BuildCentering();
	}

	AddNewStep(m_root);
	
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(1);
}

void AVL_Tree::OnDestroy() {

}

void AVL_Tree::Activate() {

}

void AVL_Tree::Deactivate() {

}

void AVL_Tree::ClearAlign() {
	for (int i = 0; i < m_align.size(); i++) {
		m_align[i] = nullptr;
	}
}

void AVL_Tree::ClearTree(Node * Cur) {
	if (!Cur)
		return;

	ClearTree(Cur->left);
	Cur->left = nullptr;
	ClearTree(Cur->right);
	Cur->right = nullptr;
	delete Cur;
}

bool AVL_Tree::ValidateCreate(const std::string& l_value, std::vector<int>& res) {
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


bool AVL_Tree::ValidateInput(const std::string& l_value, std::vector<int>& res) {
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

void AVL_Tree::HandleEvent(sf::Event* l_event) {

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->HandleEvent(l_event);
}


void AVL_Tree::Update(const sf::Time& l_time) {

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->Update(l_time.asMilliseconds());
}

void AVL_Tree::Draw() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	
	renderer->DrawTree(m_root);
	renderer->DrawTree(m_removedNode);
}

void AVL_Tree::ResetNodes(Node* Cur) {
	if (!Cur)
		return;

	Cur->SaveState();
	Cur->getInfo()->clear();

	ResetNodes(Cur->left);
	ResetNodes(Cur->right);
}

void AVL_Tree::AddNewStep(Node* Cur) {
	if (!Cur)
		return;

	auto CurInfo = Cur->getInfo();

	if (CurInfo->empty()) {
		CurInfo->push_back(Cur->m_save);
		
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
			clone.m_shownValue[clone.m_valueChange.first] = clone.m_valueChange.second;
		}

		for (int i = 0; i < 3; i++) {
			clone.m_arrowCoord[i] = clone.m_arrowChange[i];
		}

		CurInfo->push_back(clone);
	}

	AddNewStep(Cur->left);
	AddNewStep(Cur->right);
}

void AVL_Tree::Centering() {
	if (m_nodeNum == 0) {
		return;
	}

	int delta = m_root->getInfo()->back().m_coord.first.first;
	std::cerr << m_leftWidth << " " << m_rightWidth << " " << delta << std::endl;

	if (delta > 0) {
		AddNewStep(m_root);
		for (int i = -m_leftWidth + ALIGN_OFFSET; i <= m_rightWidth + ALIGN_OFFSET; i++) {
			std::cerr << i << " " << i + delta << std::endl;
			m_align[i] = m_align[i + delta];
			m_align[i]->getInfo()->back().is_moving = 1;
			m_align[i]->getInfo()->back().m_coord.second.first = i - ALIGN_OFFSET;
		}

		for (int i = m_rightWidth + ALIGN_OFFSET + 1; i <= m_rightWidth + ALIGN_OFFSET + delta; i++) {
			m_align[i] = nullptr;
		}

	}
	else if (delta < 0) {
		AddNewStep(m_root);

		for (int i = m_rightWidth + ALIGN_OFFSET; i >= -m_leftWidth + ALIGN_OFFSET; i--) {
			std::cerr << i << " " << i + delta << std::endl;
			m_align[i] = m_align[i + delta]; //Error something here
			m_align[i]->getInfo()->back().is_moving = 1;
			m_align[i]->getInfo()->back().m_coord.second.first = i - ALIGN_OFFSET;
		}

		for (int i = -m_leftWidth + ALIGN_OFFSET - 1; i >= -m_leftWidth + ALIGN_OFFSET + delta; i--) {
			m_align[i] = nullptr;
		}
	}
}

void AVL_Tree::ShiftRightFrom(int value) {
	int index = value + ALIGN_OFFSET;

	while (m_align[index]) {
		index++;
	}

	for (int i = index; i > value + ALIGN_OFFSET; i--) {
		m_align[i] = m_align[i - 1];

		if (m_align[i]->getInfo()->empty()) {
			m_align[i]->m_save.is_moving = 1;
			m_align[i]->m_save.m_coord.second.first++;

			continue;
		}

		m_align[i]->getInfo()->back().is_moving = 1;
		m_align[i]->getInfo()->back().m_coord.second.first++;
	}

	m_align[value + ALIGN_OFFSET] = nullptr;
}

void AVL_Tree::ShiftLeftFrom(int value) {
	// Shift left all value lesser than value

	int index = value + ALIGN_OFFSET;

	while (m_align[index]) {
		index--;
	}

	for (int i = index; i < value + ALIGN_OFFSET; i++) {
		m_align[i] = m_align[i + 1];

		if (m_align[i]->getInfo()->empty()) {
			m_align[i]->m_save.is_moving = 1;
			m_align[i]->m_save.m_coord.second.first--;

			continue;
		}

		m_align[i]->getInfo()->back().is_moving = 1;
		m_align[i]->getInfo()->back().m_coord.second.first--;
	}

	m_align[value + ALIGN_OFFSET] = nullptr;
}

void AVL_Tree::ShiftRightTo(int value) {
	int index = value + ALIGN_OFFSET;

	while (index > 0 && m_align[index - 1]) {
		m_align[index] = m_align[index - 1];

		if (m_align[index]->getInfo()->empty()) {
			m_align[index]->m_save.is_moving = 1;
			m_align[index]->m_save.m_coord.second.first++;
		}
		else {
			m_align[index]->getInfo()->back().is_moving = 1;
			m_align[index]->getInfo()->back().m_coord.second.first++;
		}
		index--;
	}

	m_align[index] = nullptr;
}

void AVL_Tree::ShiftLeftTo(int value) {
	int index = value + ALIGN_OFFSET;

	while (index < MAX_WIDTH + ALIGN_OFFSET - 1 && m_align[index + 1]) {
		m_align[index] = m_align[index + 1];

		if (m_align[index]->getInfo()->empty()) {
			m_align[index]->m_save.is_moving = 1;
			m_align[index]->m_save.m_coord.second.first++;
		}
		else {
			m_align[index]->getInfo()->back().is_moving = 1;
			m_align[index]->getInfo()->back().m_coord.second.first--;
		}

		index++;
	}

	m_align[index] = nullptr;
}

void AVL_Tree::ShiftUp(Node* Cur) {
	if (!Cur)
		return;

	if (Cur->getInfo()->empty()) {
		Cur->m_save.is_moving = 1;
		Cur->m_save.m_coord.second.second--;
	}
	else {
		NodeInfo* CurInfo = &Cur->getInfo()->back();
		CurInfo->is_moving = 1;
		CurInfo->m_coord.second.second--;
	}

	ShiftUp(Cur->left);
	ShiftUp(Cur->right);
}

void AVL_Tree::ShiftDown(Node* Cur) {
	if (!Cur)
		return;

	if (Cur->getInfo()->empty()) {
		Cur->m_save.is_moving = 1;
		Cur->m_save.m_coord.second.second++;
	}
	else {
		NodeInfo* CurInfo = &Cur->getInfo()->back();
		CurInfo->is_moving = 1;
		CurInfo->m_coord.second.second++;
	}
	
	ShiftDown(Cur->left);
	ShiftDown(Cur->right);
}

Node* AVL_Tree::InsertNode(Node* Cur, int value, int hor_depth, int ver_depth) {

	//std::cerr << "Inserting " << value << std::endl;
	std::cerr << "Value: " << value << "Hor: " << hor_depth << " " << "Ver: " << ver_depth << std::endl;
	
	AddNewStep(m_root);
	AddNewStep(m_newNode);
	m_newNode->getInfo()->back().is_visible = 0;
	
	if (!Cur)
		Cur = m_newNode;

	auto CurInfo = Cur->getInfo();
	assert(!CurInfo->empty()); //make sure there's a NodeInfo for the current step
	NodeInfo* CurStepInfo = &CurInfo->back();

	if (Cur == m_newNode) {

		CurStepInfo->m_coord.first = { ver_depth, hor_depth };
		CurStepInfo->m_coord.second = { ver_depth, hor_depth };
		CurStepInfo->node_state = { NodeState::Selected, NodeState::Selected };
		CurStepInfo->is_visible = 1;
		CurStepInfo->is_appearing = 1;

		m_nodeNum++;
		
		if (m_root) {
			if (value < m_root->getInfo()->back().m_shownValue[0])
				m_leftWidth++;
			else if (value > m_root->getInfo()->back().m_shownValue[0])
				m_rightWidth++;
		}


		std::cerr << "new node\n";

		if (m_align[ver_depth +	ALIGN_OFFSET]) {

			if (value > m_align[ver_depth + ALIGN_OFFSET]->getInfo()->back().m_shownValue[0]) {
				if (value > m_root->getInfo()->back().m_shownValue[0]) {
					std::cerr << ver_depth << " " << hor_depth << std::endl;

					ShiftRightFrom(ver_depth + 1);
					m_align[ver_depth + ALIGN_OFFSET + 1] = Cur;
					std::cerr << value << " " << ver_depth + ALIGN_OFFSET + 1 << std::endl;
					CurStepInfo->m_coord.first = { ver_depth + 1, hor_depth };
					CurStepInfo->m_coord.second = { ver_depth + 1, hor_depth };

				}
				else {
					ShiftLeftFrom(ver_depth);
					m_align[ver_depth + ALIGN_OFFSET] = Cur;
					std::cerr << value << " " << ver_depth + ALIGN_OFFSET << std::endl;

					CurStepInfo->m_coord.first = { ver_depth, hor_depth };
					CurStepInfo->m_coord.second = { ver_depth, hor_depth };
				}
				
			}
			else {
				if (value > m_root->getInfo()->back().m_shownValue[0]) {
					ShiftRightFrom(ver_depth);
					m_align[ver_depth + ALIGN_OFFSET] = Cur;
					std::cerr << value << " " << ver_depth + ALIGN_OFFSET << std::endl;
					CurStepInfo->m_coord.first = { ver_depth, hor_depth };
					CurStepInfo->m_coord.second = { ver_depth, hor_depth };
				}
				else {
					ShiftLeftFrom(ver_depth - 1);
					m_align[ver_depth + ALIGN_OFFSET - 1] = Cur;
					std::cerr << value << " " << ver_depth + ALIGN_OFFSET - 1 << std::endl;
					CurStepInfo->m_coord.first = { ver_depth - 1, hor_depth };
					CurStepInfo->m_coord.second = { ver_depth - 1, hor_depth };

				}
			}
		}
		else {
			m_align[ver_depth + ALIGN_OFFSET] = Cur;
			std::cerr << value << " " << ver_depth + ALIGN_OFFSET << std::endl;
		}

		Cur->height = 1;

		return Cur;
	}

	CurStepInfo->node_state.second = NodeState::Selected;
	CurStepInfo->is_stateChanging = 1;
	CurStepInfo->m_bf = Cur->GetBalance();

	int CurValue = Cur->getInfo()->back().m_shownValue[0];

	if (value < CurValue) {
		Cur->left = InsertNode(Cur->left, value, CurStepInfo->m_coord.first.second + 1, CurStepInfo->m_coord.first.first - 1);
		Cur->getInfo()->back().m_arrowChange[0] = Cur->left;
	}
	else if (value > CurValue) {
		Cur->right = InsertNode(Cur->right, value, CurStepInfo->m_coord.first.second + 1, CurStepInfo->m_coord.first.first + 1);
		Cur->getInfo()->back().m_arrowChange[2] = Cur->right;
	}
	else {
		//Case where inserted value already exists
		delete m_newNode;
		m_newNode = nullptr;
	}

	Cur->height = Cur->GetHeight();
	int balance = Cur->GetBalance();

	AddNewStep(m_root);
	CurStepInfo = &CurInfo->back();

	CurStepInfo->is_stateChanging = 1;
	CurStepInfo->node_state.second = NodeState::Selected;
	CurStepInfo->m_bf = balance;

	if (balance > 1 && value < Cur->left->getValue()[0]) {
		return RotateRight(Cur);
	}

	if (balance < -1 && value > Cur->right->getValue()[0]) {
		return RotateLeft(Cur);
	}

	if (balance > 1 && value > Cur->left->getValue()[0]) {
		Cur->left = RotateLeft(Cur->left);
		Cur->getInfo()->back().m_arrowChange[0] = Cur->left;
		return RotateRight(Cur);
	}

	if (balance < -1 && value < Cur->right->getValue()[0]) {
		Cur->right = RotateRight(Cur->right);
		Cur->getInfo()->back().m_arrowChange[2] = Cur->right;
		return RotateLeft(Cur);
	}

	return Cur;
}

Node* AVL_Tree::RemoveNode(Node* Cur, int value) {
	if (!Cur) {
		m_nodeNum++;
		return Cur;
	}

	AddNewStep(m_root);

	auto CurInfo = Cur->getInfo();
	assert(!CurInfo->empty()); //make sure there's a NodeInfo for the current step
	NodeInfo* CurStepInfo = &CurInfo->back();

	CurStepInfo->node_state.second = NodeState::Selected;
	CurStepInfo->is_stateChanging = 1;
	CurStepInfo->m_bf = Cur->GetBalance();

	int CurValue = Cur->getInfo()->back().m_shownValue[0];

	if (value > CurValue) {
		Cur->right = RemoveNode(Cur->right, value);
		Cur->getInfo()->back().m_arrowChange[2] = Cur->right;
	}
	else if (value < CurValue) {
		Cur->left = RemoveNode(Cur->left, value);
		Cur->getInfo()->back().m_arrowChange[0] = Cur->left;
	}
	else {
		if (Cur->left == nullptr || Cur->right == nullptr) {
			Node* temp = Cur->left ? Cur->left : Cur->right;

			if (value < m_root->getInfo()->back().m_shownValue[0]) {
				m_leftWidth--;
			}
			else {
				m_rightWidth--;
			}

			if (temp == nullptr) {

				AddNewStep(m_root);
				Cur->getInfo()->back().is_stateChanging = 0;
				Cur->getInfo()->back().is_appearing = 2;
				m_removedNode = Cur;

				if (Cur->getInfo()->back().m_coord.first.first < m_root->getInfo()->back().m_coord.first.first) {
					ShiftRightTo(Cur->getInfo()->back().m_coord.first.first);
				}
				else {
					ShiftLeftTo(Cur->getInfo()->back().m_coord.first.first);
				}
			
				return nullptr;
			}
			else {
				std::pair<int, int> tempCoord = CurInfo->back().m_coord.first;

				AddNewStep(m_root);
				Cur->getInfo()->back().is_stateChanging = 0;
				Cur->getInfo()->back().is_appearing = 2;
				Cur->getInfo()->back().m_arrowChange[0] = nullptr;
				Cur->getInfo()->back().m_arrowChange[2] = nullptr;
				m_removedNode = Cur;

				ShiftUp(temp);

				if (Cur->getInfo()->back().m_coord.first.first < m_root->getInfo()->back().m_coord.first.first) {
					ShiftRightTo(Cur->getInfo()->back().m_coord.first.first);
				}
				else {
					ShiftLeftTo(Cur->getInfo()->back().m_coord.first.first);
				}
				
				return temp;
			}
		}
		else {
			Node* temp = Cur->right;

			AddNewStep(m_root);

			temp->getInfo()->back().node_state.second = NodeState::Selected;
			temp->getInfo()->back().is_stateChanging = 1;

			while (temp->left != nullptr) {
				temp = temp->left;

				AddNewStep(m_root);
				Cur->getInfo()->back().is_stateChanging = 0;
				temp->getInfo()->back().is_stateChanging = 1;
				temp->getInfo()->back().node_state.second = NodeState::Selected;
			}

			AddNewStep(m_root);
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state.second = NodeState::Found;
			Cur->getInfo()->back().is_valueChanging = 1;
			Cur->getInfo()->back().m_valueChange.second = temp->getValue()[0];

			temp->getInfo()->back().is_stateChanging = 0;
			
			Cur->right = RemoveNode(Cur->right, temp->getValue()[0]);
			Cur->getInfo()->back().m_arrowChange[2] = Cur->right;

			return Cur;
		}
	}	

	Cur->height = Cur->GetHeight();
	int balance = Cur->GetBalance();

	std::cerr << "Value: " << Cur->getValue()[0] << std::endl;

	std::cerr << "Balance: " << balance << std::endl;
	std::cerr << "Height: " << Cur->height << std::endl;

	AddNewStep(m_root);
	AddNewStep(m_removedNode);
	CurStepInfo = &CurInfo->back();

	CurStepInfo->is_stateChanging = 1;
	CurStepInfo->node_state.second = NodeState::Selected;
	CurStepInfo->m_bf = balance;

	if (balance > 1 && Cur->left->GetBalance() >= 0) {
		return RotateRight(Cur);
	}

	if (balance < -1 && Cur->right->GetBalance() <= 0) {
		return RotateLeft(Cur);
	}

	if (balance > 1 && Cur->left->GetBalance() < 0) {
		Cur->left = RotateLeft(Cur->left);
		Cur->getInfo()->back().m_arrowChange[0] = Cur->left;
		return RotateRight(Cur);
	}

	if (balance < -1 && Cur->right->GetBalance() > 0) {
		Cur->right = RotateRight(Cur->right);
		Cur->getInfo()->back().m_arrowChange[2] = Cur->right;
		return RotateLeft(Cur);
	}


	return Cur;

}

void AVL_Tree::SearchNode(Node* Cur, int l_value) {
	AddNewStep(m_root);
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	if (Cur->getValue()[0] == l_value) {
		AddNewStep(m_root);
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::Found;
		return;
	}

	if (Cur->getValue()[0] > l_value) {
		if (Cur->left) {
			SearchNode(Cur->left, l_value);
		}
		else {
			AddNewStep(m_root);
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state.second = NodeState::NotFound;
			return;
		}
	} else 	if (Cur->getValue()[0] < l_value) {
		if (Cur->right) {
			SearchNode(Cur->right, l_value);
		}
		else {
			AddNewStep(m_root);
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state.second = NodeState::NotFound;
			return;
		}
	}
}

void AVL_Tree::OnInsert(const std::string& l_value) {
	std::vector<int> input;

	if (!ValidateInput(l_value, input)) {
		return;
	}
	
	if (m_nodeNum > 0 && (m_nodeNum == MAX_NODE_NUM || (l_value[0] < m_root->getInfo()->back().m_shownValue[0] && m_leftWidth == MAX_WIDTH) || (l_value[0] > m_root->getInfo()->back().m_shownValue[0] && m_rightWidth == MAX_WIDTH))) {
		return;
	}

	PostProcessing();

	m_newNode = new Node(input);
	ResetNodes(m_root);

	m_root = InsertNode(m_root, input[0], 0, 0);
	Centering();

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void AVL_Tree::OnRemove(const std::string& l_value) {
	std::vector<int> input;
	if (!ValidateInput(l_value, input)) {
		return;
	}
	
	if (m_nodeNum == 0)
		return;

	m_nodeNum--;

	PostProcessing();
	ResetNodes(m_root);

	m_root = RemoveNode(m_root, input[0]);
	Centering();


	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	if (!m_root)
		renderer->Reset(m_removedNode->getInfo()->size());
	else
		renderer->Reset(m_root->getInfo()->size());
}

void AVL_Tree::OnSearch(const std::string& l_value) {
	std::vector<int> input;
	if (!ValidateInput(l_value, input)) {
		return;
	}

	if (m_nodeNum == 0)
		return;

	PostProcessing();
	ResetNodes(m_root);

	SearchNode(m_root, input[0]);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->Reset(m_root->getInfo()->size());
}

Node* AVL_Tree::RotateLeft(Node* Cur) {
	AddNewStep(m_root);

	NodeInfo* CurInfo = &Cur->getInfo()->back();

	Node* Right = Cur->right;
	Node* RightsLeft = Right->left;

	Right->left = nullptr;
	ShiftUp(Right);

	Right->left = Cur;
	Right->getInfo()->back().m_arrowChange[0] = Right->left;
	Cur->right = nullptr;
	ShiftDown(Right->left);
	
	Cur->right = RightsLeft;
	Cur->getInfo()->back().m_arrowChange[2] = Cur->right;

	Cur->height = Cur->GetHeight();
	Right->height = Right->GetHeight();

	if (Cur == m_root) {
		m_rightWidth -= Right->getInfo()->back().m_coord.first.first;
		m_leftWidth += Right->getInfo()->back().m_coord.first.first;
	}

	Right->getInfo()->back().is_stateChanging = 1;
	Right->getInfo()->back().node_state.second = NodeState::Selected;

	return Right;
}

Node* AVL_Tree::RotateRight(Node* Cur) {
	AddNewStep(m_root);

	NodeInfo* CurInfo = &Cur->getInfo()->back();

	Node* Left = Cur->left;
	Node* LeftsRight = Left->right;

	Left->right = nullptr;
	ShiftUp(Left);

	Left->right = Cur;
	Left->getInfo()->back().m_arrowChange[2] = Left->right;
	Cur->left = nullptr;
	ShiftDown(Left->right);

	Cur->left = LeftsRight;
	Cur->getInfo()->back().m_arrowChange[0] = Cur->left;

	Cur->height = Cur->GetHeight();
	Left->height = Left->GetHeight();

	if (Cur == m_root) {
		m_leftWidth -= std::abs(Left->getInfo()->back().m_coord.first.first);
		m_rightWidth += std::abs(Left->getInfo()->back().m_coord.first.first);
	}

	Left->getInfo()->back().is_stateChanging = 1;
	Left->getInfo()->back().node_state.second = NodeState::Selected;

	return Left;
}

void AVL_Tree::PostProcessing() {
	delete m_removedNode;
	m_removedNode = nullptr;
	m_newNode = nullptr;
}

void AVL_Tree::TestFunc() {
	std::cerr << "it works\n";
}