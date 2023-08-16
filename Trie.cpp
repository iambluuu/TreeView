#include "Trie.h"

Trie::~Trie() {
	//delete m_root;
	ClearTree(m_root);
}

bool Trie::ValidateInput(const std::string& l_value) {
	if (l_value.empty())
		return false;

	for (int i = 0; i < l_value.size(); i++) {
		if (l_value[i] < 'a' && l_value[i] > 'z' && l_value[i] < 'A' && l_value[i] > 'Z')
			return false;
	}

	return true;
}

bool Trie::ValidateCreate(const std::string& l_value, std::vector<std::string>& res) {
	std::string soFar = "";

	if (l_value.empty()) {
		std::cerr << "Empty input\n";
		return false;
	}

	for (int i = 0; i < l_value.size(); i++) {
		if (l_value[i] == ',' || l_value[i] == ' ') {
			if (soFar != "") {
				res.push_back(soFar);
				soFar.clear();
			}
		}
		else if (l_value[i] < 'a' && l_value[i] > 'z' && l_value[i] < 'A' && l_value[i] > 'Z') {
			std::cerr << "Invalid character: " << l_value[i] << "\n";
			return false;
		}
		else {
			soFar += l_value[i];
		}
	}

	if (!soFar.empty()) {
		res.push_back(soFar);
		soFar.clear();
	}

	return true;
}

void Trie::AddNewStep(Node* Cur) {
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

		if (clone.is_stateChanging) {
			if (clone.node_state.first == NodeState::Marked) {
				if (clone.node_state.second == NodeState::InRemove) {
					clone.node_state.first = NodeState::InRemove;
					clone.node_state.second = NodeState::Default;
				} else
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

	for (int i = 0; i < 26; i++) {
		AddNewStep(Cur->child[i]);
	}
}

void Trie::ResetNodes(Node* Cur) {
	if (!Cur)
		return;

	Cur->SaveState();
	Cur->getInfo()->clear();

	for (int i = 0; i < 26; i++) {
		ResetNodes(Cur->child[i]);
	}
}

void Trie::ClearTree(Node* Cur) {
	if (!Cur) {
		return;
	}

	for (int i = 0; i < 26; i++) {
		ClearTree(Cur->child[i]);
		Cur->child[i] = nullptr;
	}

	delete Cur;
}

void Trie::PostProcessing() {
	ClearTree(m_removed);
	m_removed = nullptr;
}

void Trie::HandleEvent(sf::Event* l_event) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->HandleEvent(l_event);
}

void Trie::Update(const sf::Time& l_time) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->Update(l_time.asMilliseconds());
}

void Trie::Draw() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	renderer->DrawTree(m_root);
	renderer->DrawTree(m_removed);
}

void Trie::OnCreate(const std::string& l_numbers, const std::string& l_value) {
	std::vector<std::string> l_strings;

	if (!ValidateCreate(l_value, l_strings)) {
		std::cerr << "Invalid input\n";
		return;
	}

	PostProcessing();
	ResetNodes(m_root);
	ClearTree(m_root);

	m_root = new Node(0);
	m_root->m_save.is_visible = 1;

	for (int i = 0; i < l_strings.size(); i++) {
		BuildTree(m_root, m_root, -1, l_strings[i]);
	}
	Align(m_root, -m_root->child_num / 2.f, m_root->child_num / 2.f);
	AddNewStep(m_root);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void Trie::OnInsert(const std::string& l_value) {
	if (!ValidateInput(l_value)) {
		std::cerr << "Invalid input\n";
		return;
	}

	if (!m_root) {
		m_root = new Node(0);
		m_root->m_save.is_visible = 1;
	}

	PostProcessing();
	ResetNodes(m_root);
	m_nodeNum++;
	InsertNode(m_root, m_root, -1, l_value);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void Trie::OnRemove(const std::string& l_value) {
	if (!ValidateInput(l_value)) {
		std::cerr << "Invalid input\n";
		return;
	}

	PostProcessing();
	ResetNodes(m_root);
	m_nodeNum++;
	RemoveNode(m_root, m_root, -1, l_value);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void Trie::OnSearch(const std::string& l_value) {
	int value = 0;
	if (!ValidateInput(l_value))
		return;

	PostProcessing();
	ResetNodes(m_root);

	SearchNode(m_root, -1, l_value);

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Reset(m_root->getInfo()->size());
}

void Trie::BuildTree(Node* Cur, Node* pre, int index, const std::string& l_string) {
	if (!Cur) {
		int value = l_string[index] - 'A' + 1;
		if (l_string[index] >= 'a')
			value = l_string[index] - 'a' + 1;

		Cur = new Node(value);

		pre->child[value - 1] = Cur;

		Cur->par = pre;
		Node* Par = Cur->par;

		if (Par->child_num > 0) {
			while (Par) {
				Par->child_num++;
				Par = Par->par;
			}
			Par = pre;
		}
		else {
			Par->child_num++;
		}

		Par->m_save.m_arrowChange[value - 1] = Cur;
		Cur->m_save.m_coord.first = pre->m_save.m_coord.first;
		Cur->m_save.m_coord.second.second = pre->m_save.m_coord.second.second + 1;

		Cur->m_save.is_appearing = 1;
		Cur->m_save.is_visible = 1;

		if (index == l_string.size() - 1)
			Cur->m_save.node_state = { NodeState::Marked, NodeState::Marked };
		else {
			int childIndex = l_string[index + 1] - 'A';
			if (l_string[index + 1] >= 'a')
				childIndex = l_string[index + 1] - 'a';

			BuildTree(Cur->child[childIndex], Cur, index + 1, l_string);
		}

		return;
	}

	int childIndex = l_string[index + 1] - 'A';
	if (l_string[index + 1] >= 'a')
		childIndex = l_string[index + 1] - 'a';

	if (index == l_string.size() - 1) {
		Cur->m_save.is_stateChanging = 1;
		Cur->m_save.node_state.second = NodeState::Marked;
	}
	else
		BuildTree(Cur->child[childIndex], Cur, index + 1, l_string);
}

void Trie::InsertNode(Node* Cur, Node* pre, int index, const std::string& l_string) {
	if (!Cur) {
		int value = l_string[index] - 'A' + 1;
		if (l_string[index] >= 'a')
			value = l_string[index] - 'a' + 1;

		Cur = new Node;
		
		pre->child[value - 1] = Cur;
		
		Cur->par = pre;
		Node* Par = Cur->par;

		if (Par->child_num > 0) {
			while (Par) {
				Par->child_num++;
				Par = Par->par;
			}
			Par = pre;
		}
		else { 
			Par->child_num++;
		}
		
		Par->getInfo()->back().m_arrowChange[value - 1] = Cur;
		Cur->getInfo()->resize(pre->getInfo()->size());
		Cur->getInfo()->back().is_valueChanging = 1;
		Cur->getInfo()->back().m_valueChange = { 0, value };
		Cur->getInfo()->back().m_coord.first = pre->getInfo()->back().m_coord.first;
		Cur->getInfo()->back().m_coord.second.second = pre->getInfo()->back().m_coord.second.second + 1;

		AddNewStep(m_root);
		Cur->getInfo()->back().is_appearing = 1;
		Cur->getInfo()->back().is_visible = 1;
		Align(m_root, - m_root->child_num / 2.f, m_root->child_num / 2.f);
		
		if (index == l_string.size() - 1) 
			Cur->getInfo()->back().node_state = {NodeState::Marked, NodeState::Marked};
		else {
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state = { NodeState::Selected, NodeState::Selected };
			int childIndex = l_string[index + 1] - 'A';
			if (l_string[index + 1] >= 'a')
				childIndex = l_string[index + 1] - 'a';

			InsertNode(Cur->child[childIndex], Cur, index + 1, l_string);
		}

		return;
	}
	
	int childIndex = l_string[index + 1] - 'A';
	if (l_string[index + 1] >= 'a')
		childIndex = l_string[index + 1] - 'a';
	AddNewStep(m_root);
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	if (index == l_string.size() - 1) {
		AddNewStep(m_root);
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::Marked;
	} else
		InsertNode(Cur->child[childIndex], Cur, index + 1, l_string);
}

void Trie::RemoveNode(Node* Cur, Node* pre, int index, const std::string& l_string) {
	if (!Cur) {
		AddNewStep(m_root);
		pre->getInfo()->back().is_stateChanging = 1;
		pre->getInfo()->back().node_state.second = NodeState::NotFound;
		return;
	}

	if (index == l_string.size() - 1) {
		if (Cur->getInfo()->back().node_state.first != NodeState::Marked) {
			AddNewStep(m_root);
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state.second = NodeState::NotFound;
			return;
		}

		AddNewStep(m_root);
		if (Cur->child_num > 0) {
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state.second = NodeState::InRemove;
			return;
		}
		else {
			Cur->getInfo()->back().is_appearing = 2;
			if (Cur->par) {
				Cur->par->getInfo()->back().m_arrowChange[Cur->getInfo()->back().m_shownValue[0] - 1] = nullptr;
			}
			m_removed = Cur;
		}

		Node* Par = Cur->par;

		while (Par && Par->child_num > 0 && Par->getInfo()->back().node_state.first != NodeState::Marked) {
			Par->child_num--;
			if (Par->child_num == 0) {
				m_removed = Par;
				Par->getInfo()->back().is_appearing = 2;
				if (Par->par) {
					Par->par->getInfo()->back().m_arrowChange[Par->getInfo()->back().m_shownValue[0] - 1] = nullptr;
				}
			}

			Par = Par->par;
		}

		if (Par) {
			Par->child_num--;
		}

		if (m_removed) {
			if (m_removed->par) {
				m_removed->par->child[m_removed->getInfo()->back().m_shownValue[0] - 1] = nullptr;
			}
		}

		Align(m_root, - m_root->child_num / 2.f, m_root->child_num / 2.f);

		return;
	}

	int childIndex = l_string[index + 1] - 'A';
	if (l_string[index + 1] >= 'a')
		childIndex = l_string[index + 1] - 'a';
	AddNewStep(m_root);
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;


	RemoveNode(Cur->child[childIndex], Cur, index + 1, l_string);
}

void Trie::SearchNode(Node* Cur, int index, const std::string& l_string) {
	if (index == l_string.size() - 1) {
		if (Cur->getInfo()->back().node_state.first != NodeState::Marked) {
			AddNewStep(m_root);
			Cur->getInfo()->back().is_stateChanging = 1;
			Cur->getInfo()->back().node_state.second = NodeState::NotFound;
			return;
		}

		AddNewStep(m_root);
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::Found;
		return;
	}

	int childIndex = l_string[index + 1] - 'A';
	if (l_string[index + 1] >= 'a')
		childIndex = l_string[index + 1] - 'a';
	AddNewStep(m_root);
	Cur->getInfo()->back().is_stateChanging = 1;
	Cur->getInfo()->back().node_state.second = NodeState::Selected;

	if (!Cur->child[childIndex]) {
		AddNewStep(m_root);
		Cur->getInfo()->back().is_stateChanging = 1;
		Cur->getInfo()->back().node_state.second = NodeState::NotFound;
		return;
	}

	SearchNode(Cur->child[childIndex], index + 1, l_string);
}


void Trie::Align(Node* Cur, const float& left, const float& right) {
	if (!Cur)
		return;

	if (!Cur->getInfo()->empty()) {
		Cur->getInfo()->back().is_moving = 1;
		Cur->getInfo()->back().m_coord.second.first = (left + right) / 2;
	}
	else {
		Cur->m_save.is_moving = 1;
		Cur->m_save.m_coord.second.first = (left + right) / 2;
	}

	int cnt = 0; // count unit added
	for (int i = 0; i < 26; i++) {
		if (!Cur->child[i])
			continue;

		Align(Cur->child[i], left + cnt, left + cnt + std::max(1, Cur->child[i]->child_num));
		cnt += std::max(1, Cur->child[i]->child_num);
	}
}