#pragma once
#include "UIManager.h"

enum class ElementState { Neutral = 1, Focused, Clicked, Hidden };

class UIManager;

class BaseElement {
protected:
	sf::Sprite* m_sprite{ nullptr };
	sf::Text m_text;
	ElementState m_state;

	int m_layer{ 0 };
	sf::IntRect m_hitBox;
	sf::Vector2f m_pos {0.f, 0.f};

	UIManager* m_owner{ nullptr };
	
	//std::unordered_map<StateType, std::function<void()>> m_func;

public:
	BaseElement() {}

	BaseElement(UIManager* l_owner) {
		m_owner = l_owner;
	}

	virtual void HandleEvent(sf::Event* l_event) = 0;

	virtual void OnHover() = 0;
	virtual void OnClick() = 0;
	virtual void OnRelease() = 0;
	virtual void OnLeave() = 0;
	virtual void Update(float l_dT) = 0;
	virtual void Draw() = 0;
	int GetLayer() { return m_layer; }
};