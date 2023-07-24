#pragma once
#include "StateManager.h"
#include "Node.h"
#include "TextureManager.h"

struct SharedContext;

using NodeGraphics = std::pair<sf::Sprite, sf::Sprite>;

class NodeRenderer {
private:
	std::vector<NodeGraphics> m_nodeGraphics;
	StateManager* m_stateManager;

	sf::Color m_selectedColor{ sf::Color(103, 137, 131) };
	sf::Color m_defaultColor{ sf::Color::Black };
	sf::Color m_newColor{ sf::Color(103, 137, 131) };

	sf::Font m_font;
	sf::Texture m_texture;

	sf::Text m_label;

	const float STEP_DURATION{ 0.7f };
	float m_animationCurrent{ 0.f };
	float m_speedupRate{ 0.f };
	int m_stepNum{ 0 };

public:
	NodeRenderer(StateManager* l_manager) {
		m_stateManager = l_manager;
		m_nodeGraphics.reserve(4);

		m_texture.loadFromFile("Resource/Texture/NodeSheet.png");
		m_font.loadFromFile("Resource/Font/OpenSans-SemiBold.ttf");

		m_label.setFont(m_font);
		m_label.setCharacterSize(20);
		m_label.setFillColor(sf::Color::Black);
		
		PrepareSprite();
	}

	~NodeRenderer();

	void PrepareSprite();
	NodeGraphics* GetNodeGraphics(int valueNum);

	void Update(const float& l_fT);
	void Reset(const int& l_stepNum);
	
	void SplitNode();
	void ExpandNode();
	void SpawnNode();
	void DespawnNode();
	
	void DrawNode(Node* Cur);
	void DrawArrow();
	void SplitNode(Node* Cur, sf::Time l_time);

	int GetStep();
};