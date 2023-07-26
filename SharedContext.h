#pragma once
#include "Window.h"
#include "NodeRenderer.h"
#include "UIManager.h"

class NodeRenderer;
class UIManager;
class Window;

struct SharedContext {
	SharedContext() : 
		m_wind(nullptr),
		m_nodeRenderer(nullptr),
		m_uiManager(nullptr) {}

	Window* m_wind;
	NodeRenderer* m_nodeRenderer;
	UIManager* m_uiManager;
};

