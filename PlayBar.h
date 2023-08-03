#pragma once
#include "UI_Element.h"
#include "UIManager.h"
#include "NodeRenderer.h"

class NodeRenderer;
class UIManager;

class PlayBar : public BaseElement {
	friend class UIManager;
	friend class NodeRenderer;

private:
	UIManager* m_owner{ nullptr };
	NodeRenderer* m_nodeRenderer{ nullptr };
	sf::Sprite* m_playedSprite{ nullptr };
	
public:
	PlayBar(UIManager* m_owner);
	~PlayBar();

	void HandleEvent(sf::Event* l_event);
	void OnHover();
	void OnClick();
	void OnRelease();
	void OnLeave();
	void SetPosition(sf::Vector2f l_pos);
	void Update(float l_dT);
	void Draw();

	void SetTheme(int l_themeID);
	void SetString(const std::string& l_text);
	void GetShownLength();
};

