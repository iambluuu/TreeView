#pragma once
#include "StateManager.h"

class StateManager;
class NodeRenderer;

class CodeWindow {
private:
	std::vector<std::pair<int, int> > m_lines;
	std::vector<std::string> m_code;

	sf::Font font;

	sf::Color SelectedColor{ sf::Color::White };
	sf::Color DefaultColor{ sf::Color::Black };
	sf::Color HighlightColor{ sf::Color::White };

	sf::Text m_text;
	sf::RectangleShape m_highlight;

	StateManager* m_owner;

	float GetLinePosition(int line);
	
public:
	CodeWindow() {
		m_highlight.setSize(sf::Vector2f(420, 30));
		
		font.loadFromFile("Assets/Font/monofonto rg.otf");
		m_text.setFont(font);
		m_text.setCharacterSize(20);
	}

	~CodeWindow() {
		m_lines.clear();
	}

	void SetStateManager(StateManager* l_owner);

	void LoadCode(std::vector<std::string> l_code) {
		m_code = l_code;
	}

	void SwitchTheme(int l_theme);

	void Reset(); // Reset steps
	void Clear() {
		if (!m_lines.empty())
			m_lines.clear();

		if (!m_code.empty())
			m_code.clear();
	}// Clear strings

	void Draw(int step, float percent, sf::RenderWindow* l_window);
	void MoveHighlight(const int& l_line);
	void Stay();

	sf::Color GetColorTransition(float percent, const sf::Color& start, const sf::Color& end);
};