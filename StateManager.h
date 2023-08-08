#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SharedContext.h"

#include <unordered_map>
#include <functional>

enum class StateType {
	Menu = 1, Hash_Table, Heap, AVLTree, TTFTree, Trie, Graph, Exit
};

class StateManager;
struct SharedContext;

//Clase BaseState
class BaseState {
	friend class StateManager;

protected:
	StateManager* m_stateManager;
	bool m_transparent;
	bool m_transcendent;

public:
	BaseState() {}
	BaseState(StateManager* l_stateManager) :m_stateManager(l_stateManager), m_transparent(false), m_transcendent(false) {}

	virtual ~BaseState() {}

	StateManager* GetStateManager() {
		return m_stateManager;
	}

	virtual void OnCreate(const std::string& l_numbers, const std::string& l_value) = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void HandleEvent(sf::Event* l_event) = 0;
	virtual void Update(const sf::Time& l_time) = 0;
	virtual void Draw() = 0;

	virtual void OnInsert(const std::string& l_value) = 0;
	virtual void OnRemove(const std::string& l_value) = 0;
	
	void setTransparent(const bool& l_transparent) {
		m_transparent = l_transparent;
	}

	void setTranscendent(const bool& l_transcendent) {
		m_transcendent = l_transcendent;
	}

	bool IsTransparent() {
		return m_transparent;
	}

	bool IsTranscendent() {
		return m_transcendent;
	}
};

using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

//Clase StateManager

class StateManager {
private:
	SharedContext* m_context;
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;

	void CreateState(const StateType& l_type);
	void RemoveState(const StateType& l_type);  

	template<class T>
	void RegisterState(const StateType& l_type) {
		m_stateFactory[l_type] = [this]() -> BaseState*
		{
			return new T (this);
		};
	}

public:
	StateManager(SharedContext* l_context);
	~StateManager();

	void HandleEvent(sf::Event* l_event);
	void Update(const sf::Time& l_time);
	void Draw();

	void ProcessRequests();
	SharedContext* GetContext();

	BaseState* GetState(const StateType& l_type);
	bool HasState(const StateType& l_type);
	void SwitchTo(const StateType& l_type);
	void Remove(const StateType& l_type);
};

