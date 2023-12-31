#include "CodeWindow.h"

const sf::Color LightBlue = sf::Color(92, 113, 151);
const sf::Color DarkBlue = sf::Color(59, 73, 98);

const float TOP_LINE = 645;
const float LEFT_LINE = 1180;

float CodeWindow::GetLinePosition(int line) {
	return TOP_LINE + line * 30;
}

void CodeWindow::Reset() {
	if (!m_lines.empty())
		m_lines.clear();
}

void CodeWindow::SetStateManager(StateManager* l_owner) {
	m_owner = l_owner;
}

void CodeWindow::SwitchTheme(int l_theme) {
	if (l_theme == 0) {
		DefaultColor = LightBlue;
		SelectedColor = DarkBlue;
		HighlightColor = LightBlue;

		m_highlight.setFillColor(HighlightColor);
	}
	else {
		
	}
}

void CodeWindow::MoveHighlight(const int& l_line) {
	if (m_lines.empty()) {
		m_lines.push_back(std::make_pair(-1, l_line));
	}
	else {
		std::pair<int, int> temp = { m_lines.back().second, l_line };
		m_lines.push_back(temp);
	}
}

void CodeWindow::Stay() {
	if (!m_lines.empty()) {
		m_lines.push_back(std::make_pair(m_lines.back().second, m_lines.back().second));
	}
	else {
		m_lines.push_back(std::make_pair(-1, -1));
	}
}

void CodeWindow::Draw(int step, float percent, sf::RenderWindow* l_window) {
	if (m_lines.empty()) {
		return;
	}

	if (step >= m_lines.size()) {
		step = m_lines.size() - 1;
		percent = 1;
	}

	if (step < 0) {
		step = 0;
		percent = 0;
	}

	int previous_line = m_lines[step].first;
	int selected_line = m_lines[step].second;

	sf::Vector2f offset = m_owner->GetContext()->m_wind->GetOffset();
	float leftPos = LEFT_LINE + offset.x;

	// Draw the highlight bar////////////////////////////////////
	if (previous_line == -1 && selected_line != -1 && (previous_line != selected_line)) {
		m_highlight.setPosition(leftPos, GetLinePosition(selected_line));
		m_highlight.setFillColor(GetColorTransition(percent, sf::Color(255, 255, 255, 0), HighlightColor));
		l_window->draw(m_highlight);
	}
	else if (previous_line != -1 && selected_line == -1 && (previous_line != selected_line)) {
		m_highlight.setPosition(leftPos, GetLinePosition(previous_line));
		m_highlight.setFillColor(GetColorTransition(percent, HighlightColor, sf::Color(255, 255, 255, 0)));
		l_window->draw(m_highlight);
	}
	else if (selected_line != -1) {
		float prePos = GetLinePosition(previous_line);
		float curPos = GetLinePosition(selected_line);

		m_highlight.setFillColor(HighlightColor);
		m_highlight.setPosition(leftPos, prePos + (curPos - prePos) * percent);
		l_window->draw(m_highlight);
	}

	//Draw text///////////////////////////////////////////////
 	for (int i = 0; i < m_code.size(); i++) {
		m_text.setString(m_code[i]);
		m_text.setPosition(leftPos, GetLinePosition(i));

		if (i != previous_line && i != selected_line) {
			m_text.setFillColor(DefaultColor);
		}
		else if (i == previous_line && i != selected_line) {
			m_text.setFillColor(GetColorTransition(percent, SelectedColor, DefaultColor));
		}
		else if (i != previous_line && i == selected_line) {
			m_text.setFillColor(GetColorTransition(percent, DefaultColor, SelectedColor));
		}
		else {
			m_text.setFillColor(SelectedColor);
		}

		l_window->draw(m_text);
	}
}

sf::Color CodeWindow::GetColorTransition(float percent, const sf::Color& start, const sf::Color& end) {
	sf::Color res = sf::Color(0, 0, 0);
	if (start.r > end.r) {
		res.r = start.r - (start.r - end.r) * percent;
	}
	else {
		res.r = start.r + (end.r - start.r) * percent;
	}

	if (start.g > end.g) {
		res.g = start.g - (start.g - end.g) * percent;
	}
	else {
		res.g = start.g + (end.g - start.g) * percent;
	}

	if (start.b > end.b) {
		res.b = start.b - (start.b - end.b) * percent;
	}
	else {
		res.b = start.b + (end.b - start.b) * percent;
	}

	if (start.a > end.a) {
		res.a = start.b - (start.b - end.a) * percent;
	}
	else {
		res.a = start.a + (end.a - start.a) * percent;
	}

	return res;
}