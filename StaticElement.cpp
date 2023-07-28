#include "StaticElement.h"

StaticElement::StaticElement(UIManager* l_owner, ElementName l_name) {
	m_owner = l_owner;
	m_name = l_name;

	m_themeManager = m_owner->GetThemeManager();
	m_sprite = m_themeManager->GetSprite(m_themeID, m_name, ElementState::Neutral);
}

void StaticElement::HandleEvent(sf::Event* l_event){}
void StaticElement::OnHover(){}
void StaticElement::OnClick(){}
void StaticElement::OnRelease(){}
void StaticElement::OnLeave(){}

void StaticElement::SetTheme(int l_themeID){
	m_themeID = l_themeID;
}

void StaticElement::SetPosition(sf::Vector2f l_pos) {
	m_pos = l_pos;
	m_sprite->setPosition(m_pos);
}

void StaticElement::Update(float l_dT){}

void StaticElement::Draw() {
	sf::RenderWindow* wind = m_owner->GetStateManager()->GetContext()->m_wind->GetRenderWindow();
	wind->draw(*m_sprite);
}

void StaticElement::SetLayer(int l_layer) {
	m_layer = l_layer;
}