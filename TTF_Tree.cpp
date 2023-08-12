#include "TTF_Tree.h"


void Centering();

bool TTF_Tree::ValidateInput(const std::string& l_value, int& resValue) {
	if (l_value.empty() || l_value.size() > 3)
		return false;

	for (int i = 0; i < l_value.size(); i++) {
		if (!isdigit(l_value[i]))
			return false;
	}

	resValue = std::stoi(l_value);
	return true;
}

bool TTF_Tree::ValidateCreate(const std::string& l_value, std::vector<int>& res) {
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

void TTF_Tree::AddNewStep(Node* Cur) {
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
			clone.m_arrowCoord[i] = clone.m_arrowChange[i];
		}

		CurInfo->push_back(clone);
	}

	for (int i = 0; i < 4; i++) {
		AddNewStep(Cur->child[i]);
	}
}

void TTF_Tree::ResetNodes(Node* Cur) {
	if (!Cur)
		return;

	Cur->SaveState();
	Cur->getInfo()->clear();

	for (int i = 0; i < 4; i++) {
		AddNewStep(Cur->child[i]);
	}
}


void TTF_Tree::HandleEvent(sf::Event* l_event) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->HandleEvent(l_event);
}

void TTF_Tree::Update(const sf::Time& l_time) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->Update(l_time.asMilliseconds());
}

void TTF_Tree::Draw() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->DrawTree(m_root);
	renderer->DrawTree(m_removedNode);
}

void TTF_Tree::OnCreate(const std::string& l_numbers, const std::string& l_value) {

}

void TTF_Tree::OnInsert(const std::string& l_value) {
	int value = 0;
	if (!ValidateInput(l_value, value))
		return;

	ResetNodes(m_root);
	m_nodeNum++;
	InsertNode(m_root, value);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void TTF_Tree::OnRemove(const std::string& l_value) {

}

void TTF_Tree::OnSearch(const std::string& l_value) {

}

void TTF_Tree::InsertNode(Node* Cur, int value) {
	if (!Cur) {
		Cur = new Node(value);
		AddNewStep(Cur);

		Cur->getInfo()->back().m_coord.first = { 0, 0 };
		Cur->getInfo()->back().m_coord.second = { 0, 0 };
		Cur->getInfo()->back().node_state = { NodeState::Selected, NodeState::Selected };
		Cur->getInfo()->back().is_visible = 1;
		Cur->getInfo()->back().is_appearing = 1;

		m_align.push_back(std::vector<Node*>());
		m_align.back().push_back(Cur);

		return;
	}

	AddNewStep(m_root);
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	if (Cur->getInfo()->back().value_num == 3) {
		AddNewStep(m_root);
		SplitNode(Cur);
		Aligning();

		InsertNode(Cur->par, value);
	}
	else if (!Cur->child[0]) {
		ExpandNode(Cur, value);
		Aligning();
	}
	else if (Cur->getInfo()->back().value_num == 1) {
		if (value < Cur->getInfo()->back().m_shownValue[0]) {
			InsertNode(Cur->child[0], value);
		}
		else {
			InsertNode(Cur->child[1], value);
		}
	}
	else {
		if (value < Cur->getInfo()->back().m_shownValue[0]) {
			InsertNode(Cur->child[0], value);
		}
		else if (value < Cur->getInfo()->back().m_shownValue[1]) {
			InsertNode(Cur->child[1], value);
		}
		else {
			InsertNode(Cur->child[2], value);
		}
	}

}

Node* TTF_Tree::RemoveNode(Node* Cur, int value) {

}

void TTF_Tree::SearchNode(Node* Cur, int value) {

}

void TTF_Tree::ExpandNode(Node* Cur, int value) {

	Cur->getInfo()->back().is_expanding = 1;
	Cur->getInfo()->back().m_valueChange.first = 0;
	Cur->getInfo()->back().m_valueChange.second = value;
	Cur->getInfo()->back().value_num++;

	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (value < Cur->getInfo()->back().m_shownValue[i]) {
			for (int j = Cur->getInfo()->back().value_num; j > i; j--) {
				Cur->getInfo()->back().m_shownValue[i] = Cur->getInfo()->back().m_shownValue[i - 1];
			}
		}
	}
}
void TTF_Tree::CollapseNode(Node* Cur) {

}

void TTF_Tree::SplitNode(Node* Cur) {
	Cur->getInfo()->back().is_splitting = 1;

	//New Node is the right node
	Node* newNode = new Node;
	newNode->par = Cur->par;
	newNode->getInfo()->reserve(Cur->getInfo()->size());
	
	// (...) -> (.().) 
	newNode->getInfo()->back().m_coord.first = { Cur->getInfo()->back().m_coord.first.first + 1, Cur->getInfo()->back().m_coord.first.second};
	Cur->getInfo()->back().m_coord.first.first--;

	newNode->getInfo()->back().m_shownValue[0] = Cur->getInfo()->back().m_shownValue[2];

	newNode->getInfo()->back().is_visible = 1;
	newNode->getInfo()->back().is_splitting = 1;
	newNode->getInfo()->back().splitFromleft = 1;

	m_align[m_align.size() - 1 - Cur->getInfo()->back().m_coord.first.second].push_back(newNode);

	int middleValue = Cur->getInfo()->back().m_shownValue[1];

	Cur->getInfo()->back().value_num = 1;
	Cur->getInfo()->back().m_shownValue[1] = 0;
	Cur->getInfo()->back().m_shownValue[2] = 0;

	//Split children
	for (int i = 0; i < 2; i++) {
		newNode->child[i] = Cur->child[i + 2];
		newNode->getInfo()->back().m_arrowCoord[i] = newNode->child[i];

		if (newNode->child[i])
			newNode->child[i]->par = newNode;

		Cur->child[i + 2] = nullptr;
		Cur->getInfo()->back().m_arrowCoord[i + 2] = nullptr;
	}


	if (Cur->par) {
		ExpandNode(Cur->par, middleValue);
		Node* par = Cur->par;
		for (int i = 0; i < 4; i++) {
			if (par->child[i] == Cur) {
				for (int j = 3; j > i + 1; j--) {
					par->child[j] = par->child[j - 1];
					par->getInfo()->back().m_arrowChange[j] = par->child[j - 1];
				}

				par->child[i + 1] = newNode;
				par->getInfo()->back().m_arrowChange[i + 1] = newNode;
				break;
			}
		}
	}
	else {
		ShiftDown();

		m_root = new Node;
		m_root->getInfo()->reserve(Cur->getInfo()->size());
		m_root->getInfo()->back().m_coord.first = { 0, 0 };
		m_root->getInfo()->back().m_coord.second = { 0, 0 };
		m_root->getInfo()->back().m_shownValue[0] = Cur->getInfo()->back().m_shownValue[1];
		m_root->getInfo()->back().is_visible = 1;
		m_root->getInfo()->back().is_appearing = 1;

		m_align.push_back(std::vector<Node*>());
		m_align.back().push_back(m_root);

		m_root->child[0] = Cur;
		m_root->child[2] = newNode;
		m_root->getInfo()->back().m_arrowChange[0] = Cur;
		m_root->getInfo()->back().m_arrowChange[1] = newNode;
	}
}

void TTF_Tree::Aligning() {
	int maxSize = 0;

	for (int i = 0; i < m_align[0].size(); i++) {
		maxSize += m_align[0][i]->getInfo()->back().value_num;
	}

	maxSize = std::max(5, maxSize);
	int left = - maxSize / 2;

	for (int i = 0; i < m_align.size(); i++) {
		int CurSize = 0;
		for (int j = 0; j < m_align[i].size(); j++) {
			CurSize += m_align[i][j]->getInfo()->back().value_num;
		}

		for (int j = 0; j < m_align[i].size(); j++) {
			float slotSize = m_align[i][j]->getInfo()->back().value_num * maxSize / (float)CurSize;
			m_align[i][j]->getInfo()->back().is_moving = 1;
			m_align[i][j]->getInfo()->back().m_coord.second.first = left + slotSize / 2;
			
			left += slotSize;
		}

		left = - maxSize / 2;
	}
}

void TTF_Tree::ShiftDown() {
	for (int i = 0; i < m_align.size(); i++) {
		for (int j = 0; j < m_align[i].size(); j++) {
			m_align[i][j]->getInfo()->back().is_moving = 1;
			m_align[i][j]->getInfo()->back().m_coord.second.second += 1;
		}
	}
}