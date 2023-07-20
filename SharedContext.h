#pragma once
#include "Window.h"
#include "NodeRenderer.h"

struct SharedContext {
	SharedContext() : 
		m_wind(nullptr),
		m_nodeRenderer(nullptr),
		m_textureManager(nullptr) {}

	Window* m_wind;
	NodeRenderer* m_nodeRenderer;
	TextureManager* m_textureManager;
};

