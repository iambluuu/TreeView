#pragma once
#include "NodeRenderer.h"

class CodeWindow {
private:
	std::vector<std::pair<int, int> > m_lines;
	std::vector<std::string> m_code;

	sf::Color SelectedColor{ sf::Color::White };
	sf::Color DefaultColor{ sf::Color::Black };
	sf::Color HighlightColor{ sf::Color::White };

	sf::Text m_text;
	sf::RectangleShape m_highlight;

	sf::RenderWindow* m_window{ nullptr };

	float GetLinePosition(int line);
	
public:
	CodeWindow() {
		m_highlight.setSize(sf::Vector2f(420, 30));
	}

	void setWindow(sf::RenderWindow* l_window) {
		m_window = l_window;
	}

	void LoadCode(std::vector<std::string> l_code) {
		m_code = l_code;
	}

	void SwitchTheme(int l_theme);

	void Reset();
	void Draw(int step, float percent);
	void MoveHighlight(const int& l_line);

	sf::Color GetColorTransition(float percent, const sf::Color& start, const sf::Color& end);
};