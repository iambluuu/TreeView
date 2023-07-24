#include "NodeRenderer.h"
#include <algorithm>

void NodeRenderer::PrepareSprite() {
	sf::Sprite Border(m_texture);
	sf::Sprite Fill(m_texture);

	int sizeX = 46;
	int sizeY = 46;
	Border.setTextureRect(sf::IntRect(0, 0, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, 0, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics.push_back(std::make_pair(Border, Fill));

	sizeX = 92;
	Border.setTextureRect(sf::IntRect(0, 0, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, 0, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics.push_back(std::make_pair(Border, Fill));

	sizeX = 120;
	Border.setTextureRect(sf::IntRect(0, 0, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, 0, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics.push_back(std::make_pair(Border, Fill));

	sizeX = 160;
	Border.setTextureRect(sf::IntRect(0, 0, sizeX, sizeY));
	Fill.setTextureRect(sf::IntRect(sizeX, 0, sizeX, sizeY));
	Border.setOrigin(Border.getLocalBounds().left + Border.getLocalBounds().width / 2, Border.getLocalBounds().top + Border.getLocalBounds().height / 2);
	Fill.setOrigin(Fill.getLocalBounds().left + Fill.getLocalBounds().width / 2, Fill.getLocalBounds().top + Fill.getLocalBounds().height / 2);

	m_nodeGraphics.push_back(std::make_pair(Border, Fill));
}

void NodeRenderer::Reset(const int& l_stepNum) {
	m_animationCurrent = 0;
	m_stepNum = l_stepNum;
}

void NodeRenderer::Update(const float& l_fT) {
	m_animationCurrent = std::min(m_animationCurrent + l_fT, m_stepNum * STEP_DURATION);
}

int NodeRenderer::GetStep() {
	return floor(m_animationCurrent / STEP_DURATION);
}

NodeGraphics* NodeRenderer::GetNodeGraphics(int valueNum) {
	return &m_nodeGraphics[valueNum - 1];
}

void NodeRenderer::SpawnNode() {
	
}

void NodeRenderer::DrawNode(Node* Cur) {
	sf::RenderWindow* wind = m_stateManager->GetContext()->m_wind->GetRenderWindow();
	std::vector<NodeInfo>* info = Cur->getInfo();

	int i = GetStep();
	NodeInfo CurInfo = info->at(i);
	float CurStepElapsed = m_animationCurrent - STEP_DURATION * i;

	if (!CurInfo.is_visible)
		return;

	SharedContext* context = m_stateManager->GetContext();
	sf::RenderWindow* wind = context->m_wind->GetRenderWindow();

	NodeGraphics* CurSprite = GetNodeGraphics(CurInfo.value_num); //do something here 
	sf::Sprite* BorderSprite = &CurSprite->first;
	sf::Sprite* FillerSprite = &CurSprite->second;

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

	if (CurInfo.is_splitting) {

		return;
	}

	if (CurInfo.is_moving) {
		
		return;
	}

	if (CurInfo.is_appearing) {
		SpawnNode();
		return;
	}

	if (CurInfo.is_stateChanging) {

	}

	wind->draw(*BorderSprite);
	wind->draw(*FillerSprite);

	float TopPos = CurInfo.m_coord.y + (BorderSprite->getLocalBounds().height / 2);
	float LeftPos = CurInfo.m_coord.x - (BorderSprite->getLocalBounds().width / 2);
	float spacing = BorderSprite->getLocalBounds().width / (CurInfo.value_num + 1);

	for (int j = 0; j < CurInfo.value_num; j++) {
		LeftPos += spacing;

		m_label.setString(std::to_string(CurInfo.m_shownValue[j]));
		m_label.setOrigin(m_label.getLocalBounds().left + m_label.getLocalBounds().width / 2, m_label.getLocalBounds().top + m_label.getLocalBounds().height / 2);
		m_label.setPosition(sf::Vector2f(LeftPos, TopPos));

		wind->draw(m_label);
	}

}