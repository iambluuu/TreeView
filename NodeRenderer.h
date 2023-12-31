#pragma once
#include "SharedContext.h"
#include "StateManager.h"
#include "CodeWindow.h"
#include "Node.h"

struct SharedContext;
class StateManager;
class ThemeManager;

using NodeGraphics = std::pair<sf::Sprite, sf::Sprite>;
using NodeColor = std::vector<std::map<NodeState, std::tuple<sf::Color, sf::Color, sf::Color>>>;

enum class StateType;

class CodeWindow;

class NodeRenderer {
private:
	CodeWindow* m_codeWindow{ nullptr };

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

	int m_theme{ 0 };

	const float STEP_DURATION{ 500.f };
	float m_animationCurrent{ 0.f };
	float m_speedupRate{ 1.f };
	int m_stepNum{ 0 };
	int m_curStep{ 0 };
	int m_limitStep{ 0 };

	const float TOP_LINE = 200;
	const float MIDDLE_LINE = 592 + 1500;
	const float HORIZONTAL_SPACING = 56;
	const float VERTICAL_SPACING = 70;

	bool is_paused{ false };
	bool is_reverse{ false };
	bool step_by_step{ false };

	float parametric(float time) {
		int alpha = 2;

		float left = 1.0f, right = 1.0f;
		while (alpha--) {
			left *= time;
			right *= (1.0f - time);
		}

		return left / (left + right);
	}

public:
	NodeRenderer(StateManager* l_manager);
	~NodeRenderer();

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
	
	void DrawTree(Node* Root);
	void DrawNode(Node* Cur, bool Directed);
	void DrawArrow();

	void DrawGraph(GraphNode* Root);
	void DrawGraphNode(GraphNode* Root);

	void DrawCodeWindow();

	std::tuple<sf::Color, sf::Color, sf::Color>* GetNodeColor(int ThemeID, NodeState l_state);

	sf::Vector2f GetPosOnScreen(std::pair<float, float> treeCoord);
	sf::Vector2f GetPosOnScreen(sf::Vector2f treecoord);
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

	void SwitchTheme(int l_theme);

	void ResetCodeWindow();

	CodeWindow* GetCodeWindow();
};