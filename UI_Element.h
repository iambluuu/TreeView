#pragma once
#include "UIManager.h"

enum class ElementState { Neutral, Focused, Clicked, Hidden };

class UIManager;

class BaseElement {
protected:
	sf::Sprite m_sprite;
	sf::Text m_text;
	ElementState m_state;

	int m_layer{ 0 };
	sf::IntRect m_hitBox;

	UIManager* m_owner{ nullptr };

public:
	BaseElement() {}

	BaseElement(UIManager* l_owner) {
		m_owner = l_owner;
	}

	virtual void HandleInput(sf::Event* l_event) = 0;

	virtual void OnHover(const sf::Vector2f& mousePos) = 0;
	virtual void OnClick(const sf::Vector2f& mousePos) = 0;
	virtual void OnRelease() = 0;
	virtual void OnLeave() = 0;
	virtual void Update(float l_dT) = 0;
	virtual void Draw(sf::RenderWindow* l_target);
};