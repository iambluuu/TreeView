#pragma once
#include "StateManager.h"

class Menu : public BaseState {
public:
	Menu(StateManager* stateManager) : BaseState(stateManager) {}
	~Menu() {}

	void OnCreate() {}
	void OnDestroy() {}

	void Activate() {}
	void Deactivate() {}

	void HandleEvent(sf::Event* l_event) {}
	void Update(const sf::Time& time) {}
	void Draw() {}

	void OnInsert(const std::string& l_value) {}
	void OnRemove(const std::string& l_value) {}
};