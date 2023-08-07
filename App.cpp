#include "App.h"

enum class StateType;

App::App() : m_window("TreeView", sf::Vector2u(1600, 900)), m_stateManager(&m_context),  m_uiManager(&m_stateManager), m_nodeRenderer(&m_stateManager) {
	m_context.m_wind = &m_window;
	m_context.m_nodeRenderer = &m_nodeRenderer;
	m_context.m_uiManager = &m_uiManager;
	m_context.m_wind->SetUIManager(&m_uiManager);
	m_context.m_wind->SetStateManager(&m_stateManager);

	m_stateManager.SwitchTo(StateType::AVLTree);
}

App::~App() {}

void App::HandleInput() {

}

void App::Update() {
	m_window.Update();
	m_stateManager.Update(m_elapsed);
	m_uiManager.Update(m_elapsed);
}

void App::Render() {
	m_window.BeginDraw();
	m_uiManager.Draw();
	m_stateManager.Draw();
	m_window.EndDraw();
}

void App::LateUpdate() {
	m_stateManager.ProcessRequests();
	RestartClock();
}