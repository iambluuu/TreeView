#pragma once
#include "StateManager.h"
#include <stack>

class Heap : public BaseState {
private:
	std::vector<Node*>* m_arr{ nullptr };
	std::vector<Node*> m_maxHeap = { std::vector<Node*> () };
	std::vector<Node*> m_minHeap = { std::vector<Node*> () };

	std::vector<std::pair<int, int> > m_size = { std::vector<std::pair<int ,int> >(51) };
	Node* m_removed;

	bool m_mode{ 0 }; // 0 - max, 1 - min

	Node* m_root{ nullptr };
	Node* m_newNode{ nullptr };

	void InsertNode(int value);
	void RemoveNode(int value);
	void SearchNode(int value);
	void BuildTree(int value);

	void ResetNodes();
	void AddNodeStep(Node* Cur);
	void AddNewStep();

	void Arrange();
	int CalSize(Node* Cur, int index);
	void Align(Node* Cur, int index, const float& left, const float& right);
	void Center(Node* Cur, const float& delta);

	void Heapify(int index);
	void HeapifyDown(int index);
	void Swap(int a, int b);
	void BuildSwap(int a, int b);

	bool ValidateInput(const std::string& l_value, std::vector<int>& res);
	bool ValidateIndex(const std::string& l_value, int& index);
	bool ValidateCreate(const std::string& l_value, std::vector<int>& res);

	void PostProcessing();
	void ClearAlign();
	void ClearTree();

	void SwitchMode(bool l_mode) {
		m_mode = l_mode;
		if (m_mode) {
			m_arr = &m_minHeap;
		}
		else {
			m_arr = &m_maxHeap;
		}
	}
public:
	Heap(StateManager* l_stateManager);

	~Heap();

	void OnDestroy() {}

	void Activate() {}
	void Deactivate() {}

	void HandleEvent(sf::Event* l_event);
	void Update(const sf::Time& l_time);
	void Draw();

	void OnCreate(const std::string& l_numbers, const std::string& l_value);
	void OnInsert(const std::string& l_value);
	void OnRemove(const std::string& l_value);
	void OnSearch(const std::string& l_value);
	void OnGetSize();
	void OnGetTop();

	void SwitchMode(int l_mode) {
		m_mode = l_mode;

		switch (m_mode) {
		case 0:
			m_arr = &m_maxHeap;
			break;
		case 1:
			m_arr = &m_minHeap;
			break;
		}
	}

	Node* RotateLeft(Node* Cur);
	Node* RotateRight(Node* Cur);

	void TestFunc();
};
