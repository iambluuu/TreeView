#include "AVL_Tree.h"
#include <assert.h> 

void AVL_Tree::Update(const sf::Time& l_time) {

}

void AVL_Tree::Draw() {

}

void AVL_Tree::AddNewStep(Node* Cur) {
	if (!Cur)
		return;

	auto CurInfo = Cur->getInfo();

	if (CurInfo->empty()) {
		CurInfo->push_back(DEFAULT_NODE_INFO);
	}
	else {
		NodeInfo clone = CurInfo->back();
		CurInfo->push_back(clone);
	}

	AddNewStep(Cur->getLink(NodeLink::NLeft));
	AddNewStep(Cur->getLink(NodeLink::NRight));
}

void AVL_Tree::ClearStep(Node* Cur) {
	if (!Cur)
		return;

	Cur->getInfo()->clear();
	ClearStep(Cur->getLink(NodeLink::NLeft));
	ClearStep(Cur->getLink(NodeLink::NRight));
}

void AVL_Tree::Generate(bool isRandom) {
	int treeSize = 0;
	if (isRandom) {
		treeSize = rand() % 10 + 1;
	}
	else {
		treeSize = 5; //Subject to change
	}


}

Node* AVL_Tree::InsertNode(Node* Cur, int value) {
	AddNewStep(m_root);

	auto CurInfo = Cur->getInfo();
	assert(!CurInfo->empty()); //make sure there's a NodeInfo for the current step
	NodeInfo* CurStepInfo = &CurInfo->back();

	if (!Cur) {
		CurStepInfo->isVisible = 1;
		return m_newNode;
	}
	
	CurStepInfo->isSelected = 1;
	CurStepInfo->isColorChanging = 1;

	int CurValue = Cur->getValue(NodeLink::NLeft);

	if (value < CurValue) {
		Cur->setLink(InsertNode(Cur->getLink(NodeLink::NLeft), value), NodeLink::NLeft);
	}
	else if (value > CurValue) {
		Cur->setLink(InsertNode(Cur->getLink(NodeLink::NRight), value), NodeLink::NRight);
	}
	else {
		//Case where inserted value already exists
	}


	return Cur;
}

void AVL_Tree::OnInsert() {
	std::vector<int> value(3, 0); //Subject to change, it should get value from text box
	m_newNode = new Node(value);

	InsertNode(m_root, value[0]);
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