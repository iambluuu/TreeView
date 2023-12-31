#include "Window.h"

Window::Window() {
	m_fps = 60;
	Setup("Window", sf::Vector2u(1600, 900));
	m_page = new Page(this);
}

Window::Window(const std::string& l_title, const sf::Vector2u& l_size) : m_view(sf::FloatRect(0, 0, 1600, 900)) {
	m_fps = 60;
	Setup(l_title, l_size);
	m_page = new Page(this);
}

Window::~Window() {
	Destroy();
}

void Window::Setup(const std::string& l_title, const sf::Vector2u& l_size) {
	m_windowSize = l_size;
	m_windowTitle = l_title;
	m_isFullscreen = false;
	m_isDone = false;

	Create();
	
	MoveView(DefaultOffset);
	m_window.setView(m_view);
}

void Window::Create() {
	auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Close);
	m_window.create({ m_windowSize.x, m_windowSize.y, 32 }, m_windowTitle, style);
	m_window.setFramerateLimit(m_fps);
}

void Window::Destroy() {
	m_window.close();
	delete m_page;
}

void Window::ToggleFullscreen() {
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

void Window::BeginDraw() {
	int themeID = m_stateManager->GetContext()->m_uiManager->GetThemeID();
	auto color = m_stateManager->GetContext()->m_uiManager->GetThemeManager()->GetColor(themeID, ElementName::BackgroundColor, ElementState::Neutral);

	m_window.clear(*color);
}

bool Window::IsDone() {
	return m_isDone;
}

bool Window::IsFullscreen() {
	return m_isFullscreen;
}

void Window::EndDraw() {
	m_window.display();
}

void Window::Draw(sf::Drawable& l_drawable) {
	m_window.draw(l_drawable);
}

void Window::DrawPage() {
	if (!m_page->isOpen)
		return;

	m_page->Draw();
}

void Window::Update() {
	sf::Event E;
	
	m_cursorType = 0;

	while (m_window.pollEvent(E)) {
		if (E.type == sf::Event::Closed) {	
			m_isDone = 1;
			return;
		}

		if (m_page->isOpen) {
			m_page->HandleEvent(&E);
			return;
		}

		if (E.type == sf::Event::KeyPressed && !m_isOverlayed) {
			switch (E.key.code) {
			case sf::Keyboard::W:
				MoveView(sf::Vector2f(0, -5));
				break;
			case sf::Keyboard::A:
				MoveView(sf::Vector2f(-5, 0));
				break;
			case sf::Keyboard::S:
				MoveView(sf::Vector2f(0, 5));
				break;
			case sf::Keyboard::D:
				MoveView(sf::Vector2f(5, 0));
				break;
			case sf::Keyboard::R:
				ResetView();
				break;
			}
		}

		m_stateManager->HandleEvent(&E);
		m_uiManager->HandleEvent(&E);
	}

	if (m_page->isOpen)
		m_page->Update();
	UpdateCursor();
}

void Window::UpdateCursor() {
	if (m_cursorType == 0)
		return;

	ThemeManager* m_themeManager = m_uiManager->GetThemeManager();

	switch (m_cursorType) {
		case 1:
			m_window.setMouseCursor(*m_themeManager->GetCursor(sf::Cursor::Arrow));
			break;
		case 2:
			m_window.setMouseCursor(*m_themeManager->GetCursor(sf::Cursor::Hand));
			break;
		case 3:
			m_window.setMouseCursor(*m_themeManager->GetCursor(sf::Cursor::Text));
			break;
	}
}

void Window::MoveView(sf::Vector2f delta) {
	m_offset += delta;
	sf::Vector2f temp = m_view.getCenter();

	m_view.setCenter(temp + delta);

	m_window.setView(m_view);
}

void Window::ResetView() {
	m_offset = DefaultOffset;
	m_view.setCenter(DefaultOffset + sf::Vector2f(800, 450));
	m_window.setView(m_view);
}

sf::Vector2f Window::GetOffset() {
	return m_offset;
}

Page* Window::GetPage() {
	return m_page;
}

