#pragma once
#include "SharedContext.h"
#include "Node.h"

struct SharedContext;

using NodeGraphics = std::pair<sf::Sprite*, sf::Sprite*>;

class NodeRenderer {
private:
	std::vector<NodeGraphics> m_spriteMap;
	SharedContext* m_context;
	sf::Color m_selectedColor;
	sf::Color m_defaultColor;

public:
	NodeRenderer(SharedContext* l_context) {
		m_context = l_context;
	}

	~NodeRenderer();

	void DrawNode(Node* Cur);
	void DrawArrow();
	void SplitNode(Node* Cur, sf::Time l_time);
};