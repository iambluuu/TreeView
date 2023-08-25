#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "Page.h"

const float LEFT = (1600 - 720) / 2;
const float TOP = (900 - 720) / 2;
const float WIDTH = 720.f;
const float HEIGHT = 720.f;

const sf::Color DarkBeige = sf::Color(136, 132, 122);
const sf::Color LightBeige = sf::Color(240, 233, 210);
const sf::Color Green = sf::Color(103, 137, 131);

Page::Page(Window* l_window) : m_Window(l_window)
{
	m_Cursor.setSize(sf::Vector2f(5, 20));
	m_font.loadFromFile("Assets/Font/monofonto rg.otf");

	m_Bounds = sf::IntRect(LEFT, TOP, WIDTH, HEIGHT);
	HandleNewLine();
}

void Page::AddCharToCurrentLine(char c)
{
	if (m_LineIndex > -1)
	{
		if (m_Lines[m_LineIndex]->ToString().size() > 50) {
			if (m_LineIndex == m_Lines.size() - 1 && m_LineIndex < 34)
				HandleNewLine();
			else if (m_LineIndex < m_Lines.size() - 1) {
				MoveDownLine();
			}
			else
				return;
		}

		m_Lines[m_LineIndex]->InsertCharacterAtCursor(c);
	}
}

void Page::HandleNewLine()
{
	if (m_LineIndex == m_Lines.size() - 1)
	{
		m_Lines.emplace_back(std::make_unique<TextLine>());
		m_LineIndex = m_Lines.size() - 1;
	}
	else
	{
		m_Lines.insert(m_Lines.begin() + m_LineIndex + 1, std::make_unique<TextLine>());
		m_LineIndex++;
	}
}

void Page::DeleteCurrentLine()
{
	if (m_Lines.size() > 1)
	{
		m_Lines.erase(m_Lines.begin() + m_LineIndex);
		if (m_LineIndex == m_Lines.size())
		{
			m_LineIndex--;
		}
	}
}

void Page::MoveCursorLeft()
{
	if (m_LineIndex > -1)
	{
		m_Lines[m_LineIndex]->MoveCursorLeft();
	}
}

void Page::MoveCursorRight()
{
	if (m_LineIndex > -1)
	{
		m_Lines[m_LineIndex]->MoveCursorRight();
	}
}

void Page::MoveDownLine()
{
	if (m_LineIndex < m_Lines.size() - 1)
	{
		m_LineIndex++;
	}
}

void Page::MoveUpLine()
{
	if (m_LineIndex > 0)
	{
		m_LineIndex--;
	}
}

void Page::DeleteCharacterAfterCursor()
{
	if (m_LineIndex > -1)
	{
		if (m_Lines[m_LineIndex]->Size() == 0)
		{
			DeleteCurrentLine();
			return;
		}
		m_Lines[m_LineIndex]->DeleteCharacterAfterCursor();
	}
}

void Page::DeleteCharacterAtCursor()
{
	if (m_LineIndex > -1)
	{
		if (m_Lines[m_LineIndex]->Size() == 0)
		{
			DeleteCurrentLine();
			return;
		}
		m_Lines[m_LineIndex]->DeleteCharacterAtCursor();
	}
}

void Page::HandleEvent(sf::Event* l_event) {
	if (!isOpen)
		return;

	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_Window->GetRenderWindow());

	if (l_event->type == sf::Event::MouseButtonPressed && l_event->key.code == sf::Mouse::Left && !m_Bounds.contains(mousePos)) {
		Close();
	}

	if (l_event->type == sf::Event::KeyPressed) {
		if (l_event->key.code == sf::Keyboard::Escape || l_event->key.code == sf::Keyboard::Tab)
			Close();

		if (l_event->key.code == sf::Keyboard::Right)
		{
			MoveCursorRight();
			isBlinking = false;
			frameCounter = 0;
		}

		if (l_event->key.code == sf::Keyboard::Left)
		{
			MoveCursorLeft();
			isBlinking = false;
			frameCounter = 0;
		}

		if (l_event->key.code == sf::Keyboard::Up)
		{
			MoveUpLine();
			isBlinking = false;
			frameCounter = 0;
		}

		if (l_event->key.code == sf::Keyboard::Down)
		{
			MoveDownLine();
			isBlinking = false;
			frameCounter = 0;
		}

		if (l_event->key.code == sf::Keyboard::Delete)
		{
			DeleteCharacterAfterCursor();
			isBlinking = false;
			frameCounter = 0;
		}

		if (l_event->key.code == sf::Keyboard::Backspace)
		{
			DeleteCharacterAtCursor();
			isBlinking = false;
			frameCounter = 0;
		}

		if (l_event->key.code == sf::Keyboard::Enter)
		{
			if (m_Lines.size() < 35)
				HandleNewLine();
			isBlinking = false;
			frameCounter = 0;
		}
	}
	else if (l_event->type == sf::Event::TextEntered) {
		auto key = l_event->text.unicode;

		if ((key >= 32) && (key <= 125))
		{
			AddCharToCurrentLine((char)key);
			isBlinking = false;
			frameCounter = 0;
		}
	}
}

void Page::Update()
{
	frameCounter++;
	if (!isBlinking && frameCounter > blinkAfterFrames)
	{
		isBlinking = true;
		frameCounter = 0;
	}

	if (isBlinking)
	{
		frameCounter = frameCounter % (2 * blinkTime);
	}
}

void Page::Draw()
{
	if (!isOpen)
		return;

	sf::RenderWindow* window = m_Window->GetRenderWindow();
	sf::Vector2f offset = m_Window->GetOffset();

	//Dim Background
	sf::RectangleShape background(sf::Vector2f(1600, 900));
	background.setFillColor(sf::Color(0, 0, 0, 100));
	background.setPosition(offset);
	window->draw(background);

	//Draw Page
	static char buffer[20];

	sf::RectangleShape LineNumberBackground(sf::Vector2f(30, 720));
	LineNumberBackground.setFillColor(DarkBeige);
	LineNumberBackground.setPosition(LEFT + offset.x, TOP + offset.y);
	window->draw(LineNumberBackground);

	sf::RectangleShape LineBackground(sf::Vector2f(720 - 30, 720));
	LineBackground.setFillColor(Green);
	LineBackground.setPosition(LEFT + 30 + offset.x, TOP + offset.y);
	window->draw(LineBackground);

	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(20);

	for (int i = 0; i < m_Lines.size(); ++i)
	{
		std::string lineNumber = std::to_string(i + 1);

		text.setString(lineNumber);
		int width = text.getLocalBounds().width;
		text.setPosition(LEFT + 25 - width + offset.x, TOP + i * 20 + 10 + offset.y);

		window->draw(text);
	}

	for (int i = 0; i < m_Lines.size(); ++i)
	{
		std::string lineText = m_Lines[i]->ToString();
		text.setString(lineText);
		text.setPosition(LEFT + 35 + offset.x, TOP + i * 20 + 10 + offset.y);

		window->draw(text);
	}

	float cursorX = 0.f, cursorY = 0.f;
	GetCursorXY(cursorX, cursorY);
	if (isBlinking)
	{
		if (frameCounter < blinkTime)
		{
			m_Cursor.setPosition(LEFT + cursorX + offset.x + 35, TOP + cursorY + offset.y + 10);
			window->draw(m_Cursor);
		}
	}
	else
	{
		m_Cursor.setPosition(LEFT + cursorX + offset.x + 35, TOP + cursorY + offset.y + 10);
		window->draw(m_Cursor);
	}
}

std::vector<std::string> Page::GetContent() {
	std::vector<std::string> content;
	for (int i = 0; i < m_Lines.size(); ++i)
	{
		std::string line = m_Lines[i]->ToString();
		if (line.size() > 0)
			content.push_back(line);
	}

	return content;
}

void Page::GetCursorXY(float& x, float& y)
{
	if (m_LineIndex > -1)
	{
		std::string text = m_Lines[m_LineIndex]->ToString();
		sf::Text tempText;
		tempText.setFont(m_font);
		tempText.setCharacterSize(20);
		tempText.setString(text.substr(0, m_Lines[m_LineIndex]->CursorIndex()).c_str());

		x = tempText.getLocalBounds().width + 3;
		y = m_LineIndex * 20 + 3;
	}
	else
	{
		x = -100;
		y = -200;
	}
}