#pragma once
#include "StateManager.h"

class TTF_Tree : public BaseState {
private:
	std::vector<std::vector<Node*> > m_align;

	const int ALIGN_OFFSET{ 25 };

	const int MAX_NODE_NUM{ 51 };
	const int MAX_WIDTH{ 25 };

	int m_nodeNum{ 0 };
	int m_leftWidth{ 0 };
	int m_rightWidth{ 0 };

	float m_elapsed{ 0.f };

	Node* m_root{ nullptr };
	Node* m_newNode{ nullptr };
	Node* m_removedNode{ nullptr };

	void InsertNode(Node* Cur, int value);
	void RemoveNode(Node* Cur, int value);
	void SearchNode(Node* Cur, int value);

	void ExpandNode(Node* Cur, int value);
	void CollapseNode(Node* Cur, int value);
	void SplitNode(Node* Cur);

	void InsertToRow(Node* Cur, Node* LeftOfCur, int row);
	void Aligning();
	void ShiftDown();

	bool ValidateInput(const std::string& l_value, int& resValue);
	bool ValidateCreate(const std::string& l_value, std::vector<int>& res);

	void AddNewStep(Node* Cur);
	void ResetNodes(Node* Cur);

	void ClearTree(Node* Cur);
	void ClearAlign();

public:
	TTF_Tree(StateManager* l_stateManager) : BaseState(l_stateManager) {
	}

	~TTF_Tree();

	StateManager* GetStateManager() {
		return m_stateManager;
	}

	void OnDestroy() {};

	void Activate();
	void Deactivate() {};

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
};
