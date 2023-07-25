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

	if (!m_isPausing) {
		float elapsed = l_time.asMilliseconds();
		if (m_isReversing)
			elapsed = -elapsed;

		renderer->Update(elapsed);
	}
}

void AVL_Tree::Draw() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	
	renderer->DrawTree(m_root);

}

void AVL_Tree::AddNewStep(Node* Cur) {
	if (!Cur)
		return;

	auto CurInfo = Cur->getInfo();

	if (CurInfo->empty()) {
		CurInfo->push_back(DEFAULT_NODE_INFO);
		CurInfo->back().m_shownValue = Cur->getValue();
		
	}
	else {
		NodeInfo clone = CurInfo->back();
		clone.is_moving = 0;
		clone.is_appearing = 0;
		clone.is_stateChanging = 0;
		clone.is_expanding = 0;
		if (clone.node_state == Selected)
			clone.node_state = Visited;

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

void AVL_Tree::ShiftOut(Node* Cur, int value) {
	NodeInfo* CurInfo = &Cur->getInfo()->back();
	
	if (value > 0) {
		if (Cur->getValue()[0]) {

		}
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
	AddNewStep(m_root);
	AddNewStep(m_newNode);
	m_newNode->getInfo()->back().is_visible = 0;
	
	if (!Cur)
		Cur = m_newNode;

	auto CurInfo = Cur->getInfo();
	assert(!CurInfo->empty()); //make sure there's a NodeInfo for the current step
	NodeInfo* CurStepInfo = &CurInfo->back();

	if (Cur == m_newNode) {
		CurStepInfo->m_coord = { ver_depth, hor_depth };
		CurStepInfo->is_visible = 1;
		CurStepInfo->is_appearing = 1;

		ShiftOut(m_root, ver_depth);

		return Cur;
	}
	
	CurStepInfo->node_state = Selected;
	CurStepInfo->is_stateChanging = 1;

	int CurValue = Cur->getValue(NodeLink::NLeft);

	if (value < CurValue) {
		Cur->left = InsertNode(Cur->left, value, CurInfo->back().m_coord.second + 1, CurInfo->back().m_coord.first);
	}
	else if (value > CurValue) {
		Cur->right = InsertNode(Cur->right, value, CurInfo->back().m_coord.first + 1, CurInfo->back().m_coord.first + 1);
	}
	else {
		//Case where inserted value already exists
	}

	Cur->height = std::max(Cur->left->height, Cur->right->height) + 1;

	return Cur;
}

void AVL_Tree::OnGenerate() {
	 //Subject to change, should get value from text box;
	std::vector<int> input;

	for (auto value : input) {
		m_root = InsertNode(m_root, value, 0, 0);
	}
}

void AVL_Tree::OnInsert() {
	std::vector<int> input = { 3, 5 }; //Subject to change, it should get value from text box
	m_newNode = new Node(input);

	InsertNode(m_root, input[0], 0, 0);
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