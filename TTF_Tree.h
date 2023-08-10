#pragma once
#include "StateManager.h"

class TTF_Tree : public BaseState {
private:
	std::vector<Node*> m_align{ std::vector<Node*>(51, nullptr) };
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

	Node* InsertNode(Node* Cur, int value, int hor_depth, int ver_depth);
	Node* RemoveNode(Node* Cur, int value);
	void SearchNode(Node* Cur, int value);

	void ShiftLeftFrom(int value);
	void ShiftRightFrom(int value);
	void ShiftLeftTo(int value);
	void ShiftRightTo(int value);
	void ShiftUp(Node* Cur);
	void ShiftDown(Node* Cur);

	void AddNewStep(Node* Cur);
	void ClearStep(Node* Cur);
	void Centering();

	bool ValidateInput(const std::string& l_value, int& resValue);
	bool ValidateCreate(const std::string& l_numbers, const std::string& l_value, int& n, int& m);

	void AddNodeStep(Node* node);
	void AddNewStep();

	void ResetNodes();

public:
	TTF_Tree(StateManager* l_stateManager) : BaseState(l_stateManager) {
	}

	~TTF_Tree();

	StateManager* GetStateManager() {
		return m_stateManager;
	}

	void OnDestroy() {};

	void Activate() {};
	void Deactivate() {};

	void HandleEvent(sf::Event* l_event);
	void Update(const sf::Time& l_time);
	void Draw();

	void OnCreate(const std::string& l_numbers, const std::string& l_value);
	void OnInsert(const std::string& l_value);
	void OnRemove(const std::string& l_value);
	void OnSearch(const std::string& l_value);
};
