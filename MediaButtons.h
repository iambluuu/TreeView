#pragma once
#include "UI_Element.h"
#include "UIManager.h"
#include "NodeRenderer.h"

class UIManager;
class NodeRenderer;

enum class MediaFunction {
	Play,
	Pause,
	Forward,
	SkipForward,
	Backward,
	SkipBackward
};

class MediaButton : public BaseElement
{
	friend class UIManager;
	friend class NodeRenderer;

public:
	MediaButton(UIManager* m_owner, MediaFunction l_function);
	~MediaButton();

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void SetTheme(int l_themeID);

};