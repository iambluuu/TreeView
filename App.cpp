#include "App.h"

App::App() : m_window("TreeView", sf::Vector2u(1600, 900)), m_stateManager(&m_context) {
	m_context.m_wind = &m_window;
	m_stateManager.SwitchTo(StateType::HashTable);
}

void App::Update() {
	m_window.Update();
	m_stateManager.Update(m_elapsed);
}

void App::Render() {
	m_window.BeginDraw();
	m_stateManager.Draw();
	m_window.EndDraw();
}

void App::LateUpdate() {
	m_stateManager.ProcessRequests();
	RestartClock();
}