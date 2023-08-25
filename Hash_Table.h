#pragma once
#include "StateManager.h"

class HashTable : public BaseState {
private:
	const int MAX_NODE_NUM{ 50 };
	const sf::Vector2i DEFAULT_COORD{ -9, 0 };
	const int NODE_PER_ROW	{ 20 };

	std::vector<Node*> m_probingNodes;
	std::vector<Node*> m_chainingNodes;

	Node* m_removed{ nullptr };

	int m_mode{ 0 }; // 0: chaining, 1: linear probing, 2: quadratic probing

	bool ValidateInput(const std::string& l_value, int& resValue);
	bool ValidateCreate(const std::string& l_value, int& n, int& m);

	void AddNodeStep(Node* node);
	void AddNewStep();

	void InsertChaining(int value);
	void InsertLinearProbing(int value);
	void InsertQuadraticProbing(int value);

	void RemoveChaining(int value);
	void RemoveLinearProbing(int value);
	void RemoveQuadraticProbing(int value);

	void SearchChaining(int value);
	void SearchLinearProbing(int value);
	void SearchQuadraticProbing(int value);

	void ClearNodes(std::vector<Node*>& nodes);
	void ClearChain(Node* Cur);

	void Create(int n, int m);

	void ShiftUp(Node* Cur);

	void InsertNode(int value); //No animation
	void ResetNodes();

	void PostProccessing();

public:
	HashTable(StateManager* l_stateManager) : BaseState(l_stateManager) {
	}

	~HashTable();

	StateManager* GetStateManager() {
		return m_stateManager;
	}

	void OnDestroy(){};

	void Activate();
	void Deactivate() {};

	void HandleEvent(sf::Event* l_event);
	void Update(const sf::Time& l_time);
	void Draw();

	void OnCreate(const std::string& l_value);
	void OnInsert(const std::string& l_value);
	void OnRemove(const std::string& l_value);
	void OnSearch(const std::string& l_value);
	void OnGetSize() {};
	void OnGetTop() {}
	void OnInputMatrix(const std::vector<std::string> l_lines) {}

	void SwitchMode(int l_mode);
	int GetMode() const {
		return m_mode;
	}
};
