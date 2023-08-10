#pragma once
#include "StateManager.h"

class AVL_Tree : public BaseState {
private:
	std::vector<Node*> m_align;
	const int ALIGN_OFFSET{ 10 };

	const int MAX_NODE_NUM{ 21 };
	const int MAX_WIDTH{ 10 };

	int m_nodeNum{ 0 };
	int m_leftWidth{ 0 };
	int m_rightWidth{ 0 };
	
	float m_elapsed{ 0.f };

	Node* m_root{ nullptr };
	Node* m_newNode{ nullptr };
	Node* m_removedNode{ nullptr };

	Node* InsertNode(Node* Cur, int value, int hor_depth, int ver_depth);
	Node* Generate(Node* Cur, int value);
	Node* RemoveNode(Node* Cur, int value);

	void ShiftLeftFrom(int value);
	void ShiftRightFrom(int value);
	void ShiftLeftTo(int value);
	void ShiftRightTo(int value);
	void ShiftUp(Node* Cur);
	void ShiftDown(Node* Cur);

	void AddNewStep(Node* Cur);
	void ClearStep(Node* Cur);
	void Centering();

	bool ValidateInput(const std::string& l_value, std::vector<int>& res);
	Node* BuildTree(Node* Cur, int value);

	void ClearTree(Node* Cur);
public:
	AVL_Tree(StateManager* l_stateManager) : BaseState(l_stateManager) {
		m_align.resize(2 * MAX_WIDTH + 1);
		fill(m_align.begin(), m_align.end(), nullptr);
	}

	~AVL_Tree();

	void OnCreate(const std::string& l_numbers, const std::string& l_value);
	void OnDestroy();

	void Activate() ;
	void Deactivate();

	void HandleEvent(sf::Event* l_event);
	void Update(const sf::Time& l_time);
	void Draw();
	void ResetNodes(Node* Cur);

	void OnGenerate();
	void OnInsert(const std::string& l_value);
	void OnRemove(const std::string& l_value);

	Node* RotateLeft(Node* Cur);
	Node* RotateRight(Node* Cur);

	void PostProcessing();

	void TestFunc();
};
