#pragma once
#include "StateManager.h"

class Hash_Table : public BaseState {
private:
	sf::Sprite m_sprite;
	float m_elapsed;

public:
	~Hash_Table();
	
	void Update(const sf::Time& l_time);
	void Draw();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

};