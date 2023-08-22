#include "Graph.h"
#include <sstream>

const float TIME_CONST = 0.2f;

const float RADIUS = 150.f;

const float TOP = 110.f;
const float BOT = 110.f + 680.f;
const float LEFT = 0.f + 1500.f;
const float RIGHT = 2 * 592.f + 1500.f;

const sf::Vector2f CENTER = { (LEFT + RIGHT) / 2, (TOP + BOT) / 2 };

Graph::~Graph() {
	ClearGraph();
}

void Graph::ClearGraph() {
	for (int i = 0; i < m_nodes.size(); i++) {
		delete m_nodes[i];
	}
	m_nodes.clear();
	m_registered.clear();
}

void Graph::ResetNodes() {

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
		NodeInfo clone = CurInfo->back();

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

		if (clone.is_valueChanging) {
			clone.is_valueChanging = 0;
			clone.m_shownValue[clone.m_valueChange.first] = clone.m_valueChange.second;
		}

		CurInfo->push_back(clone);
	}
}

void Graph::HandleEvent(sf::Event* l_event) {
	NodeRenderer* renderer = m_stateManager->GetContext()->m_nodeRenderer;
	renderer->HandleEvent(l_event);

	sf::RenderWindow* window = m_stateManager->GetContext()->m_wind->GetRenderWindow();
	sf::Vector2f offset = m_stateManager->GetContext()->m_wind->GetOffset();

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
			return false;
		}

		if (u == v)
			continue;

		if (m_edges.find({ u, v }) == m_edges.end() && m_edges.find({v, u}) == m_edges.end()) {
			m_edges.emplace( std::pair{u, v}, w);
		}


		i = j + 1;
	}

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

		u->m_vel += (CENTER - u->pos) / distFromCenter * distFromCenter / (25 * TIME_CONST);
	}
}

void Graph::MoveNode(GraphNode* Cur, const float& l_time) {
	if (Cur->isFixed)
		return;

	Cur->pos.x += Cur->m_vel.x * l_time;
	Cur->pos.y += Cur->m_vel.y * l_time;

}

void Graph::OnCreate(const std::string& l_numbers, const std::string& l_value) {
	
	if (!ValidateCreate(l_value)) {
		std::cerr << "Invalid input" << std::endl;
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
	}

	m_nodes[0]->pos = CENTER;

	AddNewStep();
	renderer->Reset(m_nodes[0]->m_info.size());
}

void Graph::OnInsert(const std::string& l_value) {

}

void Graph::OnRemove(const std::string& l_value) {

}

void Graph::OnSearch(const std::string& l_value) {

}