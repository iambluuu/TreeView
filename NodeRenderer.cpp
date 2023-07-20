#include "NodeRenderer.h"

int NodeRenderer::GetStep() {
	return floor(m_animationCurrent / STEP_DURATION);
}

void NodeRenderer::DrawNode(Node* Cur) {
	std::vector<NodeInfo>* info = Cur->getInfo();

	int i = GetStep();
	NodeInfo CurInfo = info->at(i);

	if (!CurInfo.is_visible)
		return;

	SharedContext* context = m_stateManager->GetContext();
	sf::RenderWindow* wind = context->m_wind->GetRenderWindow();

	std::pair<sf::Sprite, sf::Sprite> CurSprite = context->m_textureManager->GetNodeSprite(CurInfo.value_num);
	sf::Sprite* BorderSprite = &CurSprite.first;
	sf::Sprite* FillerSprite = &CurSprite.second;

	BorderSprite->setOrigin(BorderSprite->getLocalBounds().width / 2, BorderSprite->getLocalBounds().height / 2);
	FillerSprite->setOrigin(FillerSprite->getLocalBounds().width / 2, FillerSprite->getLocalBounds().height / 2);

	BorderSprite->setPosition(CurInfo.m_coord);
	FillerSprite->setPosition(CurInfo.m_coord);

	switch (CurInfo.node_state) {
	case Visited:
		BorderSprite->setColor(m_selectedColor);
		FillerSprite->setColor(sf::Color::White);
		break;
	case Selected:
		BorderSprite->setColor(m_selectedColor);
		FillerSprite->setColor(m_selectedColor);
		break;
	case New:
	case InRemove:
	default:
	}
}