#pragma once
#include "StateManager.h"

#include <queue>
#include <stack>

using Edge = std::pair<std::pair<std::string, std::string>, int>;

class Graph : public BaseState {
private:
	int m_nodeNum{ 0 };

	std::vector<GraphNode*> m_nodes;
	std::map<std::string, GraphNode*> m_registered;
	std::map<std::pair<std::string, std::string>, int> m_edges;

	void ResetNodes();
	void AddNewStep();
	void AddNodeStep(GraphNode* node);

	void Flow(GraphNode* start, GraphNode* end);
	void Fade(GraphNode* start, GraphNode* end);

	void ApplyAcc();
	void MoveNode(GraphNode* Cur, const float& l_time);

	bool ValidateInput(const std::string& l_value);
	bool ValidateCreate(const std::string& l_value);
	bool ValidateMatrix(const std::vector<std::string>& l_lines);

	void Dijkstra(GraphNode* start);
	void DFS(GraphNode* start, GraphNode* pre);
	void MST();
	void CC();

	GraphNode* Find(GraphNode* u);
	void Union(GraphNode* u, GraphNode* v);

	void PostProcessing();
	void ClearGraph();
public:
	Graph(StateManager* l_stateManager) : BaseState(l_stateManager) { }

	~Graph();

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
	void OnInputMatrix(const std::vector<std::string> l_lines);
	void OnGetSize() {}
	void OnGetTop() {}

	void SwitchMode(int l_mode) {}
};
