#pragma once
#include "StateManager.h"

class AVL_Tree : public BaseState {
private:
	const int MAX_NODE_NUM{ 15 };
	int m_nodeNum{ 0 };
	
	float m_elapsed{ 0.f };
	bool m_isReversing{ 0 };
	bool m_isPausing{ 0 };
	Node* m_root{ nullptr };
	Node* m_newNode{ nullptr };
	Node* m_removedNode{ nullptr };

	Node* InsertNode(Node* Cur, int value, int hor_depth, int ver_depth);
	Node* Generate(Node* Cur, int value);
	void RemoveNode(Node* Cur, int value);

	void ShiftIn(Node* Cur, int value);
	void ShiftOut(Node* Cur, int value);

	void AddNewStep(Node* Cur);
	void ClearStep(Node* Cur);

public:
	AVL_Tree(StateManager* l_stateManager) : BaseState(l_stateManager) {}
	~AVL_Tree();

	void OnCreate();
	void OnDestroy();

	void Activate() ;
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

	void OnGenerate();
	void OnInsert();
	void OnRemove(int value);

	void RotateLeft();
	void RotateRight();

	void PostProcessing();
};
