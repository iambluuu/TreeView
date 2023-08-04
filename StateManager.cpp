#include "StateManager.h"
#include "AVL_Tree.h"
#include "Hash_Table.h"

StateManager::StateManager(SharedContext* l_shareContext)
{
	m_context = l_shareContext;

	//RegisterState<Main_Menu>(StateType::MainMenu);
	//RegisterState<Hash_Table>(StateType::HashTable);
	//RegisterState<Binary_Tree>(StateType::BinaryTree);
	RegisterState<AVL_Tree>(StateType::AVLTree);
	//RegisterState<TT_Tree>(StateType::TTTree);
	//RegisterState<TTF_Tree>(StateType::TTFTree);
	//RegisterState<Trie>(StateType::Trie);
	//RegisterState<Graph>(StateType::Graph);

}

StateManager::~StateManager() {
	for (auto &itr : m_states) {
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::HandleEvent(sf::Event* l_event) {
	m_states.back().second->HandleEvent(l_event);
}

void StateManager::Update(const sf::Time& l_time) {
	if (m_states.empty())
		return;

	auto itr = m_states.end();

	while (itr != m_states.begin()) {
		if (itr != m_states.end()) {
			if (!itr->second->IsTransparent()) {
				break;
			}
		}
		itr--;
	}

	for (; itr < m_states.end(); itr++) {
		itr->second->Update(l_time);
	}
}

void StateManager::Draw() {
	if (m_states.empty())
		return;

	auto itr = m_states.end();

	while (itr != m_states.begin()) {
		if (itr != m_states.end()) {
			if (!itr->second->IsTransparent()) {
				break;
			}
		}
		itr--;
	}

	for (; itr < m_states.end(); itr++) {
		itr->second->Draw();
	}
}

SharedContext* StateManager::GetContext() {
	return m_context;
}

BaseState* StateManager::GetState(const StateType& l_type) {
	for (auto itr : m_states) {
		if (itr.first == l_type) {
			return itr.second;
		}
	}
}

bool StateManager::HasState(const StateType& l_type) {
	for (auto itr : m_states) {
		if (itr.first == l_type) {
			auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
			if (removed == m_toRemove.end()) {
				return true;
			}
			return false;
		}
	}

	return false;
}

void StateManager::Remove(const StateType& l_type) {
	m_toRemove.push_back(l_type);
}

void StateManager::ProcessRequests() {
	while (m_toRemove.begin() != m_toRemove.end()) {
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& l_type) {
	for (auto itr = m_states.begin(); itr < m_states.end(); itr++) {
		if (itr->first == l_type) {
			m_states.back().second->Deactivate();
			StateType tmpType = itr->first;
			BaseState* tmpState = itr->second;
			m_states.erase(itr);
			m_states.emplace_back(tmpType, tmpState);
			m_states.back().second->Activate();

			return;
		}
	}

	if (!m_states.empty()) {
		m_states.back().second->Deactivate();
	}

	CreateState(l_type);
	m_states.back().second->Activate();
}

void StateManager::CreateState(const StateType& l_type) {
	auto newState = m_stateFactory.find(l_type);

	//State is not registered
	if (newState == m_stateFactory.end())
		return;

	BaseState* l_state = newState->second(); //call function to create new base state
	m_states.emplace_back(l_type, l_state);
	l_state->OnCreate();
}

void StateManager::RemoveState(const StateType& l_type) {
	for (auto itr = m_states.begin(); itr < m_states.end(); itr++) {
		if (itr->first == l_type) {
			itr->second->OnDestroy();
			delete itr->second;
			m_states.erase(itr);
			return;
		}
	}
}

