#include "Window.h"

Window::Window() {
	m_fps = 60;
	Setup("Window", sf::Vector2u(1600, 900));
}

Window::Window(const std::string& l_title, const sf::Vector2u& l_size) {
	m_fps = 60;
	Setup(l_title, l_size);
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
}

void Window::Create() {
	auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	m_window.create({ m_windowSize.x, m_windowSize.y, 32 }, m_windowTitle, style);
}

void Window::Destroy() {
	m_window.close();
}

void Window::ToggleFullscreen() {
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

void Window::BeginDraw() {
	m_window.clear(sf::Color::Black);
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

void Window::Update() {
	sf::Event E;

	while (m_window.pollEvent(E)) {
		if (E.type == sf::Event::Closed) {
			m_isDone = 1;
		}
	}
}