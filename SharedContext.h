#pragma once
#include "Window.h"
#include "NodeRenderer.h"

struct SharedContext {
	SharedContext() : m_wind(nullptr) {}

	Window* m_wind;
	NodeRenderer* m_nodeRenderer;

};

