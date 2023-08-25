#pragma once
#include "TextLine.h"
#include "Window.h"

#include <vector>
#include <memory>

class Window;

class Page
{
	friend class Window;
public:
	Page(Window* l_window);

	void AddCharToCurrentLine(char c);
	void HandleNewLine();
	void DeleteCurrentLine();
	void MoveCursorLeft();
	void MoveCursorRight();
	void MoveDownLine();
	void MoveUpLine();

	void DeleteCharacterAfterCursor();
	void DeleteCharacterAtCursor();

	void GetCursorXY(float& x, float& y);

	void HandleEvent(sf::Event* l_event);
	void Update();
	void Draw();

	std::vector<std::string> GetContent();

	void Open() {
		isOpen = true;
	}

	void Close() {
		isOpen = false;
	}
private:
	int m_LineIndex = -1;
	std::vector<std::unique_ptr<TextLine>> m_Lines;
	Window* m_Window;

	sf::Font m_font;
	sf::RectangleShape m_Cursor;

	sf::IntRect m_Bounds;

	const int blinkAfterFrames = 30;
	const int blinkTime = 30;
	bool isBlinking = false;
	int frameCounter = 0;

	bool isOpen = false;
};