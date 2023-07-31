#pragma once
#include "SharedContext.h"
#include "StateManager.h"
#include "Node.h"

struct SharedContext;
class StateManager;
class ThemeManager;

using NodeGraphics = std::pair<sf::Sprite, sf::Sprite>;
using NodeColor = std::vector<std::map<NodeState, std::tuple<sf::Color, sf::Color, sf::Color>>>;

class NodeRenderer {
private:
	std::vector<NodeGraphics> m_nodeGraphics;
	NodeColor m_nodeColor;

	StateManager* m_stateManager{ nullptr };
	//ThemeManager* m_themeManager{ nullptr };


	sf::Font m_font;
	sf::Texture m_texture;

	sf::Text m_label;

	const float STEP_DURATION{ 500.f };
	float m_animationCurrent{ 0.f };
	float m_speedupRate{ 0.f };
	int m_stepNum{ 0 };

	const float TOP_LINE = 250;
	const float MIDDLE_LINE = 592;
	const float HORIZONTAL_SPACING = 46;
	const float VERTICAL_SPACING = 70;

	bool is_paused{ false };
	bool is_reverse{ false };

public:
	NodeRenderer(StateManager* l_manager) {
		m_stateManager = l_manager;
		m_nodeGraphics.resize(4);
		m_nodeColor.resize(4);

		m_texture.loadFromFile("Assets/Texture/NodeSheet.png");
		m_font.loadFromFile("Assets/Font/monofonto rg.otf");

		m_label.setFont(m_font);
		m_label.setCharacterSize(20);
		
		PrepareSprite();
	}

	~NodeRenderer() {}

	void PrepareSprite();
	NodeGraphics* GetNodeGraphics(int valueNum);

	void Update(const float& l_fT);
	void Reset(const int& l_stepNum);
	
	void SplitNode(const NodeInfo& l_info, float percent);
	void ExpandNode(const NodeInfo& l_info, float percent);
	void SpawnNode(const NodeInfo& l_info, float percent);
	void ChangeState(const NodeInfo& l_info, float percent);
	void DespawnNode(const NodeInfo& l_info, float percent);
	void MoveNode(const NodeInfo& l_info, float percent);
	
	void DrawTree(Node* Root);
	void DrawNode(Node* Cur);
	void DrawArrow();

	std::tuple<sf::Color, sf::Color, sf::Color>* GetNodeColor(int ThemeID, NodeState l_state);
	sf::Vector2f GetPosOnScreen(std::pair<int, int> treeCoord);
	sf::Color GetColorTransition(float percent, const sf::Color& start, const sf::Color& end);
	int GetStep();
	void SetStepNum(int val) {
		m_stepNum++;
	}
};