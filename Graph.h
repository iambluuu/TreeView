#pragma once
#include "StateManager.h"
#include <stack>

class Graph : public BaseState {
private:
	int m_nodeNum{ 0 };

	std::vector<GraphNode*> m_nodes;
	std::map<std::string, GraphNode*> m_registered;

	void ResetNodes();
	void AddNewStep();
	void AddNodeStep(GraphNode* node);

	void ApplyAcc();
	void MoveNode(GraphNode* Cur, const float& l_time);

	bool ValidateInput(const std::string& l_value);
	bool ValidateCreate(const std::string& l_value, std::vector<std::pair<std::string, std::pair<std::string, int> > >& edges);

	void PostProcessing();
	void ClearGraph();
public:
	Graph(StateManager* l_stateManager) : BaseState(l_stateManager) {

	}

	~Graph();

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
	void OnGetSize() {}
	void OnGetTop() {}

	void SwitchMode(int l_mode) {}
};
