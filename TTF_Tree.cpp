#include "TTF_Tree.h"

TTF_Tree::~TTF_Tree() {
	//delete m_root;
	PostProcessing();
	ClearTree(m_root);
	ClearAlign();
}

void TTF_Tree::Activate() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	if (m_root)
		renderer->Reset(m_root->getInfo()->size());
	else
		renderer->Reset(0);

	codeWindow->Clear();
	renderer->OnSkipForward();
}

void TTF_Tree::PostProcessing() {
	for (auto& node : m_removedNode) {
		if (node == m_root)
			m_root = nullptr;

		delete node;
	}

	m_removedNode.clear();

	ResetNodes(m_root);
}

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

		if (clone.is_expanding == 2) {
			for (int i = clone.m_valueChange.first; i < clone.value_num - 1; i++) {
				clone.m_shownValue[i] = clone.m_shownValue[i + 1];
			}
			clone.m_shownValue[clone.value_num - 1] = 0;
			clone.value_num--;
		}

		clone.is_expanding = 0;

		for (int i = 0; i < 4; i++) {
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
		ResetNodes(Cur->child[i]);
	}
}

void TTF_Tree::ClearTree(Node* Cur) {
	if (!Cur) {
		return;
	}

	for (int i = 0; i < 4; i++) {
		ClearTree(Cur->child[i]);
		Cur->child[i] = nullptr;
	}

	delete Cur;
}

void TTF_Tree::ClearAlign() {
	for (int i = 0; i < m_align.size(); i++) {
		for (int j = 0; j < m_align[i].size(); j++) {
			m_align[i][j] = nullptr;
		}

		m_align[i].clear();
	}

	m_align.clear();
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
	
	for (int i = 0; i < m_removedNode.size(); i++)
		renderer->DrawNode(m_removedNode[i], 0);
}

void TTF_Tree::OnCreate(const std::string& l_value) {

}

void TTF_Tree::OnInsert(const std::string& l_value) {
	int value = 0;
	if (!ValidateInput(l_value, value))
		return;

	PostProcessing();

	m_nodeNum++;
	InsertNode(m_root, value);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void TTF_Tree::OnRemove(const std::string& l_value) {
	int value = 0;
	if (!ValidateInput(l_value, value))
		return;

	PostProcessing();

	m_nodeNum++;
	InsertNode(m_root, value);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	if (m_root)
		renderer->Reset(m_root->getInfo()->size());
	else
		renderer->Reset(m_removedNode[0]->getInfo()->size());
}

void TTF_Tree::OnSearch(const std::string& l_value) {
	int value = 0;
	if (!ValidateInput(l_value, value))
		return;

	ResetNodes(m_root);

	SearchNode(m_root, value);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
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

		m_root = Cur;
		return;
	}

	AddNewStep(m_root);
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	if (Cur->getInfo()->back().value_num == 3) {
		AddNewStep(m_root);
		SplitNode(Cur);
		Aligning();

		Node* par = Cur->par;

		int index = 0;


		for (int i = 0; i < par->getInfo()->back().value_num; i++) {
			int childValue = par->getInfo()->back().m_shownValue[i];
				
			if (par->getInfo()->back().is_valueChanging && par->getInfo()->back().m_valueChange.first == i) {
				childValue = par->getInfo()->back().m_valueChange.second;
			}

			if (value > childValue) {
				index = i + 1;
			}
		}

		InsertNode(par->child[index], value);
	}
	else if (!Cur->child[0]) {
		AddNewStep(m_root);
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

void TTF_Tree::RemoveNode(Node* Cur, int value) {
	AddNewStep(m_root);

	auto CurInfo = Cur->getInfo()->back();

	CurInfo.is_stateChanging = 1;
	CurInfo.node_state.second = NodeState::Selected;


	for (int i = 0; i < CurInfo.value_num; i++) {
		if (Cur->getValue()[i] == value) {
			//Many value
			if (CurInfo.value_num > 1) {

				//is leaf
				if (Cur->child[0] == nullptr) {
					AddNewStep(m_root);
					CollapseNode(Cur, value);
					Aligning();
					return;
				}

				//is not leaf

				//left child has more than one value
				if (Cur->child[i]->getInfo()->back().value_num > 1) {
					int predecessor = Cur->child[i]->getInfo()->back().m_shownValue[Cur->child[i]->getInfo()->back().value_num - 1];
					
					AddNewStep(m_root);
					CollapseNode(Cur->child[i], predecessor);
					ExpandNode(Cur, predecessor);
					Aligning();
					return;
				}

				//right child has more than one value
				if (Cur->child[i + 1]->getInfo()->back().value_num > 1) {
					int successor = Cur->child[i + 1]->getInfo()->back().m_shownValue[0];

					AddNewStep(m_root);
					CollapseNode(Cur->child[i + 1], successor);
					ExpandNode(Cur, successor);
					Aligning();
					return;
				}

				//both child has one value
				if (Cur->child[i]->getInfo()->back().value_num == 1 && Cur->child[i + 1]->getInfo()->back().value_num == 1) {
					int predecessor = Cur->child[i]->getInfo()->back().m_shownValue[0];
					int successor = Cur->child[i + 1]->getInfo()->back().m_shownValue[0];

					AddNewStep(m_root);
					CollapseNode(Cur->child[i], predecessor);
					CollapseNode(Cur->child[i + 1], successor);
					ExpandNode(Cur, predecessor);
					ExpandNode(Cur, successor);
					Aligning();
					return;
				}
			}
			return;
		}
	}

	int next = 0;
	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (Cur->getValue()[i] < value)
			next = i + 1;
	}


	RemoveNode(Cur->child[next], value);

}

void TTF_Tree::SearchNode(Node* Cur, int value) {
	
	AddNewStep(m_root);
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;
	
	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (Cur->getValue()[i] == value) {
			AddNewStep(m_root);
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state.second = NodeState::Found;
			return;
		}
	}

	if (!Cur->child[0]) {
		AddNewStep(m_root);
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::NotFound;
		return;
	}

	int next = 0;

	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (Cur->getValue()[i] > value) {
			break;
		}
		next++;
	}

	SearchNode(Cur->child[next], value);
}

void TTF_Tree::ExpandNode(Node* Cur, int value) {
	Cur->getInfo()->back().is_stateChanging = 0;
	Cur->getInfo()->back().node_state.first = NodeState::Selected;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;
	Cur->getInfo()->back().is_expanding = 1;
	Cur->getInfo()->back().is_valueChanging = 1;

	//find the position of the new value
	Cur->getInfo()->back().m_valueChange.second = value;
	Cur->getInfo()->back().m_valueChange.first = 0;
	
	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (value > Cur->getInfo()->back().m_shownValue[i]) {
			Cur->getInfo()->back().m_valueChange.first = i + 1;
		}
	}

	Cur->getInfo()->back().value_num++;

	if (Cur->getInfo()->back().value_num - 1 > Cur->getInfo()->back().m_shownValue.size())
		std::cerr << "something wrong buddy\n";

	for (int i = Cur->getInfo()->back().value_num - 1; i > Cur->getInfo()->back().m_valueChange.first; i--) {
		Cur->getInfo()->back().m_shownValue[i] = Cur->getInfo()->back().m_shownValue[i - 1];
	}

	Cur->getInfo()->back().m_shownValue[Cur->getInfo()->back().m_valueChange.first] = 0;
}


void TTF_Tree::CollapseNode(Node* Cur, int value) {
	Cur->getInfo()->back().is_stateChanging = 0;
	Cur->getInfo()->back().node_state.first = NodeState::Selected;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;
	Cur->getInfo()->back().is_expanding = 2;
	Cur->getInfo()->back().is_valueChanging = 1;

	//find the position of the new value
	Cur->getInfo()->back().m_valueChange.second = 0;
	Cur->getInfo()->back().m_valueChange.first = 0;

	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (value > Cur->getInfo()->back().m_shownValue[i]) {
			Cur->getInfo()->back().m_valueChange.first = i + 1;
		}
	}

	for (int i = Cur->getInfo()->back().m_valueChange.first; i < Cur->getInfo()->back().value_num - 1; i--) {
		Cur->getInfo()->back().m_shownValue[i] = Cur->getInfo()->back().m_shownValue[i + 1];
	}
	Cur->getInfo()->back().m_shownValue[Cur->getInfo()->back().value_num - 1] = 0;
	Cur->getInfo()->back().value_num--;
}

void TTF_Tree::SplitNode(Node* Cur) {
	Cur->getInfo()->back().is_splitting = 1;

	//New Node is the right node
	Node* newNode = new Node;
	newNode->par = Cur->par;
	newNode->getInfo()->resize(Cur->getInfo()->size());
	
	// (...) -> (.().) 
	newNode->getInfo()->back().m_shownValue[0] = Cur->getInfo()->back().m_shownValue[2];
	newNode->getInfo()->back().is_visible = 1;
	newNode->getInfo()->back().is_splitting = 1;
	newNode->getInfo()->back().splitFromleft = 1;
	newNode->getInfo()->back().node_state = { NodeState::Selected, NodeState::Selected };

	newNode->getInfo()->back().m_coord.first = { Cur->getInfo()->back().m_coord.first.first + 1, Cur->getInfo()->back().m_coord.first.second};
	newNode->getInfo()->back().m_coord.second = newNode->getInfo()->back().m_coord.first;
	Cur->getInfo()->back().m_coord.first.first--;

	InsertToRow(newNode, Cur, m_align.size() - Cur->getInfo()->back().m_coord.first.second - 1);

	int middleValue = Cur->getInfo()->back().m_shownValue[1];

	Cur->getInfo()->back().value_num = 1;
	Cur->getInfo()->back().m_shownValue[1] = 0;
	Cur->getInfo()->back().m_shownValue[2] = 0;
	Cur->getInfo()->back().is_stateChanging = 0;
	Cur->getInfo()->back().is_splitting = 1;
	Cur->getInfo()->back().node_state.first = NodeState::Selected;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	//Split children
	for (int i = 0; i < 2; i++) {
		newNode->child[i] = Cur->child[i + 2];
		newNode->getInfo()->back().m_arrowChange[i] = newNode->child[i];

		if (newNode->child[i])
			newNode->child[i]->par = newNode;

		Cur->child[i + 2] = nullptr;
		Cur->getInfo()->back().m_arrowChange[i + 2] = nullptr;
	}


	if (Cur->par) {
		ExpandNode(Cur->par, middleValue);
		Node* par = Cur->par;
		for (int i = 0; i < 4; i++) {
			if (par->child[i] == Cur) {
				for (int j = 3; j > i + 1; j--) {
					par->child[j] = par->child[j - 1];
					par->getInfo()->back().m_arrowChange[j] = par->child[j];
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
		m_root->getInfo()->resize(Cur->getInfo()->size());
		m_root->getInfo()->back().m_coord.first = { 0, 0 };
		m_root->getInfo()->back().m_coord.second = { 0, 0 };
		m_root->getInfo()->back().is_valueChanging = 1;
		m_root->getInfo()->back().m_valueChange.first = 0;
		m_root->getInfo()->back().m_valueChange.second = middleValue;

		m_root->getInfo()->back().is_visible = 1;
		m_root->getInfo()->back().is_appearing = 1;

		m_root->getInfo()->back().is_stateChanging = 1;
		m_root->getInfo()->back().node_state.first = NodeState::Selected;
		m_root->getInfo()->back().node_state.second = NodeState::Selected;

		m_align.push_back(std::vector<Node*>());
		m_align.back().push_back(m_root);

		m_root->child[0] = Cur;
		m_root->child[1] = newNode;
		m_root->getInfo()->back().m_arrowChange[0] = Cur;
		m_root->getInfo()->back().m_arrowChange[1] = newNode;

		Cur->par = m_root;
		newNode->par = m_root;

		std::cerr << "New root is created\n";
		std::cerr << "New root step size: " << m_root->getInfo()->size() << "\n";
	}
}

void TTF_Tree::MergeNode(Node* Cur) {
	//Cur is left node, we merge right node to left node

	Node* par = Cur->par;

	int row = m_align.size() - Cur->getInfo()->back().m_coord.first.second - 1;
	int indexOfCur = 0;

	for (int i = 0; i < par->child_num; i++) {
		if (par->child[i] == Cur) {
			indexOfCur = i;
			break;
		}
	}

	Node* rightNode = par->child[indexOfCur + 1];

	AddNewStep(m_root);
	
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	rightNode->getInfo()->back().is_stateChanging = 1;
	rightNode->getInfo()->back().node_state.second = NodeState::Selected;

	AddNewStep(m_root);

	ExpandNode(Cur, rightNode->getValue()[0]);
	rightNode->getInfo()->back().value_num--;
	rightNode->getInfo()->back().is_appearing = 2;

	RemoveFromRow(rightNode, row);

	for (int i = indexOfCur + 1; i < par->child_num - 1; i++) {
		par->child[i] = par->child[i + 1];
		par->getInfo()->back().m_arrowChange[i] = par->child[i];
	}
	par->child[par->child_num - 1] = nullptr;
	par->getInfo()->back().m_arrowChange[par->child_num - 1] = nullptr;
	par->child_num--;

	if (rightNode->child[0]) {
		Cur->child_num++;
		Cur->child[Cur->child_num - 1] = rightNode->child[0];
		Cur->getInfo()->back().m_arrowChange[Cur->child_num - 1] = rightNode->child[0];
		rightNode->child[0] = nullptr;
		rightNode->getInfo()->back().m_arrowChange[0] = nullptr;
	}

	if (rightNode->child[1]) {
		Cur->child_num++;
		Cur->child[Cur->child_num - 1] = rightNode->child[1];
		Cur->getInfo()->back().m_arrowChange[Cur->child_num - 1] = rightNode->child[1];
		rightNode->child[1] = nullptr;
		rightNode->getInfo()->back().m_arrowChange[1] = nullptr;
	}

	m_removedNode.push_back(rightNode);
	Aligning();
	//push par value down
	AddNewStep(m_root);

	int parValue = par->getValue()[indexOfCur];
	ExpandNode(Cur, parValue);

	if (par->getInfo()->back().value_num == 1) {
		par->getInfo()->back().is_appearing = 2;
		par->getInfo()->back().value_num--;
		RemoveFromRow(par, row - 1);

		Cur->par = nullptr;
		par->child[0] = nullptr;
		par->getInfo()->back().m_arrowChange[0] = nullptr;

		m_removedNode.push_back(par);
		if (par == m_root)
			m_root = Cur;
	}
	else {
		CollapseNode(par, parValue);
	}

	Aligning();
}

void TTF_Tree::Aligning() {
	ShiftUp();
	
	int maxSize = 0;

	for (int i = 0; i < m_align[0].size(); i++) {
		maxSize += m_align[0][i]->getInfo()->back().value_num;
	}

	maxSize = std::max(5, maxSize);
	float left = - maxSize / 2.f;

	for (int i = 0; i < m_align[0].size(); i++) {
		float slotSize = m_align[0][i]->getInfo()->back().value_num;
		m_align[0][i]->getInfo()->back().is_moving = 1;
		m_align[0][i]->getInfo()->back().m_coord.second.first = left + slotSize / 2;

		left += slotSize;
	}

	for (int i = 1; i < m_align.size(); i++) {
		for (int j = 0; j < m_align[i].size(); j++) {
			int valNum = m_align[i][j]->getInfo()->back().value_num;
			int leftBound = m_align[i][j]->child[0]->getInfo()->back().m_coord.second.first;
			int rightBound = m_align[i][j]->child[valNum]->getInfo()->back().m_coord.second.first;

			m_align[i][j]->getInfo()->back().is_moving = 1;
			m_align[i][j]->getInfo()->back().m_coord.second.first = (leftBound + rightBound) / 2;
		}
		
	}
}

void TTF_Tree::InsertToRow(Node* Cur, Node* LeftOfCur, int row) {
	if (!Cur)
		return;

	m_align[row].push_back(m_align[row].back());

	int i = m_align[row].size() - 1;

	while (m_align[row][i - 1] != LeftOfCur) {
		m_align[row][i] = m_align[row][i - 1];
		i--;
	}

	m_align[row][i] = Cur;
}

void TTF_Tree::RemoveFromRow(Node* Cur, int row) {
	if (!Cur)
		return;

	int index = -1;
	for (int i = 0; i < m_align[row].size(); i++) {
		if (m_align[row][i] == Cur) {
			index = i;
			break;
		}
	}

	if (index == -1)
		return;

	for (int i = index; i < m_align[row].size() - 1; i++) {
		m_align[row][i] = m_align[row][i + 1];
	}

	m_align[row].pop_back();
}

void TTF_Tree::ShiftDown() {
	for (int i = 0; i < m_align.size(); i++) {
		for (int j = 0; j < m_align[i].size(); j++) {
			m_align[i][j]->getInfo()->back().is_moving = 1;
			m_align[i][j]->getInfo()->back().m_coord.second.second += 1;
		}
	}
}

void TTF_Tree::ShiftUp() {
	int index = -1;
	for (int i = 0; i < m_align.size(); i++) {
		if (m_align[i].empty()) {
			index = i;
			break;
		}
	}

	if (index == -1)
		return;

	for (int i = index; i < m_align.size() - 1; i++) {
		m_align[i] = m_align[i + 1];
	}

	m_align.pop_back();

	for (int i = index - 1; i > 0; i++) {
		for (int j = 0; j < m_align[i].size(); j++) {
			m_align[i][j]->getInfo()->back().is_moving = 1;
			m_align[i][j]->getInfo()->back().m_coord.second.second -= 1;
		}
	}
}