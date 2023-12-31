#pragma once
#include "StateManager.h"
#include <stack>

class Trie : public BaseState {
private:
	int m_nodeNum{ 0 };

	Node* m_root{ nullptr };
	Node* m_newNode{ nullptr };
	Node* m_removed;

	void InsertNode(Node* Cur, Node* pre, int index, const std::string& l_string);
	void RemoveNode(Node* Cur, Node* pre, int index, const std::string& l_string);
	void SearchNode(Node* Cur, int index, const std::string& l_string);
	void BuildTree(Node* Cur, Node* pre, int index, const std::string& l_string);

	void ResetNodes(Node* Cur);
	void AddNewStep(Node* Cur);
	void Align(Node* Cur, const float& left, const float& right);

	bool ValidateInput(const std::string& l_value);
	bool ValidateCreate(const std::string& l_value, std::vector<std::string>& res);

	void PostProcessing();
	void ClearTree(Node* Cur);
public:
	Trie(StateManager* l_stateManager) : BaseState(l_stateManager) {
		m_root = new Node(0);
		m_root->m_save.is_visible = 1;

	}

	~Trie();

	void OnDestroy() {}

	void Activate();
	void Deactivate() {}

	void HandleEvent(sf::Event* l_event);
	void Update(const sf::Time& l_time);
	void Draw();

	void OnCreate(const std::string& l_value);
	void OnInsert(const std::string& l_value);
	void OnRemove(const std::string& l_value);
	void OnSearch(const std::string& l_value);
	void OnGetSize() {}
	void OnGetTop() {}
	void OnInputMatrix(const std::vector<std::string> l_lines) {}

	void SwitchMode(int l_mode) {}


	Node* RotateLeft(Node* Cur);
	Node* RotateRight(Node* Cur);

	void TestFunc();
};
