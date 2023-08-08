#pragma once
#include "StateManager.h"

class HashTable : public BaseState {
private:
	std::vector<Node*> m_probingNodes;
	std::vector<Node*> m_chainingNodes;

	int m_mode{ 0 }; // 0: chaining, 1: linear probing, 2: quadratic probing

	bool ValidateInput(const std::string& l_value, int& resValue);
	bool ValidateCreate(const std::string& l_numbers, const std::string& l_value);

	void AddNodeStep(Node* node);
	void AddNewStep();

	void InsertChaining(int value);
	void InsertLinearProbing(int value);
	void InsertQuadraticProbing(int value);

	void ClearNodes(std::vector<Node*>& nodes);
	void ClearChain(Node* Cur);

public:
	HashTable(StateManager* l_stateManager) : BaseState(l_stateManager) {
	}

	~HashTable();

	StateManager* GetStateManager() {
		return m_stateManager;
	}

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void HandleEvent(sf::Event* l_event);
	void Update(const sf::Time& l_time);
	void Draw();

	void OnInsert(const std::string& l_value) = 0;
	void OnRemove(const std::string& l_value) = 0;
};