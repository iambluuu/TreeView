#pragma once
#include "StateManager.h"
#include "Node.h"
#include "TextureManager.h"

struct SharedContext;

using NodeGraphics = std::pair<sf::Sprite*, sf::Sprite*>;

class NodeRenderer {
private:
	std::vector<NodeGraphics> m_spriteMap;
	StateManager* m_stateManager;
	sf::Color m_selectedColor;
	sf::Color m_defaultColor;

	const float STEP_DURATION = 0.7f;
	float m_animationCurrent{ 0.f };
	float m_speedupRate{ 0.f };

public:
	NodeRenderer() {
	}

	~NodeRenderer();

	void Update(const float& l_fT);
	void Reset();
	
	void DrawNode(Node* Cur);
	void DrawArrow();
	void SplitNode(Node* Cur, sf::Time l_time);

	int GetStep();
};