#include "Graph.h"
#include <sstream>

const float TIME_CONST = 0.1f;

const float RADIUS = 150.f;

const float TOP = 110.f;
const float BOT = 110.f + 680.f;
const float LEFT = 0.f + 1500.f;
const float RIGHT = 2 * 592.f + 1500.f;

const sf::Vector2f CENTER = { (LEFT + RIGHT) / 2, (TOP + BOT) / 2 };

Graph::~Graph() {
	ClearGraph();
}

void Graph::Activate() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	if (!m_nodes.empty())
		renderer->Reset(m_nodes[0]->m_info.size());
	else
		renderer->Reset(0);

	codeWindow->Clear();
	renderer->OnSkipForward();
}


void Graph::PostProcessing() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();
	
	ResetNodes();
	codeWindow->Reset();
}

void Graph::ClearGraph() {
	for (int i = 0; i < m_nodes.size(); i++) {
		delete m_nodes[i];
	}
	m_nodes.clear();
	m_registered.clear();
}

void Graph::ResetNodes() {
	for (auto& u : m_nodes) {
		u->SaveState();
		u->m_info.clear();
		u->visited = 0;
	}
}

void Graph::AddNewStep() {
	for (auto& u : m_nodes) {
		AddNodeStep(u);
	}
}

void Graph::AddNodeStep(GraphNode* node) {
	auto CurInfo = &node->m_info;

	if (CurInfo->empty()) {
		CurInfo->push_back(node->m_save);

	}
	else {
		GraphNodeInfo clone = CurInfo->back();

		for (int i = 0; i < clone.edge_state.size(); i++) {
			if (clone.edge_state[i] == 0 || clone.edge_state[i] == 1 || clone.edge_state[i] == 2)
				clone.edge_state[i] = 3;
		}

		if (clone.is_appearing == 2) {
			clone.is_visible = 0;
		}

		clone.is_appearing = 0;

		if (clone.is_stateChanging == 1) {
			clone.is_stateChanging = 0;
			clone.node_state.first = clone.node_state.second;
		}

		if (clone.node_state.first == NodeState::Selected) {
			clone.is_stateChanging = 1;
			clone.node_state.second = NodeState::Visited;
		}

		clone.m_dist.first = clone.m_dist.second;

		CurInfo->push_back(clone);
	}
}

void Graph::Flow(GraphNode* start, GraphNode* end) {
	if (!start || !end) {
		return;
	}
	
	for (int i = 0; i < start->m_edges.size(); i++) {
		if (start->m_edges[i].first == end) {
			start->m_info.back().edge_state[i] = 0;
			break;
		}
	}

	for (int i = 0; i < end->m_edges.size(); i++) {
		if (end->m_edges[i].first == start) {
			end->m_info.back().edge_state[i] = 1;
			break;
		}
	}
}

void Graph::Fade(GraphNode* start, GraphNode* end) {
	if (!start || !end) {
		return;
	}

	start->ChangeState(NodeState::Found);
	end->ChangeState(NodeState::Found);

	for (int i = 0; i < start->m_edges.size(); i++) {
		if (start->m_edges[i].first == end) {
			start->m_info.back().edge_state[i] = 2;
			break;
		}
	}

	for (int i = 0; i < end->m_edges.size(); i++) {
		if (end->m_edges[i].first == start) {
			end->m_info.back().edge_state[i] = 2;
			break;
		}
	}
}

void Graph::HandleEvent(sf::Event* l_event) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->HandleEvent(l_event);

	sf::RenderWindow* window = m_stateManager->GetContext()->m_wind->GetRenderWindow();
	sf::Vector2f offset = m_stateManager->GetContext()->m_wind->GetOffset();

	if (l_event->type == sf::Event::KeyPressed && l_event->key.code == sf::Keyboard::Tab) {
		Page* page = m_stateManager->GetContext()->m_wind->GetPage();
		page->Open();
		return;
	}

	if (l_event->type == sf::Event::MouseButtonPressed && l_event->key.code == sf::Mouse::Left) {
		for (auto& u : m_nodes) {
			u->isFixed = 0;
		}
	}

	for (auto& u : m_nodes) {
		u->HandleEvent(l_event, window, offset);
	}
}

bool Graph::ValidateInput(const std::string& l_value) {


	return true;
}

bool Graph::ValidateMatrix(const std::vector<std::string>& l_lines) {
	if (l_lines.empty()) {
		std::cerr << "empty content\n";
		return false;
	}

	int n = l_lines.size();

	for (int i = 0; i < n; i++) {
		std::string u(1, i + 'A');
		std::stringstream ss(l_lines[i]);

		int w = 0;
		int count = 0;

		while(ss >> w) {
			std::string v(1, count + 'A');

			if (u == v) {
				if (w != 0) {
					std::cerr << "self loop" << i << " " << count << " " << w << "\n";
					m_edges.clear();
					return false;
				}
				
				count++;
				continue;
			}

			if (m_edges.find({ u, v }) == m_edges.end() && m_edges.find({ v, u }) == m_edges.end()) {
				m_edges.emplace(std::pair{ u, v }, w);
			}

			count++;
		}

		if (count != n) {
			std::cerr << "column shorter than row\n";
			m_edges.clear();
			return false;
		}
	}
}

bool Graph::ValidateCreate(const std::string& l_value) {

	for (int i = 0; i < l_value.size(); ) {
		int j = i;
		while (j < l_value.size() && l_value[j] != ',') {
			j++;
		}

		std::stringstream ss(l_value.substr(i, j - i + 1));

		std::string u, v;
		int w;

		ss >> u >> v >> w;

		if (u == "" || v == "" || w == 0) {
			m_edges.clear();
			return false;
		}

		if (u == v)
			continue;

		if (m_edges.find({ u, v }) == m_edges.end() && m_edges.find({v, u}) == m_edges.end()) {
			m_edges.emplace( std::pair{u, v}, w);
		}

		i = j + 1;
	}

	if (m_edges.empty())
		return false;

	return true;
}

void Graph::Update(const sf::Time& l_time) {
	ApplyAcc();
	for (auto& u : m_nodes) {
		MoveNode(u, l_time.asSeconds());
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->Update(l_time.asMilliseconds());

	// Reset isDrawn

	for (auto& u : m_nodes) {
		u->isDrawn = false;
	}
}

void Graph::Draw() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	for (auto& u : m_nodes) {
		if (!u->isDrawn) {
			renderer->DrawGraph(u);
		}
	}
}

void Graph::ApplyAcc() {

	for (auto& u : m_nodes) {
		u->m_vel.x = 0;
		u->m_vel.y = 0;
	}

	for (auto& u : m_nodes) {
		for (auto& v : m_nodes) {
			if (u == v)
				continue;

			sf::Vector2f uPos = u->pos;
			sf::Vector2f vPos = v->pos;

			float dist = sqrt(pow(uPos.x - vPos.x, 2) + pow(uPos.y - vPos.y, 2));

			if (dist < RADIUS) {
				v->m_vel.x += (vPos.x - uPos.x) / dist * (RADIUS - dist) / TIME_CONST;
				v->m_vel.y += (vPos.y - uPos.y) / dist * (RADIUS - dist) / TIME_CONST;
			}
		}

		for (auto& edge : u->m_edges) {
			GraphNode* v = edge.first;

			sf::Vector2f uPos = u->pos;
			sf::Vector2f vPos = v->pos;

			float dist = sqrt(pow(uPos.x - vPos.x, 2) + pow(uPos.y - vPos.y, 2));

			if (dist > RADIUS) {
				v->m_vel.x += (vPos.x - uPos.x) / dist * (RADIUS - dist) / TIME_CONST;
				v->m_vel.y += (vPos.y - uPos.y) / dist * (RADIUS - dist) / TIME_CONST;
			}
		}
	}

	//float toward center
	for (auto& u : m_nodes) {
		float distFromCenter = sqrt(pow(u->pos.x - CENTER.x, 2) + pow(u->pos.y - CENTER.y, 2));

		if (distFromCenter == 0)
			continue;

		u->m_vel += (CENTER - u->pos) / distFromCenter * distFromCenter / (25 * TIME_CONST);
	}
}

void Graph::MoveNode(GraphNode* Cur, const float& l_time) {
	if (Cur->isFixed)
		return;

	if (abs(Cur->m_vel.x * l_time) > 0.1)
		Cur->pos.x += Cur->m_vel.x * l_time;

	if (abs(Cur->m_vel.y * l_time) > 0.1)
		Cur->pos.y += Cur->m_vel.y * l_time;
}

void Graph::OnCreate(const std::string& l_value) {
	
	if (!ValidateCreate(l_value)) {
		std::cerr << "Invalid input" << std::endl;
		return;
	}

	if (m_edges.empty()) {
		std::cerr << "No valid edges" << std::endl;
		return;
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	ClearGraph();

	for (auto& edge: m_edges) {
		std::string u = edge.first.first;
		std::string v = edge.first.second;
		int w = edge.second;
		
		GraphNode* U = nullptr, *V = nullptr;

		if (m_registered.find(u) == m_registered.end()) {
			U = new GraphNode(u);
			U->pos.x = LEFT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (RIGHT - LEFT)));
			U->pos.y = BOT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (TOP - BOT)));
			m_nodes.push_back(U);
			m_registered.emplace(u, U);
		}
		else {
			U = m_registered[u];
		}

		if (m_registered.find(v) == m_registered.end()) {
			V = new GraphNode(v);
			V->pos.x = LEFT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (RIGHT - LEFT)));
			V->pos.y = BOT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (TOP - BOT)));
			m_nodes.push_back(V);
			m_registered.emplace(v, V);
		}
		else {
			V = m_registered[v];
		}

		U->m_edges.push_back(std::make_pair(V, w));
		V->m_edges.push_back(std::make_pair(U, w));

		U->m_save.edge_state.push_back(-1);
		V->m_save.edge_state.push_back(-1);
	}

	m_nodes[0]->pos = CENTER;

	std::cerr << "number of nodes: " << m_nodes.size() << std::endl;

	AddNewStep();
	renderer->Reset(m_nodes[0]->m_info.size());
}

void Graph::OnInputMatrix(const std::vector<std::string> l_lines) {
	std::cerr << "OnInputMatrix" << std::endl;

	if (!ValidateMatrix(l_lines)) {
		std::cerr << "Invalid input" << std::endl;
		return;
	}

	if (m_edges.empty()) {
		std::cerr << "No valid edges" << std::endl;
		return;
	}

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;

	ClearGraph();

	for (auto& edge : m_edges) {
		std::string u = edge.first.first;
		std::string v = edge.first.second;
		int w = edge.second;

		GraphNode* U = nullptr, * V = nullptr;

		if (m_registered.find(u) == m_registered.end()) {
			U = new GraphNode(u);
			U->pos.x = LEFT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (RIGHT - LEFT)));
			U->pos.y = BOT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (TOP - BOT)));
			m_nodes.push_back(U);
			m_registered.emplace(u, U);
		}
		else {
			U = m_registered[u];
		}

		if (m_registered.find(v) == m_registered.end()) {
			V = new GraphNode(v);
			V->pos.x = LEFT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (RIGHT - LEFT)));
			V->pos.y = BOT + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (TOP - BOT)));
			m_nodes.push_back(V);
			m_registered.emplace(v, V);
		}
		else {
			V = m_registered[v];
		}

		U->m_edges.push_back(std::make_pair(V, w));
		V->m_edges.push_back(std::make_pair(U, w));

		U->m_save.edge_state.push_back(-1);
		V->m_save.edge_state.push_back(-1);
	}

	m_nodes[0]->pos = CENTER;

	std::cerr << "number of nodes: " << m_nodes.size() << std::endl;

	AddNewStep();
	renderer->Reset(m_nodes[0]->m_info.size());
}

//Dijkstra
void Graph::OnInsert(const std::string& l_value) {
	std::cerr << "Triggering Dijkstra" << std::endl;

	if (m_nodes.empty())
		return;

	if (m_registered.find(l_value) == m_registered.end()) {
		std::cerr << "Node not found" << std::endl;
		return;
	}

	PostProcessing();

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//Set up code window///////////////////////////
	std::vector<std::string> code;

	code = {
		"dist[s] = 0, PQ = {(0, s)}",
		"while !PQ.empty():",
		"    du, u = Q.front(), Q.pop()",
		"    if du > dist[u]: continue",
		"    for v adjacent to u with weight w:",
		"        if minimize(dist[v], dist[u] + w):",
		"            PQ.push((dist[v], v))"
	};

	codeWindow->LoadCode(code);
	///////////////////////////////////////////////

	GraphNode* Start = m_registered[l_value];

	Dijkstra(Start);

	renderer->Reset(m_nodes.front()->m_info.size());
}

//Connected Components
void Graph::OnRemove(const std::string& l_value) {
	if (m_nodes.empty())
		return;

	PostProcessing();

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//Set up code window///////////////////////////
	std::vector<std::string> code;

	code = {
		"for i = 0 to n - 1:",
		"	if (!node[i].visited)",
		"		count++",
		"		dfs(node[i])"
	};

	codeWindow->LoadCode(code);
	///////////////////////////////////////////////

	CC();

	renderer->Reset(m_nodes.front()->m_info.size());
}

//MST
void Graph::OnSearch(const std::string& l_value) {
	if (m_nodes.empty())
		return;

	PostProcessing();

	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	//Set up code window///////////////////////////
	std::vector<std::string> code;

	code = {
		"sort edges by weight",
		"for i = 0 to edges.size() - 1:",
		"	if (!isSameSet(edges[i].first, edges[i].second)",
		"		union(edges[i].first, edges[i].second)",
		"		add edges[i] to MST"
	};

	codeWindow->LoadCode(code);
	///////////////////////////////////////////////

	MST();

	renderer->Reset(m_nodes.front()->m_info.size());
}

void Graph::DFS(GraphNode* start, GraphNode* pre) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	start->visited = true;

	AddNewStep();
	codeWindow->MoveHighlight(3);
	Flow(pre, start);
	start->ChangeState(NodeState::Selected);

	for (auto& edge : start->m_edges) {
		GraphNode* v = edge.first;
		if (!v->visited) {
			DFS(v, start);
		}
	}	
}

void Graph::Dijkstra(GraphNode* Start) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	std::priority_queue<std::pair<int, GraphNode*>, std::vector<std::pair<int, GraphNode*>>, std::greater<std::pair<int, GraphNode*>>> pq;

	for (int i = 0; i < m_nodes.size(); i++) {
		m_nodes[i]->m_save.m_dist = std::make_pair(2e9, 2e9);
	}

	Start->m_save.m_dist = std::make_pair(0, 0);

	pq.push({0, Start});

	while (!pq.empty()) {
		GraphNode* u = pq.top().second;
		pq.pop();

		if (u->visited)
			continue;

		u->visited = 1;

		AddNewStep();
		codeWindow->MoveHighlight(2);

		u->m_info.back().is_stateChanging = 1;
		u->m_info.back().node_state.second = NodeState::Selected;

		int distU = u->m_info.back().m_dist.first;

		for (int i = 0; i < u->m_edges.size(); i++) {
			GraphNode* v = u->m_edges[i].first;
			int w = u->m_edges[i].second;
			int distV = v->m_info.back().m_dist.first;

			if (distV > distU + w) {
				AddNewStep();
				codeWindow->MoveHighlight(6);
				u->m_info.back().is_stateChanging = 0;

				v->m_info.back().is_stateChanging = 1;
				v->m_info.back().node_state.second = NodeState::Selected;
				v->m_info.back().m_dist.second = distU + w;

				pq.push({distU + w, v});
			}
		}
	}
}

void Graph::MST() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	std::vector<Edge> edges;
	for (auto& edge : m_edges) {
		std::string u = edge.first.first;
		std::string v = edge.first.second;
		int w = edge.second;

		edges.push_back(std::make_pair(std::make_pair(u, v), w));
	}

	std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
		return a.second < b.second;
		});

	for (auto& edge : edges) {
		std::string u = edge.first.first;
		std::string v = edge.first.second;
		int w = edge.second;

		GraphNode* U = m_registered[u];
		GraphNode* V = m_registered[v];

		if (Find(U) != Find(V)) {
			AddNewStep();
			codeWindow->MoveHighlight(4);
			Union(U, V);
			Fade(U, V);
		}
	}

	AddNewStep();
	for (auto& u : m_nodes) {
		for (int i = 0; i < u->m_edges.size(); i++) {
			if (u->m_info.back().edge_state[i] == -1) {
				u->m_info.back().edge_state[i] = 4;
			}
		}
	}	
}

void Graph::CC() {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	CodeWindow* codeWindow = renderer->GetCodeWindow();

	for (auto& u : m_nodes) {
		if (!u->visited) {
			AddNewStep();
			codeWindow->MoveHighlight(2);
			DFS(u, nullptr);
		}
	}
}

GraphNode* Graph::Find(GraphNode* u) {
	return u->par = (u->par == u) ? u : Find(u->par);
}

void Graph::Union(GraphNode* u, GraphNode* v) {
	u = Find(u);
	v = Find(v);

	if (u == v)
		return;

	v->par = u;
}