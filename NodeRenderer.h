#pragma once
#include "SharedContext.h"
#include "StateManager.h"
#include "Node.h"

struct SharedContext;
class StateManager;
class ThemeManager;

using NodeGraphics = std::pair<sf::Sprite, sf::Sprite>;
using NodeColor = std::vector<std::map<NodeState, std::tuple<sf::Color, sf::Color, sf::Color>>>;

enum class StateType;

class NodeRenderer {
private:
	std::vector<NodeGraphics> m_nodeGraphics;
	sf::Sprite m_arrowSprite;
	NodeColor m_nodeColor;

	StateManager* m_stateManager{ nullptr };
	//ThemeManager* m_themeManager{ nullptr };

	StateType m_curState;

	sf::Font m_font;
	sf::Texture m_texture;
	sf::Texture m_arrowTexture;

	sf::Text m_label;
	sf::Text m_sideLabel;

	sf::RectangleShape m_line;

	const float STEP_DURATION{ 500.f };
	float m_animationCurrent{ 0.f };
	float m_speedupRate{ 1.f };
	int m_stepNum{ 0 };
	int m_curStep{ 0 };
	int m_limitStep{ 0 };

	const float TOP_LINE = 250;
	const float MIDDLE_LINE = 592 + 1500;
	const float HORIZONTAL_SPACING = 46;
	const float VERTICAL_SPACING = 70;

	bool is_paused{ false };
	bool is_reverse{ false };
	bool step_by_step{ false };

public:
	NodeRenderer(StateManager* l_manager) {
		m_stateManager = l_manager;
		m_nodeGraphics.resize(4);
		m_nodeColor.resize(4);

		m_texture.loadFromFile("Assets/Texture/NodeSheet.png");
		m_texture.setSmooth(1);

		m_arrowTexture.loadFromFile("Assets/Texture/Arrow.png");
		m_arrowTexture.setSmooth(1);

		m_font.loadFromFile("Assets/Font/monofonto rg.otf");

		m_label.setFont(m_font);
		m_label.setCharacterSize(20);
		m_sideLabel.setFont(m_font);
		m_sideLabel.setCharacterSize(15);
		
		PrepareSprite();
	}

	~NodeRenderer() {}

	void PrepareSprite();
	NodeGraphics* GetNodeGraphics(int valueNum);

	void OnForward();
	void OnBackward();
	void OnPlay();
	void OnSkipForward();
	void OnSkipBackward();
	void OnReplay();

	void HandleEvent(sf::Event* l_event);
	void Update(const float& l_fT);
	void Reset(const int& l_stepNum);
	
	void SplitNode(const NodeInfo& l_info, float percent);
	void ExpandNode(const NodeInfo& l_info, float percent);
	void SpawnNode(const NodeInfo& l_info, float percent);
	void ChangeState(const NodeInfo& l_info, float percent);
	void DespawnNode(const NodeInfo& l_info, float percent);
	void MoveNode(const NodeInfo& l_info, float percent);
	
	void DrawTree(Node* Root);
	void DrawNode(Node* Cur, bool Directed);
	void DrawArrow();

	std::tuple<sf::Color, sf::Color, sf::Color>* GetNodeColor(int ThemeID, NodeState l_state);
	sf::Vector2f GetPosOnScreen(std::pair<int, int> treeCoord);
	sf::Color GetColorTransition(float percent, const sf::Color& start, const sf::Color& end);
	int GetStep();

	bool IsPaused() {
		return is_paused;
	}

	void SetStepNum(int val) {
		m_stepNum++;
	}

	void SetSpeedupRate(float val) {
		m_speedupRate = val;
	}

	void SetStepByStep(bool val) {
		step_by_step = val;
	}

	float GetProgress() {
		if (m_stepNum == 0)
			return 0.f;

		return m_animationCurrent / (STEP_DURATION * m_stepNum);
	}

	void SetProgress(float percent) {
		m_animationCurrent = percent * STEP_DURATION * m_stepNum;
		m_curStep = GetStep();
	}

	void SetState(StateType l_type) {
		m_curState = l_type;
	}
};