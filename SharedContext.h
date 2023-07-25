#pragma once
#include "Window.h"
#include "NodeRenderer.h"

class NodeRenderer;

struct SharedContext {
	SharedContext() : 
		m_wind(nullptr),
		m_nodeRenderer(nullptr) {}

	Window* m_wind;
	NodeRenderer* m_nodeRenderer;
};

