#include "AVL_Tree.h"
#include "StateManager.h"
#include <assert.h> 


AVL_Tree::~AVL_Tree() {

}

void AVL_Tree::OnCreate() {

}

void AVL_Tree::OnDestroy() {

}

void AVL_Tree::Activate() {

}

void AVL_Tree::Deactivate() {

}



void AVL_Tree::Update(const sf::Time& l_time) {

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->Update(l_time.asMilliseconds());
}

void AVL_Tree::Draw() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	
	renderer->DrawTree(m_root);

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
		clone.is_appearing = 0;
		clone.is_expanding = 0;

		if (clone.is_moving) {
			clone.is_moving = 0;
			clone.m_coord.first = clone.m_coord.second;
		}

		if (clone.is_stateChanging) {
			clone.is_stateChanging = 0;
			clone.node_state.first = clone.node_state.second;
		}

		if (clone.node_state.first == NodeState::Selected) {
			clone.is_stateChanging = 1;
			clone.node_state.second = NodeState::Visited;
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

void AVL_Tree::ShiftRight(int value) {
	int index = value + ALIGN_OFFSET;

	while (m_align[index]) {
		index++;
	}

	for (int i = index; i > value + ALIGN_OFFSET; i--) {
		m_align[i] = m_align[i - 1];
		m_align[i]->getInfo()->back().is_moving = 1;
		m_align[i]->getInfo()->back().m_coord.second.first = m_align[i]->getInfo()->back().m_coord.first.first + 1;
	}
}

void AVL_Tree::ShiftLeft(int value) {
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

	std::cerr << "Inserting " << value << std::endl;
	
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

		std::cerr << "new node\n";

		if (m_align[ver_depth +	ALIGN_OFFSET]) {

			if (CurStepInfo->m_shownValue[0] > m_align[ver_depth + ALIGN_OFFSET]->getInfo()->back().m_shownValue[0]) {
				ShiftRight(ver_depth + 1);
				m_align[ver_depth + ALIGN_OFFSET + 1] = Cur;
				CurStepInfo->m_coord.first = { ver_depth + 1, hor_depth };
				CurStepInfo->m_coord.second = { ver_depth + 1, hor_depth };
			}
			else {
				ShiftLeft(ver_depth - 1);
				m_align[ver_depth + ALIGN_OFFSET - 1] = Cur;
				CurStepInfo->m_coord.first = { ver_depth - 1, hor_depth };
				CurStepInfo->m_coord.second = { ver_depth - 1, hor_depth };
			}
		}
		else {
			m_align[ver_depth + ALIGN_OFFSET] = Cur;
		}

		return Cur;
	}
	
	CurStepInfo->node_state.second = NodeState::Selected;
	CurStepInfo->is_stateChanging = 1;

	int CurValue = Cur->getInfo()->back().m_shownValue[0];

	if (value < CurValue) {
		Cur->left = InsertNode(Cur->left, value, CurInfo->back().m_coord.first.second + 1, CurInfo->back().m_coord.first.first - 1);
	}
	else if (value > CurValue) {
		Cur->right = InsertNode(Cur->right, value, CurInfo->back().m_coord.first.second + 1, CurInfo->back().m_coord.first.first + 1);
	}
	else {
		//Case where inserted value already exists
	}

	Cur->height = Cur->GetHeight();

	return Cur;
}

void AVL_Tree::OnGenerate() {
	 //Subject to change, should get value from text box;
	std::vector<int> input;

	for (auto value : input) {
		m_root = InsertNode(m_root, value, 0, 0);
	}
}

void AVL_Tree::OnInsert(const std::vector<int>& l_value) {
	m_newNode = new Node(l_value);
	ResetNodes(m_root);

	m_root = InsertNode(m_root, l_value[0], 0, 0);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
	
}

void AVL_Tree::RemoveNode(Node* Cur, int value) {

}

void AVL_Tree::RotateLeft() {

}

void AVL_Tree::RotateRight() {

}

void AVL_Tree::PostProcessing() {
	delete m_removedNode;
	m_removedNode = nullptr;
	m_newNode = nullptr;
}

void AVL_Tree::TestFunc() {
	std::cerr << "it works\n";
}