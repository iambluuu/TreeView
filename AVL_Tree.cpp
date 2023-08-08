#include "AVL_Tree.h"
#include "StateManager.h"
#include <assert.h> 


AVL_Tree::~AVL_Tree() {
	PostProcessing();
}

void AVL_Tree::OnCreate(const std::string& l_numbers, const std::string& l_value) {

}

void AVL_Tree::OnDestroy() {

}

void AVL_Tree::Activate() {

}

void AVL_Tree::Deactivate() {

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
	if (num > 999 || num < -999) {
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
	renderer->DrawNode(m_removedNode, 0);
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

	AddNewStep(Cur->left);
	AddNewStep(Cur->right);
}

void AVL_Tree::ClearStep(Node* Cur) {
	if (!Cur)
		return;

	Cur->getInfo()->clear();
	ClearStep(Cur->left);
	ClearStep(Cur->right);
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
			m_align[i] = m_align[i + delta];
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
		m_align[i]->getInfo()->back().is_moving = 1;
		m_align[i]->getInfo()->back().m_coord.second.first = m_align[i]->getInfo()->back().m_coord.first.first + 1;
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
		m_align[i]->getInfo()->back().is_moving = 1;
		m_align[i]->getInfo()->back().m_coord.second.first = m_align[i]->getInfo()->back().m_coord.first.first - 1;
	}

	m_align[value + ALIGN_OFFSET] = nullptr;
}

void AVL_Tree::ShiftRightTo(int value) {
	int index = value + ALIGN_OFFSET;

	while (index > 0 && m_align[index - 1]) {
		m_align[index] = m_align[index - 1];
		m_align[index]->getInfo()->back().is_moving = 1;
		m_align[index]->getInfo()->back().m_coord.second.first = m_align[index]->getInfo()->back().m_coord.first.first + 1;

		index--;
	}

	m_align[index] = nullptr;
}

void AVL_Tree::ShiftLeftTo(int value) {
	int index = value + ALIGN_OFFSET;

	while (index < MAX_WIDTH + ALIGN_OFFSET - 1 && m_align[index + 1]) {
		m_align[index] = m_align[index + 1];
		m_align[index]->getInfo()->back().is_moving = 1;
		m_align[index]->getInfo()->back().m_coord.second.first = m_align[index]->getInfo()->back().m_coord.first.first - 1;

		index++;
	}

	m_align[index] = nullptr;
}

void AVL_Tree::ShiftUp(Node* Cur) {
	if (!Cur)
		return;

	NodeInfo* CurInfo = &Cur->getInfo()->back();
	CurInfo->is_moving = 1;
	CurInfo->m_coord.second.second = CurInfo->m_coord.first.second - 1;

	ShiftUp(Cur->left);
	ShiftUp(Cur->right);
}

void AVL_Tree::ShiftDown(Node* Cur) {
	if (!Cur)
		return;

	NodeInfo* CurInfo = &Cur->getInfo()->back();
	CurInfo->is_moving = 1;
	CurInfo->m_coord.second.second = CurInfo->m_coord.first.second + 1;

	ShiftDown(Cur->left);
	ShiftDown(Cur->right);
}

Node* AVL_Tree::Generate(Node* Cur, int value) {
	if (!Cur) {
		Cur = new Node();

		Cur->setValue(value);
		Cur->getInfo()->push_back(DEFAULT_NODE_INFO);
		Cur->getInfo()->back().is_appearing = 1;
		Cur->getInfo()->back().m_shownValue = Cur->getValue();

		return Cur;
	}

	if (value < Cur->getValue()[0])
		Cur->left = Generate(Cur->left, value);

	if (value > Cur->getValue()[0])
		Cur->right = Generate(Cur->right, value);

	return Cur;
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
		Cur->getInfo()->back().m_arrowCoord[0] = Cur->left;
	}
	else if (value > CurValue) {
		Cur->right = InsertNode(Cur->right, value, CurStepInfo->m_coord.first.second + 1, CurStepInfo->m_coord.first.first + 1);
		Cur->getInfo()->back().m_arrowCoord[2] = Cur->right;
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
		return RotateRight(Cur);
	}

	if (balance < -1 && value < Cur->right->getValue()[0]) {
		Cur->right = RotateRight(Cur->right);
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
	}
	else if (value < CurValue) {
		Cur->left = RemoveNode(Cur->left, value);
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
				CurStepInfo->node_state.second = NodeState::Selected;
				CurStepInfo->is_stateChanging = 1;

				AddNewStep(m_root);
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

				Cur->getInfo()->back().is_appearing = 2;
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
				AddNewStep(m_root);
				CurStepInfo = &CurInfo->back();
				CurStepInfo->is_stateChanging = 0;
				CurStepInfo->node_state = { NodeState::Selected, NodeState::Selected };

				temp->getInfo()->back().is_stateChanging = 1;
				temp->getInfo()->back().node_state.second = NodeState::Selected;

				temp = temp->left;
			}

			AddNewStep(m_root);

			CurStepInfo = &CurInfo->back();

			CurStepInfo->is_valueChanging = 1;
			CurStepInfo->m_valueChange.first = Cur->getValue()[0];
			CurStepInfo->m_valueChange.second = temp->getValue()[0];

			CurStepInfo->is_stateChanging = 0;
			CurStepInfo->node_state = { NodeState::Selected, NodeState::Selected };

			Cur->right = RemoveNode(Cur->right, temp->getValue()[0]);

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

	if (balance > 1 && value < Cur->left->getValue()[0]) {
		return RotateRight(Cur);
	}

	if (balance < -1 && value > Cur->right->getValue()[0]) {
		return RotateLeft(Cur);
	}

	if (balance > 1 && value > Cur->left->getValue()[0]) {
		Cur->left = RotateLeft(Cur->left);
		return RotateRight(Cur);
	}

	if (balance < -1 && value < Cur->right->getValue()[0]) {
		Cur->right = RotateRight(Cur->right);
		return RotateLeft(Cur);
	}


	return Cur;

}

void AVL_Tree::OnGenerate() {
	 //Subject to change, should get value from text box;
	std::vector<int> input;

	for (auto value : input) {
		m_root = InsertNode(m_root, value, 0, 0);
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

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	if (!m_root)
		renderer->Reset(m_removedNode->getInfo()->size());
	else
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
	Right->getInfo()->back().m_arrowCoord[0] = Right->left;
	Cur->right = nullptr;
	ShiftDown(Right->left);
	
	Cur->right = RightsLeft;
	Cur->getInfo()->back().m_arrowCoord[2] = Cur->right;

	Cur->height = Cur->GetHeight();
	Right->height = Right->GetHeight();

	if (Cur == m_root) {
		m_rightWidth -= Right->getInfo()->back().m_coord.first.first;
		m_leftWidth += Right->getInfo()->back().m_coord.first.first;
	}

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
	Left->getInfo()->back().m_arrowCoord[2] = Left->right;
	Cur->left = nullptr;
	ShiftDown(Left->right);

	Cur->left = LeftsRight;
	Cur->getInfo()->back().m_arrowCoord[0] = Cur->left;

	Cur->height = Cur->GetHeight();
	Left->height = Left->GetHeight();

	if (Cur == m_root) {
		m_leftWidth -= std::abs(Left->getInfo()->back().m_coord.first.first);
		m_rightWidth += std::abs(Left->getInfo()->back().m_coord.first.first);
	}

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