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

	for (int i = 0; i < 26; i++) {
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
	std::vector<int> values;
	if (!ValidateCreate(l_value, values)) {
		std::cerr << "Failed to create tree. Invalid input.\n";
		return;
	}

	PostProcessing();
	ClearTree(m_root);
	m_root = nullptr;
	ClearAlign();

	m_nodeNum = 0;

	for (int i = 0; i < values.size(); i++) {
		m_nodeNum++;
		BuildTree(m_root, values[i]);
	}

	BuildAlign(m_root);
	AddNewStep(m_root);
	Aligning();

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
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

	m_nodeNum--;
	RemoveNode(m_root, value);

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

int TTF_Tree::BuildAlign(Node* Cur) {
	if (!Cur)
		return -1;

	int height = 0;

	for (int i = 0; i < 4; i++) {
		int childHeight = BuildAlign(Cur->child[i]) + 1;
		if (childHeight > height)
			height = childHeight;
	}

	if (m_align.size() <= height) {
		m_align.push_back(std::vector<Node*>());
	}

	m_align[height].push_back(Cur);
	return height;
}

void TTF_Tree::BuildSplit(Node* Cur) {

	//New Node is the right node
	Node* newNode = new Node(Cur->m_save.m_shownValue[2]);
	newNode->par = Cur->par;

	// (...) -> (.().) 
	newNode->m_save.is_visible = 1;
	newNode->m_save.is_appearing = 1;

	newNode->m_save.m_coord.first = { 0, 0 };
	newNode->m_save.m_coord.second = { 0, 0 };

	int middleValue = Cur->m_save.m_shownValue[1];

	Cur->m_save.value_num = 1;
	Cur->m_save.m_shownValue[1] = 0;
	Cur->m_save.m_shownValue[2] = 0;

	//Split children
	for (int i = 0; i < 2; i++) {
		newNode->child[i] = Cur->child[i + 2];
		newNode->m_save.m_arrowChange[i] = newNode->child[i];

		if (newNode->child[i])
			newNode->child[i]->par = newNode;

		Cur->child[i + 2] = nullptr;
		Cur->m_save.m_arrowChange[i + 2] = nullptr;
	}


	if (Cur->par) {
		BuildExpand(Cur->par, middleValue);
		Node* par = Cur->par;
		for (int i = 0; i < 4; i++) {
			if (par->child[i] == Cur) {
				for (int j = 3; j > i + 1; j--) {
					par->child[j] = par->child[j - 1];
					par->m_save.m_arrowChange[j] = par->child[j];
				}

				par->child[i + 1] = newNode;
				par->m_save.m_arrowChange[i + 1] = newNode;
				break;
			}
		}
	}
	else {
		m_root = new Node(middleValue);

		m_root->m_save.m_coord.first = { 0, 0 };
		m_root->m_save.m_coord.second = { 0, 0 };

		m_root->m_save.is_visible = 1;
		m_root->m_save.is_appearing = 1;

		m_root->child[0] = Cur;
		m_root->child[1] = newNode;
		m_root->m_save.m_arrowChange[0] = Cur;
		m_root->m_save.m_arrowChange[1] = newNode;

		Cur->par = m_root;
		newNode->par = m_root;


	}

}

void TTF_Tree::BuildExpand(Node* Cur, int value) {
	Cur->m_save.m_shownValue[Cur->m_save.value_num] = value;
	Cur->m_save.value_num++;

	std::sort(Cur->m_save.m_shownValue.begin(), Cur->m_save.m_shownValue.begin() + Cur->m_save.value_num);
}

void TTF_Tree::BuildTree(Node* Cur, int value) {
	if (!Cur) {
		Cur = new Node(value);

		Cur->m_save.m_coord.first = { 0, 0 };
		Cur->m_save.m_coord.second = { 0, 0 };
		Cur->m_save.is_visible = 1;
		Cur->m_save.is_appearing = 1;

		m_root = Cur;
		return;
	}

	if (Cur->m_save.value_num == 3) {
		BuildSplit(Cur);

		int index = 0;
		Node* par = Cur->par;

		for (int i = 0; i < par->m_save.value_num; i++) {
			int childValue = par->m_save.m_shownValue[i];

			if (par->m_save.is_valueChanging && par->m_save.m_valueChange.first == i) {
				childValue = par->m_save.m_valueChange.second;
			}

			if (value > childValue) {
				index = i + 1;
			}
		}

		BuildTree(par->child[index], value);
	}
	else if (!Cur->child[0]) {
		BuildExpand(Cur, value);
	}
	else if (Cur->m_save.value_num == 1) {
		if (value < Cur->m_save.m_shownValue[0]) {
			BuildTree(Cur->child[0], value);
		}
		else {
			BuildTree(Cur->child[1], value);
		}
	}
	else {
		if (value < Cur->m_save.m_shownValue[0]) {
			BuildTree(Cur->child[0], value);
		}
		else if (value < Cur->m_save.m_shownValue[1]) {
			BuildTree(Cur->child[1], value);
		}
		else {
			BuildTree(Cur->child[2], value);
		}
	}
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

	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;


	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (Cur->getInfo()->back().m_shownValue[i] == value) {
			//is leaf
			if (Cur->child[0] == nullptr) {
				AddNewStep(m_root);

				if (Cur->getInfo()->back().value_num == 1) {
					Cur->getInfo()->back().is_appearing = 2;
					Cur->child[0] = nullptr;
					Cur->child[1] = nullptr;
					Cur->getInfo()->back().m_arrowChange[0] = nullptr;
					Cur->getInfo()->back().m_arrowChange[1] = nullptr;

					m_removedNode.push_back(Cur);

					return;
				}

				CollapseNode(Cur, value);
				Aligning();
				return;
			}

			//is not leaf

			//left child has more than one value
			if (Cur->child[i]->getInfo()->back().value_num > 1) {
				int predecessor = Cur->child[i]->getInfo()->back().m_shownValue[Cur->child[i]->getInfo()->back().value_num - 1];

				Node* leftBranch = Cur->child[i];
				while (leftBranch->child[leftBranch->getInfo()->back().value_num])
					leftBranch = leftBranch->child[leftBranch->getInfo()->back().value_num];

				predecessor = leftBranch->getValue()[leftBranch->getInfo()->back().value_num - 1];
					
				AddNewStep(m_root);
					
				Cur->getInfo()->back().is_stateChanging = 0;
				Cur->getInfo()->back().is_valueChanging = 1;
				Cur->getInfo()->back().m_valueChange.first = i;
				Cur->getInfo()->back().m_valueChange.second = predecessor;

				RemoveNode(Cur->child[i], predecessor);
				return;
			}

			//right child has more than one value
			if (Cur->child[i + 1]->getInfo()->back().value_num > 1) {
				int successor = Cur->child[i + 1]->getInfo()->back().m_shownValue[0];

				Node* rightBranch = Cur->child[i + 1];
				while (rightBranch->child[0])
					rightBranch = rightBranch->child[0];

				successor = rightBranch->getValue()[0];

				AddNewStep(m_root);

				Cur->getInfo()->back().is_stateChanging = 0;
				Cur->getInfo()->back().is_valueChanging = 1;
				Cur->getInfo()->back().m_valueChange.first = i;
				Cur->getInfo()->back().m_valueChange.second = successor;

				RemoveNode(Cur->child[i + 1], successor);
				return;
			}

			//both child has one value
			if (Cur->child[i]->getInfo()->back().value_num == 1 && Cur->child[i + 1]->getInfo()->back().value_num == 1) {
				Node* childLeft = Cur->child[i];
				MergeNode(childLeft);

				AddNewStep(m_root);

				RemoveNode(childLeft, value);
				return;
			}
	
			return;
		}
	}

	int next = 0;
	for (int i = 0; i < Cur->getInfo()->back().value_num; i++) {
		if (Cur->getValue()[i] < value)
			next = i + 1;
	}

	if (!Cur->child[next]) {
		AddNewStep(m_root);
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::NotFound;
		return;
	}

	Node* nextChild = Cur->child[next];
	if (Cur->child[next]->getInfo()->back().value_num == 1) {
		//Rotate
		if (next + 1 <= Cur->getInfo()->back().value_num && Cur->child[next + 1]->getInfo()->back().value_num > 1) {

			int parValue = Cur->getInfo()->back().m_shownValue[next];
			int childRightValue = Cur->child[next + 1]->getInfo()->back().m_shownValue[0];

			Node* rightNode = Cur->child[next + 1];
			Node* leftNode = Cur->child[next];

			AddNewStep(m_root);

			Cur->getInfo()->back().is_valueChanging = 1;
			Cur->getInfo()->back().m_valueChange.first = next;
			Cur->getInfo()->back().m_valueChange.second = childRightValue;

			CollapseNode(rightNode, childRightValue);
			ExpandNode(leftNode, parValue);

			//if rightNode has child, move it to leftNode
			if (rightNode->child[0]) {
				leftNode->child[2] = rightNode->child[0];
				leftNode->child[2]->par = leftNode;
				leftNode->getInfo()->back().m_arrowChange[2] = leftNode->child[2];
				leftNode->child_num++;

				//Shift rightNode's child
				for (int i = 0; i < rightNode->getInfo()->back().value_num; i++) {
					rightNode->child[i] = rightNode->child[i + 1];
					rightNode->getInfo()->back().m_arrowChange[i] = rightNode->child[i];
				}

				rightNode->child[rightNode->getInfo()->back().value_num] = nullptr;
				rightNode->getInfo()->back().m_arrowChange[rightNode->getInfo()->back().value_num] = nullptr;
			}

			Aligning();
		}
		else if (next - 1 >= 0 && Cur->child[next - 1]->getInfo()->back().value_num > 1) {
			Node* rightNode = Cur->child[next];
			Node* leftNode = Cur->child[next - 1];

			int parValue = Cur->getInfo()->back().m_shownValue[next - 1];
			int childLeftValue = leftNode->getInfo()->back().m_shownValue[leftNode->getInfo()->back().value_num - 1];

			AddNewStep(m_root);

			Cur->getInfo()->back().is_valueChanging = 1;
			Cur->getInfo()->back().m_valueChange.first = next - 1;
			Cur->getInfo()->back().m_valueChange.second = childLeftValue;

			CollapseNode(leftNode, childLeftValue);
			ExpandNode(rightNode, parValue);

			//if leftNode has child, move it to rightNode
			if (leftNode->child[0]) {
				for (int i = 2; i > 0; i--) {
					rightNode->child[i] = rightNode->child[i - 1];
					rightNode->getInfo()->back().m_arrowChange[i] = rightNode->child[i];
				}

				rightNode->child[0] = leftNode->child[leftNode->getInfo()->back().value_num];
				rightNode->child[0]->par = rightNode;
				rightNode->getInfo()->back().m_arrowChange[0] = rightNode->child[0];

				leftNode->child[leftNode->getInfo()->back().value_num] = nullptr;
				leftNode->getInfo()->back().m_arrowChange[leftNode->getInfo()->back().value_num] = nullptr;
			}

			Aligning();
		}
		else if (next - 1 >= 0) {
			nextChild = Cur->child[next - 1];
;			MergeNode(Cur->child[next - 1]);
		}
		else if (next + 1 <= Cur->getInfo()->back().value_num) {
			MergeNode(Cur->child[next]);
		}
	}

	RemoveNode(nextChild, value);

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

	for (int i = 0; i <= par->getInfo()->back().value_num; i++) {
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

	Cur->getInfo()->back().is_stateChanging = 0;

	std::vector<int> valueOfNewNode;
	int CurValue = Cur->getValue()[0];
	valueOfNewNode.push_back(rightNode->getValue()[0]);
	valueOfNewNode.push_back(Cur->getValue()[0]);
	valueOfNewNode.push_back(par->getValue()[indexOfCur]);

	std::sort(valueOfNewNode.begin(), valueOfNewNode.end());

	for (int i = 0; i < 3; i++) {
		Cur->getInfo()->back().m_shownValue[i] = valueOfNewNode[i];
		if (valueOfNewNode[i] == CurValue) {
			Cur->getInfo()->back().is_valueChanging = 1;
			Cur->getInfo()->back().m_valueChange.first = i;
			Cur->getInfo()->back().m_valueChange.second = CurValue;
		}
	}

	Cur->getInfo()->back().value_num = 3;
	Cur->getInfo()->back().is_splitting = 2;

	rightNode->getInfo()->back().is_appearing = 2;
	RemoveFromRow(rightNode, row);

	if (par->getInfo()->back().value_num > 1) {
		CollapseNode(par, par->getInfo()->back().m_shownValue[indexOfCur]);

		//Shift children of parent node/////////////////////////////////////////////
		for (int i = indexOfCur + 1; i < par->getInfo()->back().value_num; i++) {
			par->child[i] = par->child[i + 1];
			par->getInfo()->back().m_arrowChange[i] = par->child[i];
		}
		par->child[par->getInfo()->back().value_num] = nullptr;
		par->getInfo()->back().m_arrowChange[par->getInfo()->back().value_num] = nullptr;
		////////////////////////////////////////////////////////////////////////////

	}
	else {
		par->getInfo()->back().is_appearing = 2;
		par->child[0] = nullptr;
		par->child[1] = nullptr;

		par->getInfo()->back().m_arrowChange[0] = nullptr;
		par->getInfo()->back().m_arrowChange[1] = nullptr;

		Cur->par = nullptr;

		RemoveFromRow(par, row + 1);
		m_removedNode.push_back(par);

		if (par == m_root)
			m_root = Cur;

	}

	//Move children from right node to left node///////////////////////////////
	if (rightNode->child[0]) {
		Cur->child[2] = rightNode->child[0];
		Cur->child[2]->par = Cur;

		Cur->getInfo()->back().m_arrowChange[2] = rightNode->child[0];
	
		Cur->child[3] = rightNode->child[1];
		Cur->child[3]->par = Cur;

		Cur->getInfo()->back().m_arrowChange[3] = rightNode->child[1];

		rightNode->child[0] = nullptr;
		rightNode->child[1] = nullptr;
		rightNode->getInfo()->back().m_arrowChange[0] = nullptr;
		rightNode->getInfo()->back().m_arrowChange[1] = nullptr;
	}
	/////////////////////////////////////////////////////

	m_removedNode.push_back(rightNode);

	Aligning();
}

void TTF_Tree::Aligning() {
	ShiftUp();
	
	int maxSize = 0;

	for (int i = 0; i < m_align[0].size(); i++) {
		if (m_align[0][i]->getInfo()->back().is_expanding == 2)
			maxSize--;

		maxSize += m_align[0][i]->getInfo()->back().value_num;
	}

	maxSize = std::max(5, maxSize);
	float left = - maxSize / 2.f;

	for (int i = 0; i < m_align[0].size(); i++) {
		float slotSize = m_align[0][i]->getInfo()->back().value_num;

		if (m_align[0][i]->getInfo()->back().is_expanding == 2)
			slotSize--;

		m_align[0][i]->getInfo()->back().is_moving = 1;
		m_align[0][i]->getInfo()->back().m_coord.second.first = left + slotSize / 2;
		m_align[0][i]->getInfo()->back().m_coord.second.second = m_align.size() - 1;

		left += slotSize;
	}

	for (int i = 1; i < m_align.size(); i++) {
		for (int j = 0; j < m_align[i].size(); j++) {
			int valNum = m_align[i][j]->getInfo()->back().value_num;

			if (m_align[i][j]->getInfo()->back().is_expanding == 2)
				valNum--;

			float leftBound = m_align[i][j]->child[0]->getInfo()->back().m_coord.second.first;
			float rightBound = m_align[i][j]->child[valNum]->getInfo()->back().m_coord.second.first;

			m_align[i][j]->getInfo()->back().is_moving = 1;
			m_align[i][j]->getInfo()->back().m_coord.second.first = (leftBound + rightBound) / 2;
			m_align[i][j]->getInfo()->back().m_coord.second.second = m_align.size() - i - 1;
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

	for (int i = index - 1; i >= 0; i--) {
		for (int j = 0; j < m_align[i].size(); j++) {
			m_align[i][j]->getInfo()->back().is_moving = 1;
			m_align[i][j]->getInfo()->back().m_coord.second.second -= 1;
		}
	}
}