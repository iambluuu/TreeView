#pragma once
#include "SharedContext.h"

struct SharedContext;

class NodeRenderer {
private:
	std::vector<std::pair<sf::Sprite*, sf::Sprite*> > m_spriteMap;
	SharedContext* m_context;

public:
	NodeRenderer(SharedContext* l_context) {
		m_context = l_context;

	}
};